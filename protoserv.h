// definir le struct de chaque point
#define true 1;
#define false 0;
typedef struct{
	char value;
	int cas;
}point;

void copy(int,point *,int **);
void print(int,int **);
void test(int,int,int **,int *);
