/*	Systems Programming
 *    Assignment #2 (pa2)
 *    Author:  Jesse Strivelli
 */


#include <stdio.h>
#include <stdlib.h>
#include "sorted-list.h"
#include <string.h>

/* this method includes the */
int compInts(void* a, void* b){
	/* This is for int comparisons */
		int cmp1 = *(int*)a;
		int cmp2 = *(int*)b;
		if(cmp1 < cmp2){
			return -1;
		}
		else if(cmp1 == cmp2){
			return 0;
		}
		else{
			return 1;
		}
		
}	
/*This is for double comparisons*/
int compDoubles(void* a, void* b){

		double cmp3 = *(double*)a;
		double cmp4 = *(double*)b;
		if(cmp3 < cmp4){
			return -1;
		}
		else if(cmp3 == cmp4){
			return 0;
		}
		else{
			return 1;
		}	
}

int stringComp(void* a, void* b){
	/*This is for string comparisons*/
		char *str1 = a;
		char *str2 = b;
		if(strcmp(str1, str2) < 0){
			return -1;
		}
		else if(strcmp(str1, str2) > 0){
			return 1;
		}
		else
			/*This will equal 0*/
			return strcmp(str1, str2);

}

int structCmp(void* a, void* b){
	return 0;
	/*This method compares struct types*/
}

void dfInt(void * a){
	int *b = (int*) a;
	b = NULL;
	return;
}

void dfDouble(void *a){
	double *b = (double*)a;
	b = NULL;
}

void dfString(void *a){
	char *str = a;
	str = NULL;
}

int main(int argc, char** argv){

	SortedListPtr sl = SLCreate(&compInts, &dfInt);
	int* a = malloc(sizeof(int*));

	*a = 34;

	SLInsert(sl, a);
  	SLRemove(sl, a);

}