#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<errno.h>

#include"client_lib.h"

#define BUF_LEN 256

int sock,res;
socklen_t client_len;
char buf_read[BUF_LEN];
char buf_write[BUF_LEN];

// envoyer les donnees buf_write a un socket
void envoyer(int sock,char *buf_write){
    res=write(sock,buf_write,strlen(buf_write)+1);
    if(res==-1){
        perror("echec de write");
        exit(1);
    }
}

// recevoir les donnees d'un socket et le stoker dans buf_read
void recevoir(int sock,char *buf_read){
    res=read(sock,buf_read,256);
    if(res==-1){
        perror("echec de read");
        exit(1);
    }
}

int main(int argc,char **argv){
    int i,j,n;
    line_entry_table g;
    if(argc!=3){
        printf("Veuillez saisir les parametres corrects comme:\n");
        printf("./programme adresse_serveur port_serveur\n");
        exit(1);
    }

    /* Create a socket TCP */
    sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock==-1){
        fprintf(stderr,"echec de socket:%s\n",strerror(errno));
        exit(1);
    }

    // mettre les informations d'adresse
    struct sockaddr_in client;
    struct sockaddr_in serveur;

    serveur.sin_family=AF_INET;
    serveur.sin_port=htons(atoi(argv[2]));
    inet_aton(argv[1],&serveur.sin_addr);

    // connecter
    res=connect(sock,(struct sockaddr *)&serveur,sizeof(serveur));
    if(res==-1){
        perror("echec de connect");
        exit(1);
    }

    // atteindre les informations d'adresse et le stocker dans struct sockaddr_in client
    getsockname(sock,(struct sockaddr *)&client,&client_len);

    while(1){
        bzero(buf_read,sizeof(buf_read));
        recevoir(sock,buf_read);

        // pour get la taille de grille et creer le tableau
        if(!strcmp(buf_read,"client connecte")){
            envoyer(sock,"OK1");
            recevoir(sock,buf_read);
            n=atoi(buf_read);
            g=create_board(n,n);

            for(i=0;i<n;i++){
                for(j=0;j<n;j++){
                    g[i][j].checked=0;
                }
            }
            sleep(2);
            continue;
        }
        // Pour get le numero de chaque point
        else if(!strcmp(buf_read,"Saisir le coordonnee:")){
            for(i=0;i<n;i++){
                for(j=0;j<n;j++){
                    envoyer(sock,"OK");
                    bzero(buf_read,sizeof(buf_read));
                    recevoir(sock,buf_read);
                    g[i][j].checked=atoi(buf_read);
                }
            }
            print_board(g,n,n);	    
            printf("Saisir le coordonnee svp:");
        }
        // Pour le cas reussi
        else if(!strcmp(buf_read,"Bravo")){
            printf("%s\n",buf_read);
            break;
        }
        // Juste pour afficher
        else
          printf("%s",buf_read);

        // Saisir et envoyer le coordonnee
        bzero(buf_write,sizeof(buf_write));
        scanf("%s",buf_write);
        envoyer(sock,buf_write);
    }
    return 0;
}
