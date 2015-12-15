#include <stdio.h>
#include <stdlib.h>
#include "client_lib.h"

#define TAILLE 5


/* RAPPEL COORDONEES DANS UN TABLEAU 2D:

   Or de coor (0,0).

   g[axe vertical descendant][axe horizontal de gauche à droite]

   -    -   -   -   - 
   | Or | 5 | 5 | 5 | 5 |
   -    -   -   -   - 
   | 5  | 5 | 5 | 5 | 5 |
   -    -   -   -   - 
   | 5  | 5 | 5 | 5 | 5 |
   -    -   -   -   - 
   | 5  | 5 | 5 | 5 | 1 |
   -    -   -   -   - 
   | 5  | 5 | 5 | 5 | 5 |
   -    -   -   -   -

*/ 

line_entry_table create_board(int size_X,int size_Y){
    int i=0;
    struct box* board_entry_address = malloc(sizeof(struct box) * size_X * size_Y);
    line_entry_table line_tab = malloc(sizeof(struct box*) * size_X);

    for(i=0;i<size_X;i++){
        line_tab[i] = &board_entry_address[i*size_Y];
    }

    return line_tab;
}

void clear_board(line_entry_table g){
    free(g[0]);
    free(g);
}

void print_board(line_entry_table g,int size_X,int size_Y){

    int i=0;
    int j=0;
    int j2;

    for(j2=0;j2<size_Y;j2++){
        printf("  -");
        printf(" ");
    }
    printf("\n");

    for(i=0;i<size_X;i++){
        for(j=0;j<size_Y;j++){

            if(j==0) printf("|");
            printf(" %d |",g[i][j].checked);
            if(j==size_Y-1){
                printf("\n");
                for(j2=0;j2<size_Y;j2++){
                    printf("  -");
                    printf(" ");
                }
                printf("\n");
            }
        }
    }
}      



void check_that_box(line_entry_table g,int x, int y){
    int x1,y1;
    x1=x-1;
    y1=TAILLE-y;
    g[y1][x1].checked=1;

}


int is_same_color(line_entry_table g, int x,int y, int x1, int y1){

    if(g[x][y].checked==g[x1][y1].checked) return 1;
    else return 0;
}


int check_diag_SO_NE(line_entry_table g, int x, int y){ // Diagonale Sud-Ouest -> Nord-Est

    int i=0;
    int check=0;

    for(i=1;i<5;i++){

        if(x+i>=TAILLE || y+i>=TAILLE) return 0; //Si on tape hors de la grille, on renvoit 0

        if(is_same_color(g,x,y,(x+i),(y+i)) == 1 && g[x+i][y+i].checked!=0){ 
            check++;
        }

    }

    if(check == 4) return 1;
    else return 0;
}

int check_diag_NO_SE(line_entry_table g, int x, int y){ // Diagonale Nord-Ouest -> Sud-Est

    int i=0;
    int check=0;

    for(i=1;i<5;i++){

        if(x+i>=TAILLE || y-i>=TAILLE) return 0; //Si on tape hors de la grille, on renvoit 0

        if(is_same_color(g,x,y,(x+i),(y-i)) == 1 && g[x+i][y-i].checked!=0){ 
            check++;
        }
    }

    if(check == 4) return 1;
    else return 0;
}

int check_li_O_E(line_entry_table g, int x, int y){ // Ligne Ouest->Est

    int i=0;
    int check=0;

    for(i=1;i<5;i++){

        if(x+i>=TAILLE) return 0; //Si on tape hors de la grille, on renvoit 0

        if(is_same_color(g,x,y,(x+i),(y)) == 1 && g[x+i][y].checked!=0){ 
            check++;
        }
    }

    if(check == 4) return 1;
    else return 0;
}

int check_col_S_N(line_entry_table g, int x, int y){ // Colonne Sud->Nord

    int i=0;
    int check=0;

    for(i=1;i<5;i++){

        if(y+i>=TAILLE) return 0; //Si on tape hors de la grille, on renvoit 0

        if(is_same_color(g,x,y,(x),(y+i)) == 1 && g[x][y+i].checked!=0){ 
            check++;
        }
    }

    if(check == 4) return 1;
    else return 0;
}




int is_winning (line_entry_table g,int x, int y){ // Vérifie les combinaisons dans le carré autour de la case donnée

    int i=0;
    int j=0;
    int won=0;
    int borneminx=0;
    int borneminy=0;
    int bornemaxx=0;
    int bornemaxy=0;

    if(x-4<0) borneminx=0;
    else borneminx=x-4;

    if(y-4<0) borneminy=0;
    else borneminy=y-4;

    if(x+5>TAILLE) bornemaxx=TAILLE;
    else borneminx=x+5;

    if(y+5>TAILLE) bornemaxy=TAILLE;
    else borneminy=y+5;



    for(i=borneminx;i<bornemaxx;i++){
        for(j=borneminy;j<bornemaxy;j++){

            if((check_col_S_N(g,i,j)==1) || (check_li_O_E(g,i,j)==1) || (check_diag_SO_NE(g,i,j)==1) || (check_diag_NO_SE(g,i,j)==1)) won=1;

        }
    }

    return won;
}


/*
   int main(int argc, char *argv[]){

   int i=0;
   int j=0;
   int size_X=TAILLE;
   int size_Y=TAILLE;
   int res;

   line_entry_table g;

   g=create_board(size_X,size_Y);

   for(i=0;i<size_X;i++){
   for(j=0;j<size_Y;j++){
   g[i][j].checked=5;
   }
   }

   print_board(g,size_X,size_Y);

   res=is_winning(g,3,4);

   printf("\n");

   printf("%d",res);

   printf("\n");

   check_that_box(g,1,1);

   print_board(g,size_X,size_Y);

   printf("\n");

   clear_board(g);

   return 0;
   }
   */
