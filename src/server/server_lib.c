#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>

#include"server_lib.h"

int shmid,shmid_note;
point *shmaddr;
int *shmaddr_note;

// copier les donnees de memoire partage a tableau local
void copy(int n,point *shmaddr,int **tab){
	int i,j;
	for(i=0;i<n;i++){
	  for(j=0;j<n;j++){
	    if(!shmaddr[n*i+j].cas)
	      tab[i][j]=shmaddr[n*i+j].value;
	  }
	}
	return;
}

// afficher le grille
void print(int n,int **tab){
    int i,j;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            printf("%d ",tab[i][j]);
            fflush(stdout);
        }
        printf("\n");
        fflush(stdout);
    }
}			

// tester si il y a 5 aligne
void test(int id,int n,int **tab,int *shmaddr_note){
	int i,j;
	int note_tmp=0;
	for(i=0;i<n;i++){
		for(j=0;j<n-4;j++){
		  if(tab[i][j]==id && tab[i][j+1]==id && tab[i][j+2]==id && tab[i][j+3]==id && tab[i][j+4]==id && tab[i][j]!=0){
		    //printf("client %d gagne 1 point\n",id);
		    //fflush(stdout);
		    note_tmp++;
		  }
		}
	}
	for(j=0;j<n;j++){
		for(i=0;i<n-4;i++){
		  if(tab[i][j]==id && tab[i+1][j]==id && tab[i+2][j]==id && tab[i+3][j]==id && tab[i+4][j]==id && tab[i][j]!=0){
		    //printf("client %d gagne 1 point\n",id);
		    //fflush(stdout);
		    note_tmp++;
		  }
		}
	}
	for(i=0;i<n-4;i++){
		for(j=4;j<n;j++){
		  if(tab[i][j]==id && tab[i+1][j-1]==id && tab[i+2][j-2]==id && tab[i+3][j-3]==id && tab[i+4][j-4]==id && tab[i][j]!=0){
		    //printf("client %d gagne 1 point\n",id);
		    //fflush(stdout);
		    note_tmp++;
		  }
		}
	}
	for(i=0;i<n-4;i++){
		for(j=0;j<n-4;j++){
		  if(tab[i][j]==id && tab[i+1][j+1]==id && tab[i+2][j+2]==id && tab[i+3][j+3]==id && tab[i+4][j+4]==id && tab[i][j]!=0){
		    //printf("client %d gagne 1 point\n",id);
		    //fflush(stdout);
		    note_tmp++;
		  }
		}
	}
	printf("client %d:%d points\n",id,note_tmp);
	fflush(stdout);
	if(note_tmp==(shmaddr_note[id]+1))
		shmaddr_note[id]=note_tmp;
	return;
}
