#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorted-list.h"


/*
 * SLCreate creates a new, empty sorted list.  The caller must provide
 * a comparator function that can be used to order objects that will be
 * kept in the list, and a destruct function that gets rid of the objects
 * once they are no longer in the list or referred to in an iterator.
 * 
 * If the function succeeds, it returns a (non-NULL) SortedListT object,
 * otherwise, it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df) {
	SortedListPtr create = malloc(sizeof(struct SortedList));
	create->totalsize = 0;
	create->start = NULL;
	create->cmp = cf;
	create->dst = df;
	return create;
}
/*
 * When your sorted list is used to store objects of some type, since the
 * type is opaque to you, you will need a comparator function to order
 * the objects in your sorted list.
 *
 * You can expect a comparator function to return -1 if the 1st object is
 * smaller, 0 if the two objects are equal, and 1 if the 2nd object is
 * smaller.
 *
 * Note that you are not expected to implement any comparator or destruct
 * functions.  You will be given a comparator function and a destruct
 * function when a new sorted list is created.
 */


/*
 * SLDestroy destroys a list, freeing all dynamically allocated memory.
 *
 * You need to fill in this function as part of your implementation.
 */
void SLDestroy(SortedListPtr list){
	node* ptr = list->start;
	if (ptr != NULL) {
		while (ptr != NULL){
			ptr = ptr->next;
			free(ptr);
		}
	}
	else {
		free(ptr);
	}
	free(list);	
}


/*
 * SLInsert inserts a given object into a sorted list, maintaining sorted
 * order of all objects in the list.  If the new object is equal to a subset
 * of existing objects in the list, then the subset can be kept in any
 * order.
 *
 * If the function succeeds, it returns 1, othrewise it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

int SLInsert(SortedListPtr list, void *newObj, int size){
	/*make space for new node and set its values*/
	node* insertnode = malloc(sizeof(node));
	insertnode->count = 1;
	insertnode->data = newObj;
	insertnode->next = NULL;
	/*increase items in list*/
	int checkpoint = list->totalsize + size;
	//printf("\n%s%d\n","In SLInsert: " ,list->totalsize + size);
	if (checkpoint > 1024 ) {
		//printf("%s%d\n","In SLInsert: " ,list->totalsize + size);
		printf("%s\n","Error: Memory to allocate is greater than total block size." );
		printf("Need %d bytes more to complete allocation. \n", checkpoint -1024  );
		exit(0);
		return -1;
	} else list->totalsize = checkpoint;
	/*nothing in list so add to begginning of list*/
	if (list->start == NULL){
		list->start = insertnode;
		return 1;
	}
	/*new object is greater than object in head of list so should be beggining of list*/
	if (list->cmp(newObj,list->start->data) == 1){
		insertnode->next = list->start;
		list->start = insertnode;
		return 1;
	}
	/*case where only 1 node in LL so this will be end*/
	if (list->start->next == NULL && list->cmp(newObj,list->start->data) < 1){
		list->start->next = insertnode;
		return 1;
	}
	node* ptr = list->start->next;
	node* prev = list->start;
	while (prev->next != NULL){
		/*case where LL > 2  nodes and insert node is inserted at end*/
		if (ptr->next == NULL && (list->cmp(newObj,ptr->data) == 0 || list->cmp(newObj,ptr->data) < 1)){
			ptr->next = insertnode;
			return 1;
		}
		/*case where LL > 2 nodes and insertnode in put anywhere in middle*/
		if (list->cmp(newObj,ptr->data) == 0 || list->cmp(newObj,ptr->data) == 1){
			prev->next = insertnode;
			insertnode->next = ptr;
			return 1;
		}
		prev = ptr;
		ptr = ptr->next;
	}
	return 0;
}

int SLSearch(SortedListPtr slist, void *p){
	node* ptr = slist->start;
	if (ptr == NULL){
		return 0;
	}
	else if (slist->cmp(ptr->data,p) == 0){
		//printf("%s\n",";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;search");
		return 1;
	}
	else {
		ptr = ptr->next;
		while (ptr != NULL) {
			if (slist->cmp(ptr->data,p) == 0){
				//printf("%s\n",";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;search");
				return 1;
			}
			ptr = ptr->next;
		}
	}
	return 0;
}

void printitout(SortedListPtr list){
	node* starter = list->start;
	while (starter != NULL){
		printf("%c\n",*(char*)(starter->data));
		starter = starter->next;
	}
}

/*
 * SLRemove removes a given object from a sorted list.  Sorted ordering
 * should be maintained.  SLRemove may not change the object whose
 * pointer is passed as the second argument.  This allows you to pass
 * a pointer to a temp object equal to the object in the sorted list you
 * want to remove.
 *
 * If the function succeeds, it returns 1, otherwise it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

int SLRemove(SortedListPtr list, void *newObj, int size){
	node* ptr = list->start;
	/*object is in front of LL and LL is only 1 node*/
	//printf("%s%d\n","In SLRemove: ", list->totalsize - size);

	if(list->cmp(newObj,ptr->data) == 0 && ptr->next == NULL){
		free(ptr->data);
		free(ptr);
		list->start = NULL;
		list->totalsize = list->totalsize - size;
		return 1;
	}
	if(list->cmp(newObj,ptr->data) == 0 && ptr->next != NULL){
		list->start = ptr->next;
		free(ptr->data);
		free(ptr);
		list->totalsize = list->totalsize - size;
		return 1;
	}
	node* prev = ptr;
	ptr = ptr->next;
	while (ptr != NULL){
		/*object is at end of LL*/
		if (ptr->next == NULL && list->cmp(newObj,ptr->data) == 0){
			prev->next = NULL;
			free(ptr->data);
			free(ptr);
			list->totalsize = list->totalsize - size;
			return 1;
		}
		/*object in middle and LL >= 3 nodes*/
		if (list->cmp(newObj,ptr->data) == 0){
			prev->next = ptr->next;
			free(ptr->data);
			free(ptr);
			return 1;
			list->totalsize = list->totalsize - size;
		}
		prev = ptr;
		ptr = ptr->next;
	}
	/*object not in LL*/
	return 0; 
}



/*
 * SLCreateIterator creates an iterator object that will allow the caller
 * to "walk" through the list from beginning to the end using SLNextItem.
 *
 * If the function succeeds, it returns a non-NULL pointer to a
 * SortedListIterT object, otherwise it returns NULL.  The SortedListT
 * object should point to the first item in the sorted list, if the sorted
 * list is not empty.  If the sorted list object is empty, then the iterator
 * should have a null pointer.
 *
 * You need to fill in this function as part of your implementation.
 */

SortedListIteratorPtr SLCreateIterator(SortedListPtr list){
	SortedListIteratorPtr ptr = malloc(sizeof(struct SortedListIterator));
	if (list == NULL){
		return NULL;
	}
	else {

		ptr->item = list->start;
		list->start->count++;
		return ptr;
	}
}


/*
 * SLDestroyIterator destroys an iterator object that was created using
 * SLCreateIterator().  Note that this function should destroy the
 * iterator but should NOT affect the original list used to create
 * the iterator in any way.
 *
 * You need to fill in this function as part of your implementation.
 */

void SLDestroyIterator(SortedListIteratorPtr iter){
	free(iter);
}


/*
 * SLGetItem returns the pointer to the data associated with the
 * SortedListIteratorPtr.  It should return 0 if the iterator
 * advances past the end of the sorted list.
 * 
 * You need to fill in this function as part of your implementation.
*/

void * SLGetItem( SortedListIteratorPtr iter ){
	if (iter->item != NULL){
		return iter->item->data;
	}
	else{
		return 0;
	}
}

/*
 * SLNextItem returns the pointer to the data associated with the
 * next object in the list encapsulated by the given iterator.
 * It should return a NULL when the end of the list has been reached.
 *
 * One complication you MUST consider/address is what happens if a
 * sorted list encapsulated within an iterator is modified while that
 * iterator is active.  For example, what if an iterator is "pointing"
 * to some object in the list as the next one to be returned but that
 * object is removed from the list using SLRemove() before SLNextItem()
 * is called.
 *
 * You need to fill in this function as part of your implementation.
 */

void * SLNextItem(SortedListIteratorPtr iter){
	node* ptr = iter->item;
	/*if current iterator is null or next one is null while LL 
	is <= 2 nodes*/
	if (ptr == NULL || ptr->next == NULL){
		ptr->count--;
		return NULL;
	}
	ptr->count--;
	node* nextptr = ptr->next;
	/*if count is 1 or greater then then 1 or more
	 persisten pointers pointingjust */
	if(nextptr != NULL && ptr->count >= 1){
		nextptr->count++;
		iter->item = iter->item->next;
		return nextptr->data;
	}
	/*used slremove to pointer*/
	if (nextptr != NULL && ptr->count == 0) {
		nextptr->count++;
		iter->item = iter->item->next;
		return nextptr->data;
	}
	return NULL;
}