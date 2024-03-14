// This header file, "dbllist.h", contains the function definitions that are required to implement a Doubly Linked List.
// The function implementations or these methods can be found in lib/dbllist/dbllist.c

#ifndef DBLLIST_H
#define DBLLIST_H

#include <stdbool.h>

typedef struct linked_list_node Node;
typedef struct dbllist_ dbllist;

Node* node_create(int data, Node* prev, Node* next);
int node_val(Node* node);
void node_destroy(Node* node);

dbllist* dbllist_create();
int dbllist_size(dbllist* list);
void dbllist_insert_at_tail(dbllist* list, int data);
void dbllist_insert_node_at_tail(dbllist* list, Node* node);
void dbllist_move_node_to_tail(dbllist* list, Node* node);
bool dbllist_remove_head(dbllist* list);
bool dbllist_remove_node(dbllist* list, Node* node);
int dbllist_peek_head_val(dbllist* list);
int dbllist_peek_tail_val(dbllist* list);
void dbllist_print(dbllist* list);
void dbllist_destroy(dbllist* list);

// WORKING:
/*
    Let the variable name of the dbllist be list.
    NOTE: The actual head and tail values are for internal purposes only, so the user cannot access it.
    NOTE: The NODE_DEFAULT is the default value of the node (Minimum Integer Value).

    dbllist_create()
    Creates [(NODE_DEFAULT) <-> (NODE_DEFAULT)] ---> This represents [head <-> tail]

    dbllist_insert_at_tail(list, 20);
    BEFORE: [(NODE_DEFAULT) <-> 10 <-> (NODE_DEFAULT)]        // For User: [10]
    AFTER: [(NODE_DEFAULT) <-> 10 <-> 20 <-> (NODE_DEFAULT)]  // For User: [10 <-> 20]

    dbllist_move_node_to_tail(list, Node[10])
    BEFORE: [(NODE_DEFAULT) <-> 10 <-> 20 <-> 30 <-> (NODE_DEFAULT)]  // For User: [10 <-> 20 <-> 30]
    AFTER: [(NODE_DEFAULT) <-> 20 <-> 30 <-> 10 <-> (NODE_DEFAULT)]   // For User: [20 <-> 30 <-> 10]

    dbllist_remove_head(list)
    BEFORE: [(NODE_DEFAULT) <-> 10 <-> 20 <-> 30 <-> (NODE_DEFAULT)]  // For User: [10 <-> 20 <-> 30]
    AFTER: [(NODE_DEFAULT) <-> 20 <-> 30 <-> (NODE_DEFAULT)]          // For User: [20 <-> 30]

    dbllist_remove_node(list, Node[20])
    BEFORE: [(NODE_DEFAULT) <-> 10 <-> 20 <-> 30 <-> (NODE_DEFAULT)]  // For User: [10 <-> 20 <-> 30] 
    AFTER: [(NODE_DEFAULT) <-> 10 <-> 30 <-> (NODE_DEFAULT)]   // For User: [10 <-> 30] 
*/

#endif