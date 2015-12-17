/**
 * The server which running continuously to manage the game and all clients
 * 
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/shm.h>

#include"server_lib.h"

int sock,res;
int sock_pipe[100];
int i,port,pid;
int count=0;

/*
 * Sharing memory between processes
 *
 */

// Sharing memory identifier for the board, the score, and the parameter
int shmid_board,shmid_score,shmid_para;

// Sharing memory address for the board, the score, and the parameter
point *shmaddr_board;
int *shmaddr_score;
int *shmaddr_para;

/*
 * Create sharing memory
 * shmaddr_board: 1000 Bytes
 * shmaddr_score:  100 Bytes
 * shmaddr_para :  100 Bytes
 *
 */
void create_shm(){
    // Create a shared memory segment of 1000 Bytes and return an identifier of this shared memory segment
    shmid_board=shmget(IPC_PRIVATE,1000,IPC_CREAT);
    if(shmid_board==-1){
        // Print error information which is currently stored in the system variable errno
        perror("shmget error:");
        exit(1);
    }

    shmid_score=shmget(IPC_PRIVATE,100,IPC_CREAT);
    if(shmid_score==-1){
        perror("shmget error:");
        exit(1);
    }

    shmid_para=shmget(IPC_PRIVATE,100,IPC_CREAT);
    if(shmid_para==-1){
        perror("shmget error:");
        exit(1);
    }

    // Attache the shared memory specified by "shmid_board" to the address space of calling process "shmaddr_board"
    shmaddr_board = shmat(shmid_board,NULL,0);
    if(shmaddr_board == (point *)-1){
        perror("shmat error:");
        exit(1);
    }

    shmaddr_score = shmat(shmid_score,NULL,0);
    if(shmaddr_score == (int *)-1){
        perror("shmat error:");
        exit(1);
    }

    shmaddr_para = shmat(shmid_para,NULL,0);
    if(shmaddr_para == (int *)-1){
        perror("shmat error:");
        exit(1);
    }

    printf("Shared memory segment created.\n");
}

// write content(buf_write) in socket
void envoyer(int sock, char *buf_write){
    res = write(sock, buf_write, strlen(buf_write)+1);
    if(res == -1){
        perror("socket write error.");
        exit(1);
    }
}

// read content from socket to buf_read
void recevoir(int sock, char *buf_read){
    res = read(sock, buf_read, 256);
    if(res == -1){
        perror("socket read error.");
        exit(1);
    }
}

// Creer le tableau de dimention 2
void tab_2d(int n,int **tab){
    int i;
    // distribuer le memoire dynamique
    tab=(int **)calloc(n,sizeof(int *));
    for(i=0;i<n;i++)
      tab[i]=(int *)calloc(n,sizeof(int));
}


int main(int argc,char **argv){
    if(argc!=2){
        printf("please input a port as argument.\n");
        exit(1);
    }

    // Create a shared memory segment
    create_shm();

    // Creer le socket
    sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock==-1){
        fprintf(stderr,"echec de socket:%s\n",strerror(errno));
        exit(1);
    }

    // Mettre les informations d'adresse de socket
    struct sockaddr_in serveur;
    port=atoi(argv[1]);
    serveur.sin_family=AF_INET;
    serveur.sin_port=htons(port);
    serveur.sin_addr.s_addr=INADDR_ANY;

    // Bind
    res=bind(sock,(struct sockaddr*)&serveur,sizeof(serveur));
    if(res==-1){
        fprintf(stderr,"echec de bind:%s\n",strerror(errno));
        exit(1);
    }

    // Listen
    res=listen(sock,100);  // 100 clients au maximum
    if(res==-1){
        fprintf(stderr,"echec de listen:%s\n",strerror(errno));
        exit(1);
    }

    while(1){
        FILE *fp;
        FILE *fp2;
        struct sockaddr_in client;
        socklen_t client_len;
        client_len=0;
        char buf_pwd[256],buf_read[256],buf_write[256];

        // Attendre pour la connextion du client
        sock_pipe[count]=accept(sock,(struct sockaddr *)&client,&client_len);
        if(sock_pipe[count]==-1){
            fprintf(stderr,"echec de accept:%s\n",strerror(errno));
            exit(1);
        }
        // bien connecte
        else{
            // On cree le processure en utilisant fork(), le fils pour l'interaction avec le client, le pere juste pour faire le loop
            pid=fork();
            switch(pid){
                case -1:
                    perror("echec de fork");
                    exit(1);
                case 0:  // fils
                    sprintf(buf_write,"Saisir votre mot de passe svp:");
                    envoyer(sock_pipe[count],buf_write);
                    recevoir(sock_pipe[count],buf_read);

                    // Verify the password to see if it is an administrator
                    fp=fopen("./password/admin","r");
                    if(fp == NULL){
                        perror("Open password file admin error.\n");
                        exit(1);
                    }
                    while(fscanf(fp,"%s",buf_pwd)!=EOF){
                        if(!strcmp(buf_read,buf_pwd)){
                            int i;
                            int nb_client,n,point;

                            printf("admin %d connecte\n",count);
                            fflush(stdout);

                            // Nombre de client
                            sprintf(buf_write,"nombre de client:");
                            envoyer(sock_pipe[count],buf_write);		
                            recevoir(sock_pipe[count],buf_read);	
                            nb_client=atoi(buf_read);
                            // Mettre 0 dans le tableau de note
                            for(i=0;i<nb_client+1;i++)
                              shmaddr_score[i]=0;
                            shmaddr_para[0]=nb_client;

                            // Taille de grille
                            sprintf(buf_write,"La taille de grille:");
                            envoyer(sock_pipe[count],buf_write);
                            recevoir(sock_pipe[count],buf_read);
                            n=atoi(buf_read);
                            shmaddr_para[1]=n;
                            // initialiser le grille
                            for(i=0;i<n*n;i++){
                                shmaddr_board[i].value=0;
                                shmaddr_board[i].cas=true;
                            }

                            // Le point pour gagner
                            sprintf(buf_write,"Le point pour gagner:");
                            envoyer(sock_pipe[count],buf_write);
                            recevoir(sock_pipe[count],buf_read);
                            point=atoi(buf_read);
                            shmaddr_para[2]=point;

                            pause();
                        }
                    }

                    // pour tester si c'est le client
                    fp2=fopen("./password/cli","r");
                    while(fscanf(fp2,"%s",buf_pwd)!=EOF){
                        if(!strcmp(buf_read,buf_pwd)){
                            int x,y,n,i;
                            int **tab;

                            // n=la taille de grille
                            n=shmaddr_para[1];

                            // Pour envoyer la taille de grille a client
                            printf("client %d connecte\n",count);
                            fflush(stdout);
                            envoyer(sock_pipe[count],"client connecte");
                            recevoir(sock_pipe[count],buf_read);
                            if(!strcmp(buf_read,"OK1")){
                                bzero(buf_write,sizeof(buf_write));
                                sprintf(buf_write,"%d",shmaddr_para[1]);
                                envoyer(sock_pipe[count],buf_write);
                            }
                            sleep(1);

                            // Pour envoyer le numero de chaque point
                            while(1){
                                bzero(buf_write,sizeof(buf_write));
                                sprintf(buf_write,"Saisir le coordonnee:");
                                envoyer(sock_pipe[count],buf_write);

                                for(i=0;i<n*n;i++){
                                    recevoir(sock_pipe[count],buf_read);
                                    if(!strcmp(buf_read,"OK")){
                                        bzero(buf_write,sizeof(buf_write));
                                        sprintf(buf_write,"%d",shmaddr_board[i].value);
                                        envoyer(sock_pipe[count],buf_write);
                                    }
                                }

                                // mettre le donnee recu dans le memoire partage
                                bzero(buf_read,sizeof(buf_read));
                                recevoir(sock_pipe[count],buf_read);
                                sscanf(buf_read,"%d,%d",&x,&y);
                                printf("client%d:%d %d\n",count,x,y);
                                shmaddr_board[(x-1)*n+y-1].value=count;
                                shmaddr_board[(x-1)*n+y-1].cas=false;

                                // distribuer les memoire pour le tableau de dimention 2
                                tab=(int **)calloc(n,sizeof(int *));
                                for(i=0;i<n;i++){
                                    tab[i]=(int *)calloc(n,sizeof(int));
                                }

                                // copier la donnee de memoire partage a le tableau local
                                copy(n,shmaddr_board,tab);

                                // Afficher le tableau
                                print(n,tab);

                                // tester si c'est 5 aligne
                                test(count,n,tab,shmaddr_score);

                                // tester si ce client atteint le point pour gagner
                                if(shmaddr_score[count]==shmaddr_para[2]){
                                    printf("Bravo,client %d a gagne!!!\n",count);
                                    fflush(stdout);
                                    // envoyer le message de gagne a chaque client
                                    for(i=0;i<shmaddr_para[0]+1;i++){
                                        envoyer(sock_pipe[i],"Bravo");
                                    }
                                    shmaddr_para[3]=1;
                                    break;
                                }
                            }
                        }
                    }
                    break;
                default:  // Pere: juste pour faire le loop
                    count++;
                    continue;
            }	
        }
        if(shmaddr_para[3]==1)
          break;
    }
    return 0;
}
