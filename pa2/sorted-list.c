
/*	Systems Programming
 *    Assignment #2 (pa2)
 *    Author:  Jesse Strivelli
*/

#include <stdio.h>
#include <stdlib.h>
#include "sorted-list.h"

/*
 *Function to create a Sorted-List Node
 *data: data to be inserted into Node
 *next: pointer to next node in the Sorted-List
 *returns a pointer to the newly created SL Node
*/ 
SLnode *createLnode(void* data, SLnode* next){
		SLnode* a = (SLnode *)malloc(sizeof(SLnode));
		a->data = data;
		a->next = next;
		a->ref = 1;
		return a;
}

/*this method destroys and frees the memory for an SLnode once it is out of or dereferenced from the SortedList*/
void SLnodeDestroy(SLnode* a){
	free(a);
}

/*This method allocates memory for a sorted list structure that will hold SLnode structs 
 * cf: comparitive function that returns an integer value of either -1, 0 , 1
 * df: destroys a referenced node of a void* type
 * head: the list starts empty so we have h
 */
SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df){
	SortedListPtr sList = (SortedListPtr)malloc(sizeof(struct SortedList));
	sList->cf= cf;
	sList->df= df;
	sList->head = NULL;
	return sList;
}

int SLInsert(SortedListPtr list, void *newObj){

	int cmp;
	void* objPtr;
	
	SortedListIteratorPtr iter = SLCreateIterator(list);
	SLnode* prev = iter->ptr;
	/*The list is empty so we will insert at the head of the list*/
	if(list == NULL){
		return 0;
	}
	if (iter->ptr == NULL){		
		list->head = createLnode(newObj,iter->ptr);
		SLDestroyIterator(iter);
		return 1;
	}
	/*The data is bigger than the current head so we will insert to the data at the head*/
	else if((*list->cf)(newObj, SLGetItem(iter)) == 1){
			list->head = createLnode(newObj,iter->ptr);	
			SLDestroyIterator(iter);
			return 1;
	}

	objPtr =SLNextItem(iter);
	
	while(iter->ptr != NULL){
		/*We are using the comparitive function given to us to determine the difference between the two datas*/
		cmp = (*list->cf)(newObj, objPtr); 
		/*Detroy process because of duplicate values*/
		if(cmp == 0){						
			SLDestroyIterator(iter);
			return 1;
		}
		/*We can break out of the loop because we found what we are looking for */
		else if(cmp >= 1){						
			break;
		}
		/*This is else is traversing through the list*/
		else{		
			prev = iter->ptr;
			objPtr=SLNextItem(iter);					
		}
	}
	/*This is the insertion*/
	prev->next = createLnode(newObj,iter->ptr);		
	SLDestroyIterator(iter);
	return 1;

}


int SLRemove(SortedListPtr list, void *newObj){

	/*The list is empty*/
	if(list == NULL){
		return 0;
	}
	if(list->head == NULL){
		SLDestroy(list);
		return 0;
	}

	int cmp;
	void* objPtr;

	SortedListIteratorPtr iter = SLCreateIterator(list);
	SLnode *prev;
	prev = iter->ptr;
	cmp = (*list->cf)(newObj,list->head->data);
	/* Node will be at the head of list because the two are equal */
	if(cmp == 0){
		list->head = SLNextItem(iter);
		/* The only two pointers on this node would be the head and the iterator so than we can just destroy the node and iterator */
		if (iter->ptr->ref == 2){ 
			SLnodeDestroy(iter->ptr);
		}
		SLDestroyIterator(iter);
		return 1;
	}
	/*Setting objPtr to the next node*/
	objPtr = SLNextItem(iter);
	

	while(iter->ptr != NULL){ 
		cmp = (*list->cf)(newObj, objPtr);
		
		/*We have to keep iterating */
		if(cmp != 0) {

			prev = iter->ptr;
			objPtr = SLNextItem(iter);
		}
		/* if the comparator is zero than it means we found what we are looking for*/
		else{
			/*This is the actual removal of the node we are looking for*/
			prev->next = iter->ptr->next;
			if(iter->ptr->ref == 2){
				SLnodeDestroy(iter->ptr);
			}
			SLDestroyIterator(iter);
			return 1;
		}

	}
	printf("We can't find the object\n");
	SLDestroyIterator(iter);
	return 0;


}
/*This method destroys a Sorted List Struct*/
void SLDestroy(SortedListPtr list){
	SortedListIteratorPtr iter = SLCreateIterator(list);
	while(iter->ptr != NULL){
		list->head->data = NULL;
		(*list->df)(list->head->data);
		SLnodeDestroy(list->head);
		list->head = SLNextItem(iter);
	} 
	SLDestroyIterator(iter);
	free(list);
}

/*This is an Iterator struct
	When it is created we have it pointing to the head of the list and increment the reference to the node it is pointing to.
	If the iterator is moved from a node like the head for example than the ref decrements and it will increase the ref to the node it is now pointing to.
*/
SortedListIteratorPtr SLCreateIterator(SortedListPtr list){

	SortedListIteratorPtr iter = (SortedListIteratorPtr)malloc(sizeof(struct SortedListIterator));
	iter->ptr = list->head;
	 /* there is another ref pointer to the front node of the list */
	if(iter->ptr != NULL){
		list->head->ref += 1;
	}	
	return iter;

}

void SLDestroyIterator(SortedListIteratorPtr iter){
	/* this checks if the node we are pointing to still exists */
	if(iter->ptr == NULL){
		return;
	}
	else{	
		/* If it is less than 2 than we know this is the only ref pointer and then we can destroy the node */
		if(iter->ptr->ref < 2){
			SLnodeDestroy(iter->ptr);
		}
		else{
			iter->ptr->ref--;
		}
	}
	free(iter);
	return;
}
void * SLNextItem(SortedListIteratorPtr iter){
	/* we want a temporary node pointer set to the next node */
	SLnode* temp = iter->ptr->next; 
	if(iter->ptr->ref > 1){ 
		iter->ptr->ref--;
	}
	else{
		SLnodeDestroy(iter->ptr);
	}
	iter->ptr = temp;
	/* We are at the end of the list */
	if(iter->ptr == NULL){
		return NULL;
	}
	else{
		iter->ptr->ref += 1;
		return iter->ptr->data;
	}

}

void * SLGetItem( SortedListIteratorPtr iter ){
	return iter->ptr->data;
}

