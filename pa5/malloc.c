#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>   
#include <string.h>
#include <errno.h>
#include "malloc.h"
#include "sorted-list.h"

//define Blocksize (1024)
//static char Bigblock[Blocksize];
struct SortedList* slist;
static struct MemEntry *root = 0, *last = 0;

int compointer(void* x, void* y){
	return x - y;  
}
void destroyit(void * given){
  free(given);
}

void myFree(void *p, char* filename, int linenumber){
	struct MemEntry *ptr,*prev,*succ;
    if (slist == NULL || SLSearch(slist,p) == 0) {
		printf("No memory was ever malloced: Free Denied. \n");
		//exit(0);
		return;
	}
    ptr = (struct MemEntry *)((char *)p - sizeof(struct MemEntry));
	if ( (prev = ptr->prev) != 0 && prev->isfree ){
		prev->size += sizeo1f(struct MemEntry) + ptr->size;	// merge with predecessor
		prev->succ = ptr->succ;
		ptr->isfree = 1;
		prev->succ = ptr->succ;
		if(ptr->succ != 0){
			ptr->succ->prev = prev;
		}
		if (SLRemove(slist, ptr, ptr->size) == 1) {
			//printf("%s\n", ",,,,,,,,,,,,,,,,,,,,,,,,,,,,,removesuccess1");
		}
		printf( "Freeing block %#x, merging with previous block, new block size is %d.\n",p, prev->size );
	}
	else{   
        if (ptr->isfree == 0) {
        	printf( "Freeing block %#x.\n", p );
        	
        	if (SLRemove(slist, ptr, ptr->size) == 1) {
				//printf("%s\n", ",,,,,,,,,,,,,,,,,,,,,,,,,,,,,removesuccess2");
			}
            ptr->isfree = 1;
            prev = ptr;
            last = 0;
        } else printf("Error: Double Freeing. Free Denied.\n");
	}
	if ( (succ = ptr->succ) != 0 && succ->isfree ){
			prev->size += sizeof(struct MemEntry) + succ->size;	
			prev->succ = succ->succ;
			prev->isfree = 1;
       		if(succ->succ != 0){
				succ->succ->prev=prev;
       		}
			SLRemove(slist, ptr, ptr->size);
			printf( "Merging with next block, new block size is %d\n", prev->size );
		}
	
}

void *myMalloc(unsigned int size, char* filename, int linenumber){
	struct MemEntry *p,*succ;
	void *                  ret;
    p = root;
    //printf("%s%#x\n","the first size: ",size );
    //printf("%s%#x\n", ")))))))))))))))))))last " ,last);
	while ( p != 0 ){
		if ( p->size < size ){
			p = p->succ;
			//printf("%s\n", "1st");				
		}
		else if ( !p->isfree ){
			p = p->succ;	
			//printf("%s\n", "2st");	
		}			
		else if ( p->size < (size + sizeof(struct MemEntry)) ){
			p->isfree = 0;					
			ret = (char *)p + sizeof(struct MemEntry);
			if (SLInsert(slist, ret, size) == -1){
			//printf("%s\n", "problem");
			return NULL;
			//printf("%s\n", "((((((((((((((((((((((((((((((here");
		}
			//printf("%s\n", "3rc");
			printf( "Given memory is < previously freed memory, new block with size %d\n", size );
			return ret;
		}
		else{
			
			succ = (struct MemEntry *)((char *)p + sizeof(struct MemEntry) + size);
			succ->prev = p;
			succ->succ = p->succ;
			if(p->succ != 0){
				p->succ->prev = succ;
			}
			p->succ = succ;
			succ->size = p->size - sizeof(struct MemEntry) - size;
			succ->isfree = 1;
			p->size = size;
			p->isfree = 0;
			last = (p == last) ? succ : last;
			ret = (char *)p + sizeof(struct MemEntry);
			if (SLInsert(slist, ret, size) == -1){
				//printf("%s\n", "problem");
				//printf("%s\n", "((((((((((((((((((((((((((((((here");
				return NULL;
			}
			//printf("%s\n", "4th");
			printf( "Given memory is <= previously freed memory, new block with size %d\n", size );
			return ret;
		}
	}
	if ((p = (struct MemEntry*)sbrk( sizeof(struct MemEntry) * size )) == (void *)-1){
		//printf("%s\n","^^^^^^^^^^^^^^^^^^^^^^^^^^^this" );
		return 0;
	}
	else if ( last == 0 ){
		p->prev = 0;
		p->succ = 0;
		p->size = size;
		p->isfree = 0;
		root = last = p;
		//printf("%s%#x\n", ")))))))))))))))))_____))last " ,last);
		ret = (char *)p + sizeof(struct MemEntry);
		slist = SLCreate(compointer,destroyit);
		//SLInsert(slist, ret);
		
		if (SLInsert(slist, ret, size) == -1){
			//printf("%s\n", "problem");
			return NULL;
			//printf("%s\n", "((((((((((((((((((((((((((((((here");
		}
		printf( "First chunk size created. With size %d\n", size );
		return ret;
	}
	else{
		p->prev = last;
		p->succ = last->succ;
		p->size = size;
		p->isfree = 0;
		last->succ = p;
		last = p;
		ret = (char *)p + sizeof(struct MemEntry);
		if (SLInsert(slist, ret, size) == -1){
			//printf("%s\n", "problem");
			return NULL;
			//printf("%s\n", "((((((((((((((((((((((((((((((here");
		}
		printf( "Another chunk size created. With size %d\n", size );
		return ret;
	}
	return 0;
}

