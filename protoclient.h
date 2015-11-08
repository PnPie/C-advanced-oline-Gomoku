#include <stdio.h>
#include <stdlib.h>

struct box;

typedef struct box ** grille;

grille creer_grille(int Taillex,int Tailley); // create and send back a 2d-array of struct box

void vider_grille(grille g); //free the space allocated in creer_grille

void affiche_grille(grille g,int Taillex,int Tailley); // print the array
  
void check_that_box(grille g,int x, int y); // usefull in solo game, pretty useless now, since the server has to check the box according to the player's number
 
int is_same_color(grille g, int x,int y, int x1, int y1); //check if the boxes (x,y) and (x1,y1) belong to the same player

int check_diag_SO_NE(grille g, int x, int y); // Check if the diagonal from South-West to North-East, of lenght 5, is constituted of checked boxes belonging to the same player

int check_diag_NO_SE(grille g, int x, int y); // same with NW-SE diag

int check_li_O_E(grille g, int x, int y); // same W-E line

int check_col_S_N(grille g, int x, int y); //same S-N column

int is_winning (grille g,int x, int y); // Check if there is any 5-checked-boxes alignement that uses box (x,y)


