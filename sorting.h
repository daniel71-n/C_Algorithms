

/* *********************** Motivation and Overview******************* */

/* This is a collection of routines implementing various sorting 
 * algorithms ranging in efficiency, for comparison and reference 
 * purposes. 
 *
 * For the purposes of illustrating the algorithms, the routines 
 * all operate on an unsorted char-based singly linked list and/or an array. 
 * The specific implementation of the linked list used can be found in 
 * the singly_linked_list source file and associated header file.
 *
 */



/* *********************** Includes ******************* */
#include "singly_linked_list.h"



/* /////////////////////////////////////////////////////////////////
* *********************** Function Prototypes ******************* * 
* /////////////////////////////////////////////////////////////// */

void Sort_bubble(Head *head_ptr);    
/* ----------------- General overview --------------------
       Sort a singly linked list in place, in ascending order. 
       Each node is compared with the one following it, and they're
       swapped if n > n+1.

       Since each node can move at most one position in the list
       for each iteration over the list, this means that the number
       of iterations increases with the length of the list and 
       the extent to which the nodes are out of order.

       ---------------- Performance notes -------------------
       The bubble sort sorting algorithm is notoriously inefficient.
       The worst-case performance of the algorithm rules
       it out as a viable sorting option for any non-trivial sorts.
       
       The main purpose of the bubble sort algorithm should in essence
       be mainly didactic.
*/

void Sort_bubble_array(char chararray[], unsigned int array_length);
/* Array-version implementation of Bubble Sort*/ 


void Sort_selection_array(char chararray[], unsigned int array_length);
/* Array-version implementation of Selection Sort*/ 

void Sort_insertion_array(char chararray[], unsigned int array_length);
/* Array-version implementation of Insertion Sort*/ 

void Sort_quicksort_array(char the_array[], uint16_t index_start, uint16_t index_end);
/* Array-version implementation of the Quicksort algorithm*/ 



