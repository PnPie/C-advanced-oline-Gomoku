#ifndef _CLIENT_LIB_H_
#define _CLIENT_LIB_H_

struct box{
    int checked;
};

typedef struct box** line_entry_table;

line_entry_table create_board(int size_X,int size_Y); // create and send back a 2d-array of struct box

void clear_board(line_entry_table g); //free the space allocated in create_board

void print_board(line_entry_table g,int size_X,int size_Y); // print the array
  
void check_that_box(line_entry_table g,int x, int y); // usefull in solo game, pretty useless now, since the server has to check the box according to the player's number
 
int is_same_color(line_entry_table g, int x,int y, int x1, int y1); //check if the boxes (x,y) and (x1,y1) belong to the same player

int check_diag_SO_NE(line_entry_table g, int x, int y); // Check if the diagonal from South-West to North-East, of lenght 5, is constituted of checked boxes belonging to the same player

int check_diag_NO_SE(line_entry_table g, int x, int y); // same with NW-SE diag

int check_li_O_E(line_entry_table g, int x, int y); // same W-E line

int check_col_S_N(line_entry_table g, int x, int y); //same S-N column

int is_winning (line_entry_table g,int x, int y); // Check if there is any 5-checked-boxes alignement that uses box (x,y)

#endif
