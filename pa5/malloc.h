#ifndef MALLOC_H
#define MALLOC_H

struct MemEntry{
	struct MemEntry *prev,*succ;
	int isfree;
	unsigned int size;
};

void *myMalloc(unsigned int size, char* filename, int linenumber);
void myFree(void* p, char* filename, int linenumber);


#endif