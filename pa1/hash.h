#ifndef ELEMENTS_H
#define ELEMENTS_H
/*
 * inverted-indexer.h
 */

#include <stdlib.h>
#include "sorted-list.h"


/*
 *	This structure contains an array of sorted lists that contain strings that
 *	point to nodes containing tokenizers which are in sorted order.
 */

struct Element{
    SortedList *index[40];
};


typedef struct Element Element;


/*	 
 *	This will convert a string value to an integer 
 *  that maintains the index spot in the array of Keys
 *	that will helps us maintain order of the array in constant time
 */
int hashFunct(char* );


/*
 * This will help identify which Element index spot we have.
 * Some elements could be NULL if the index spot in the array of
 * Elements is empty.
 */
Element *createElement();

/*
 * This will destroy an element in the array
 */
void destroyElement(Element *);



int elementInfo(Element *, char *, char *)


#endif