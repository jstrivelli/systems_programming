#include <stdio.h>
#include "malloc.h"

#define malloc(x) myMalloc( x, __FILE__ , __LINE__ )
#define free(x)   myFree( x, __FILE__, __LINE__ )



int main(int argc, char const *argv[]){
	int x;
	free(&x);
	printf("%s\n","+++++++++++++++++++++++++++++++++++++++++++" );
	char *p = (char*)malloc(200);
	free(p+10);
	printf("%s\n","+++++++++++++++++++++++++++++++++++++++++++" );
	free(p);
	free(p);
	printf("%s\n","+++++++++++++++++++++++++++++++++++++++++++" );
	p = (char*)malloc(100);
	free(p);
	p = (char*)malloc(100);
	free(p);
	printf("%s\n","+++++++++++++++++++++++++++++++++++++++++++" );
	
	printf("%s\n","+++++++++++++++++++++++++++++++++++++++++++" );
	char *q = (char*)malloc(300);
	char *w = (char*)malloc(900);
	
	
	printf("%s\n","+++++++++++++++++++++++++++++++++++++++++++" );
	
	return 0;
}