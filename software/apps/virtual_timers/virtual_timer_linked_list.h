#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "nrf.h"

#include "virtual_timer.h"

// -- List types

// a node within a linked list
typedef struct node_t {

    // *** Additional timer fields ***

    // add your own variables here

    // *** Do not edit below this line ***

    // timer value in microseconds. Used to sort the list. Must be initialized
    //  when the node is created
    uint32_t timer_value;

    // pointer to next node in list. Do not change this field for a node or you
    //  will break the list
    struct node_t* next;
} node_t;


// -- List functions

// Insert node at the correct place in the linked list based on increasing
//  node->timer_value. The node_t must be malloc'd before being added.
void list_insert_sorted(node_t* node);


// Return the first node in the linked list without removing it. This value may
//  be NULL if the list is empty.
node_t* list_get_first();


// Remove the first node from the linked list and return it. This value may be
//  NULL if the list is empty.
node_t* list_remove_first();


// Remove the specified node from the linked list. Note that the memory for the
//  node is NOT automatically freed.
void list_remove(node_t* node);


// Print the linked list for debugging.
void list_print();

