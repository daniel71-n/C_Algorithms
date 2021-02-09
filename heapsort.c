#include "heapsort.h"
#include <stdlib.h>


/* ************************************************ */
/* --------------- IMPLEMENTATION NOTES ------------*/
/*                      * * * 
    The heap used in this implementation is a max heap:
    the root of the heap is always the maximum value in
    the heap.

    The heap implementation was done with making heapsort
    as efficient as possible in mind (as explained below).

    The heap is stored implicitly in an array. This array
    is specified as an argument to Heap_init(), along with
    the size of the array (number of elements it can hold),
    not counting the terminating Nul character, mandatory
    in strings.

    This character is never resized. This makes this 
    implementation suitable to one-time sorts : given an
    unsorted array, sort it. 
    However, if if the array is to be resized, that has to
    be done manually, and heapsort has to be run again on it
    in order to sort it. 
    If the resizing of the array were to be included in the
    
    Because the heap is sorted implicitly in the array to
    be sorted, the sorting is done in place and the space
    complexity is constant. No further space is required
    aside from that required by the functions operating
    on the heap and the memory allocated for a temporary
    Heap struct, which the functions do internally. 

                * * *

    The way the sorting is done is as follows. 
    There's a single public-facing function, 
    Heap_sort(). 
    Internally, this calls two other functions (which,
    in turn, each call one other function), 
    i.e. the sorting consists of two main parts:
    a) max-heapify-ing the array that's supposed to
    be sorted and b) sorting the array by operating on
    the heap. 

    a) The former turns the array into a max-heap. That is,
    the array is iterated over, starting at the second item.
    As the index variable i moves to the right, from 0 to n, 
    the element at i is added to the heap, which runs from 
    0 to i.
    The new element is then sifted-up the heap to the right
    place to maintain the 'heap property'. 

    The 'shape property' of the heap is maintained automatically
    by virtue of the fact that the newly inserted item's position,
    at i+1, is always such that the heap is an almost-complete binary
    tree. This is the great advantage of using an implicit
    heap as opposed to a tree-based explicit implementation
    (see minheap_ex.c). The next insertion spot is always i+1.

    At the end of this process, the array has been turned into
    a max heap, where the parent is always greater than both
    of its children.

    The max-heapify function returns a Heap 'object' (a struct ptr)
    which serves as a way to pass multiple parameters (its internal
    values) to the Heap_popS (heap pop, sort) function. That is,
    the only use for the returned value is to be passed as an 
    argumetn to Heap_popS().

    b) The latter is accomplished by a call to the aforementioned
    Heap_popS().
    The sorting is done by removing the value of root of the heap
    [index 0 in the array], and placing it as the last item in the
    array.
    This effectively partitions the array into two sections again: 
    the heap, where repeated pop operations are performed, and
    the sorted array, to the right of the heap, where the removed
    root is always placed, from right to left.
    The sorted array keeps growing and the heap section keeps 
    shrinking with each root pop operation performed. 
    Specifically, since the root is removed, the heap becomes 
    smaller by one. So the index variable tracking the last
    heap node in the array, gets decremented by one. This index
    is out of the control of the heap from this point, and part of
    the sorted array section, so the (former) root is placed here.

    In place of the removed root, the last node in the heap (the value 
    of the index variable mentioned above, which was decremented)
    is taken and placed there. This breaks the heap property,
    so the new root is sifted down the heap until the correct
    position is found and the heap property is restored. 

    This is repeated until the root of the heap is left, at which
    point that's the smallest item in the array, with all the
    former heap rootsto the right of it, in ascending order. 
    The sorting is done. 
    
    Heap_popS() will internally deallocate the memory allocated
    by Heap_max_heapify() for Heap, so nothing else needs to be
    done.

*  -------------------------------------------------------------- */
/* ************************************************************** */


// a max heap stored implicitly in an array of fixed size
struct max_heap_implicit_fs{
    int32_t last_index;
    char *array;    // array to store the heap in; this is fixed in size, never grown or shrunk
    int32_t size;
};

typedef struct max_heap_implicit_fs *Heap;







/* ********************************************************************************* */
/* ---------------------------- PRIVATE FUNCTIONS ---------------------------------- */

static void Heap_init(Heap *heap_ref, char the_array[], int32_t size_of_the_array){
    /* size_of_the_array has to be number of items in the_array, excluding Nul
       
       allocate memory for a min_heap_implicit struct
    */
    Heap new_max_heap = malloc(sizeof(struct max_heap_implicit_fs));
    new_max_heap->array = the_array;

    if (!new_max_heap){
        exit(EXIT_FAILURE);
    }
     
    new_max_heap->last_index = -1;
    new_max_heap->size = size_of_the_array;     

    *heap_ref = new_max_heap;
}

   
 
static void Heap_destroy(Heap *heap_ref){
    /* Free all memory associated with the heap
       and set *heap_ref to NULL.
    */
    if (!(*heap_ref)){
        return;
    }

    free(*heap_ref);
    *heap_ref = NULL;
}


static void Heap_sift_up(char the_array[], int32_t current_index){
    /* Sift the value at current index up in the array */
    char temp;
    int32_t parent = (current_index-1)>>1;  // parent is (current-1) / 2 (floor division)

    while( parent >= 0){
        if(the_array[parent] < the_array[current_index]){
           temp = the_array[current_index];
           the_array[current_index] = the_array[parent]; 
           the_array[parent] = temp;

           current_index = parent;
           parent = (current_index-1)>>1;
        }
        else{   // don't sift if parent !< current
            break;
        }

    } 
};



static void Heap_sift_down(char the_array[], int32_t current_index, int32_t last_index){
    /* Sift down the root to its correct position in the_array so
       as to repair and uphold the heap property (children > parent).
    */
    int32_t current = current_index;
    int32_t left_child = 1 + (current<<1);
    int32_t right_child = 2 + (current<<1);
   
    char temp;
    
    while(left_child <= last_index){
        // has left child at least
        int32_t child = left_child;

        // check if it also has a right child. If it does, get the larger of the two
        if (right_child <= last_index){
            child = (the_array[left_child] > the_array[right_child]) ? left_child : right_child;
        }
        if (the_array[child] > the_array[current]){
            temp = the_array[child];
            the_array[child] = the_array[current];
            the_array[current] = temp;
        
            current = child;
            left_child = 1 + (current<<1);
            right_child = 2 + (current<<1);
        }
        else{
            break;
        }
            
    }
}

// top-down heap construction
Heap static Heap_max_heapify_td(char the_array[], int32_t size){
    /* Start at the beginning of the array, and grow  the heap
       by inserting into it the next index.
       Heap_sift_up() is called on the heap then to reestablish 
       the heap property.

       The return value is a Heap : initialized properly and everything, to be
       passed to Heap_pops().
    */
    
    Heap max_heap;
    Heap_init(&max_heap, the_array, size);
    
    int32_t last_index = size-1; 
    // start at 1: consider the root already inserted
    for(int32_t i = 1; i <= last_index; i++){
        Heap_sift_up(the_array, i);
    }
    // the array is maxheapified. The max heap has been built.
    // update the max_heap Heap
    max_heap->last_index = last_index;
    max_heap->size = size;
    max_heap->array = the_array;

    return max_heap;
};
    

//bottom-up heap construction -- more efficient than the top-top approach
Heap static Heap_max_heapify_bu(char the_array[], int32_t size){
    Heap max_heap;
    Heap_init(&max_heap, the_array, size);
    
    int32_t last_index = size-1; 
    int32_t first_non_leaf = (last_index-1)>>1;
    // start at 1: consider the root already inserted
    for(; first_non_leaf >= 0; first_non_leaf--){
        Heap_sift_down(the_array, first_non_leaf, last_index);
    }
    // the array is maxheapified. The max heap has been built.
    // update the max_heap Heap
    max_heap->last_index = last_index;
    max_heap->size = size;
    max_heap->array = the_array;

    return max_heap;



}

static void Heap_popS(Heap *heap_ref){
    /* Repeatedly remove the root of the heap
       (the max value) and put it at the end of the heap,
       then shrink the heap section by one.

       Deallocate the space for the heap and set its pointer
       to NULL when finished, by calling Heap_destroy().
    */
    Heap max_heap = *heap_ref;
    int32_t last_index = max_heap->last_index;
    char *the_array = max_heap->array;
    
    // reestablish the heap property, broken by having removed the root.
    // put the last element in root's place
    char temp;
    while (last_index){
        temp = the_array[0];
        the_array[0] = the_array[last_index];
        the_array[last_index] = temp;
        last_index--;
        Heap_sift_down(the_array, 0, last_index);
    }

    // this case isn't covered by Heap_sift_down().
    // It's more efficient to write it here, since Heap_sift_down() gets called once for
    // evey pass through the loop while the code here only gets evaluated once, when the
    // loop is exhausted
     // if (last_index < 2){
         // if (the_array[1] > the_array[0]){
             // temp = the_array[0];
             // the_array[0] = the_array[1];
             // the_array[1] = temp;
         // }
     // }

    Heap_destroy(heap_ref); 
}


/* ------------------------------- END PRIVATE ------------------------------------- */
/* ********************************************************************************* */




void Heap_sort(char the_array[], int32_t size){
    /* Wrapper function that calls Heap_max_heapify()
       to turn the_array into a max_heap, then
       completes the sorting by calling Heap_popS().
    */
    Heap max_heap = Heap_max_heapify_bu(the_array, size);
    Heap_popS(&max_heap);
}


