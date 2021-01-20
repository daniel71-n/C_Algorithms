#include "sorting.h"
#include <string.h>

/*  *********************** Private ************************ */

static void Swap_nodes_P(Node *node1, Node *node2){
    /* Swap the values in node1 and node2. 

       Note: 
       Swap_nodes does not actually swap the pointers, but only the values
       inside the objects (Node types) pointed to by node1 and node2. 
        ----------------------------
       This means that the list which node1 and node 2 are part of is actually 
       NOT modified as far as which pointer points to what, and the sorting 
       process overall is completely transparent from that point of view.
    */
    char temp = node1->data;
    node1->data = node2->data;
    node2->data = temp;
};


/*  *********************** End Private************************ */



void Sort_bubble(Head *head_ptr){
    /* ----------------- General overview --------------
       Sort a singly linked list in place, in ascending order. 
       Each node is compared with the one following it, and they're
       swapped if n > n+1.
       The swapping is carried out by the Swap_nodes() function.

       Since each node can move at most one position in the list
       for each iteration over the list, this means that the number
       of iterations increases with the length of the list and 
       the extent to which the nodes are out of order.

       Two loops are used: the first one iterates over the list once,
       and swaps nodes as it does so. The second loop (under 'label') checks 
       to see whether the sorting is complete. If it is, then Sort_bubble
       can return. If not, the flow of control is redirected by a jump
       back up to the first loop, for another pass over the loop.
        
       ---------------- Performance notes -------------------
       The bubble sort sorting algorithm is notoriously inefficient.
       As can be deduced from the points above and especially the ones 
       that follow, the worst-case performance of the algorithm rules
       it out as a viable sorting option for any non-trivial sorts.
       
       The main purpose of the bubble sort algorithm should in essence
       be mainly didactic.

       * * * * * * * * * * * * * * * * * * * * * * * * * * * 
       ------------------ Implementation notes --------------------
       A fundamental thing to keep in mind is that this Sort_bubble implementation
       does not actually move around the nodes (i.e. doesn't change the 'next' 
       pointer of the nodes) while sorting the list. What's changed is the data
       value stored in each node. 

       This is relevant for a number of reasons. It's an important consideration
       as regards space and time complexity.  

       Since we're dealing with singly linked lists, each node points to the next
       node down the list, but not to the previous one. This means that in order
       to put a node x after a node b, we need not so much a pointer to x,
       but one to the node before it, where x is also easily reachable from (i.e.
       you can reach x+1 from x, but can't reach x-1). 
       Swapping two nodes (pointers to nodes), we thus have to keep track 
       of the node preceding each of them. This adds a number of pointers
       to the function stack frame. Doubly linked lists would do away with part
       of this awkwardness and overhead, since each node has both a previous and next
       pointer, so it can be handled on its own, without having so many things to
       keep track of.

       The above also means the implementation of the swap_nodes() helper function, which
       Sort_bubble() relies on, is made more complicated if swap_nodes() swaps the actual nodes, 
       where it would otherwise be straightforward. More parameters would have to be fed to 
       it so that the pointers can actually be changed back in the original list. As mentioned
       above, one would have to know the n-1 pointer for every n pointer operated on.
       Also cases such as the one where n is the first node, and thus there's NO n-1,
       add further considerations and bases to cover. 

       Additionally, if the list were to be reversed in place (as opposed to creating 
       a copy and operating on that, while using the original one for reference), then 
       iterating through the loop is awkward if the list is modified (by swap_nodes) AS 
       the iteration is in process. For example, if the next node in the list is supposed to 
       be x, but x is swapped with the node before it, then somehow we need to keep track 
       of the node AFTER the next one, so that the iteration stays on track. 

       A considerable number of extra pointers would take up both more space
       as well as time, as more variables need to be manipulated. 

       Conversely, the sorting process of Sort_bubble() is completely transparent as
       far as how the nodes are organized in the list from a pointer perspective.
       It leaves the 'next' node pointers unchanged, and sorts instead by simply changing 
       the data fields inside the nodes. The performance is vastly improved by this and 
       the implementation is greatly simplified. 
    */
    Node *first_node = head_ptr->firstNode_ptr;
    Node *current_node;

    unsigned int elements = head_ptr->list_length-1;   // only n-1 swaps are required on a pass over a list of n items
    unsigned int left;  
one_pass:   // label
    for (current_node = first_node, left = elements; \
        left >0; \
        left --, current_node = current_node->next_ptr)
    {
        if (current_node->data > current_node->next_ptr->data){
            // if n > n+1 swap them
            // ascending order; the smallest value has to bubble up to the left
            Swap_nodes_P(current_node, current_node->next_ptr);
        };
    };  
    // since bubble sort is guaranteed to sort a list of n items in
    // (n-1)+(n-2)+(n-3)+..(n-n) comparisons and potential swap operations,
    // the second loop here (as in the original implementation -- see below)
    // can be done away it, as it's not needed. A simple variable that gets
    // decremented with each completed pass over the loop above is all that's
    // needed. As long as it's > 0, the flow of conrol is to be sent back up
    // for another pass
    if (--elements > 0){
        goto one_pass;
    };
    // when the flow of control makes it to here, the sorting is complete
};



/* -----------------Bubble sort - initial implementation ----------------- */
/* // This is the original Implementation. Above is the updated version.
   // This is comparatively a lot less efficient but I've kept it here for 
   // comparison and reference purposes.
   // There are various weaknesses in the implementation below, having to 
   // do with the two loops. The first loop always runs to the last node in
   // the list. Although swaps are done on an as-needed basis, there are still
   // n comparisons done every time for all n elements, when only n-1 comparisons
   // need to be done per pass, and n can be decremented with each pass (so n-1
   // comparisons and potential swaps on the frst pass, n-2 on the second, n-3 on
   // the third etc.)
   // the second loop, also, runs over the whole list after every single pass
   // completed in the first loop. As established though, it only needs to look
   // at n-1 elements, and decrement n with each pass.
   // ------------------------------------------------*
void Sort_bubble1(Head *head_ptr){
    Node *first_node = head_ptr->firstNode_ptr;

one_pass:   // label
    for (Node *current_node = first_node; \
        current_node->next_ptr != NULL; \
        current_node = current_node->next_ptr)
    {
        if (current_node->data > current_node->next_ptr->data){
            // if n > n+1 swap them
            // ascending order; the smallest value has to bubble up to the left
            Swap_nodes_P(current_node, current_node->next_ptr);
        };
    };  // end of loop one, where the sorting is actually done

    //loop 2: check if the list is completely sorted
    for (Node *current = first_node; current->next_ptr; current = current->next_ptr){
        if (current->data > current->next_ptr->data){
            // ascending order; the smallest value has to bubble up to the left
            goto one_pass;  // more sorting needs to be done, jump to one_pass for another round
        };
    };
    // when the flow of control makes it to here, the sorting is complete
};
*/



/*
   Having bubble sort operate on an array makes for a much clearer 
   implementation, as far as understanding the basic idea that bubble sort
   boils down to 
*/
/*
void Bubble_sort_array(char chararray[], unsigned int count){
    / * Sort an array in place using bubble sort.

       A bubble sort implementation operating on an array instead
       of a linked list, for comparison. 

       Given an array of n elements, the outer loop will have
       n-1 iterations. The inner loop, for its part, will execute its body 
       once for each item in the outer array. But with each advancement in
       the outer array, i gets decremented, so j executes its body one less
       time. 
       In other words, if the outer loop starts at index i, the inner loop
       will do i comparisons and (potentially) swaps.
       but at index i+1, the inner loop will do i-1 comparisons.
       At index i+2, the inner loop will do i-2 comparisons and so on.
    * /
    unsigned int elements = count-1; 
    for (unsigned i = elements; elements > 0; elements--){
        for(unsigned int j = 0; j < elements; j++){
            if(chararray[j] > chararray[j+1]){
                char temp = chararray[j];
                chararray[j] = chararray[j+1];
                chararray[j+1] = temp;
            };
        };
    };
};
*/


