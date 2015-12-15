#ifndef _SERVER_LIB_H_
#define _SERVER_LIB_H_

#define true 1;
#define false 0;

// The structure of every point
typedef struct{
	char value;
	int cas;
}point;

void copy(int,point *,int **);
void print(int,int **);
void test(int,int,int **,int *);

#endif
