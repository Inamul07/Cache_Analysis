#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>

#include "dbllist.h"

#define NODE_DEFAULT INT_MIN

struct linked_list_node {
    int val;
    Node* prev;
    Node* next;
};

struct dbllist_ {
    Node* head;
    Node* tail;
    int size;
};

// Creates and Initializes a Node with previous and next pointers
Node* node_create(int data, Node* prev, Node* next) {
    Node* node = malloc(sizeof(Node));
    node->val = data;
    node->prev = prev;
    node->next = next;
    return node;
}

/*
 * Gets and returns the value of a given node.
 * If the passed node is NULL, it returns "default node value" (Minimum Integer Value).
*/
int node_val(Node* node) {
    if(node == NULL) {
        printf("Node is NULL\n");
        return NODE_DEFAULT;
    }
    return node->val;
}

// frees the memory alllocated by the node
void node_destroy(Node* node) {
    if(node == NULL) {
        printf("Node cannot be null\n");
        return;
    }
    free(node);
}

/*
 * Creates and Initializes a Doubly Linked List
 * This method creates a list with 2 nodes, head and tail.
 * These nodes are used for internal purposes.
 * This nodes are initially set to NODE_DEFAULT value (Minimum Integer Value).
 * Returned List: [(NODE_DEFAULT) <-> (NODE_DEFAULT)] // [head <-> tail]
*/
dbllist* dbllist_create() {
    dbllist* list = (dbllist*) malloc(sizeof(dbllist));
    list->head = node_create(NODE_DEFAULT, NULL, NULL);
    list->tail = node_create(NODE_DEFAULT, NULL, NULL);
    list->head->next = list->tail;
    list->tail->prev = list->head;
    list->size = 0;
    return list;
}

/*
    List = [(NODE_DEFAULT) <-> 10 <-> 20 <-> (NODE_DEFAULT)]
    Output = 2

 * NOTE: Eventhough the total size is 4, since the head and tail are used only for internal purposes, the size of list is 2. 

 * Returns the size of the given DBL List.
 * If the list is NULL. it returns -1
*/
int dbllist_size(dbllist* list) {
    if(list == NULL) {
        printf("List is NULL\n");
        return -1;
    }
    return list->size;
}

/*
    Before: [(NODE_DEFAULT) <-> 10 <-> 20 <-> (NODE_DEFAULT)]
    dbllist_insert_at_tail(list, 30);
    After: [(NODE_DEFAULT) <-> 10 <-> 20 <-> 30 <-> (NODE_DEFAULT)]

 * Creates and Inserts a Node with the given data to the tail of the DBL List
 * NOTE: In User Terms, At tail actually means before tail (here). So the node is inserted between the tail and the node before the tail.
 * Operation fails when list is NULL
*/
void dbllist_insert_at_tail(dbllist* list, int data) {
    if(list == NULL) {
        printf("List cannot be NULL\n");
        return;
    }
    Node* node = node_create(data, NULL, NULL);
    dbllist_insert_node_at_tail(list, node);
}

/*
    Before: [(NODE_DEFAULT) <-> 10 <-> 20 <-> (NODE_DEFAULT)]
    dbllist_insert_node_at_tail(list, [30]);  ~ 30 is node 
    After: [(NODE_DEFAULT) <-> 10 <-> 20 <-> 30 <-> (NODE_DEFAULT)]

 * Inserts the given node to the tail of the DBL List
 * NOTE: In User Terms, At tail actually means before tail (here). So the node is inserted between the tail and the node before the tail.
 * Operation fails when list or node is NULL.
*/
void dbllist_insert_node_at_tail(dbllist* list, Node* node) {
    if(list == NULL) {
        printf("%s cannot be NULL\n", !list && !node? "List & Node": (!list? "List": "Node"));
        return;
    }
    Node* prev = list->tail->prev;
    prev->next = node;
    node->prev = prev;
    node->next = list->tail;
    list->tail->prev = node;
    list->size++;
}

/* 
    List = [(NODE_DEFAULT) <-> 1 <-> 2 <-> 3 <-> (NODE_DEFAULT)]
    Output = 1

 * NOTE: The First value after NODE_DEFAULT is head (For User) so 1 is returned.

 * Returns the value of head Node
 * NOTE: The head node mentioned here, means the node after the actual head.
 * If the given list is null or if the list is empty it returns NODE_DEFAULT value (Minimum Integer Value)
*/
int dbllist_peek_head_val(dbllist* list) {
    if(list == NULL || list->size == 0) {
        printf(!list? "List cannot be NULL\n": "List is Empty\n");
        return NODE_DEFAULT;
    }
    return list->head->next->val;
}

/* 
    List = [(NODE_DEFAULT) <-> 1 <-> 2 <-> 3 <-> (NODE_DEFAULT)]
    Output = 3

 * NOTE: The Last value before NODE_DEFAULT is tail (For User) so 3 is returned.

 * Returns the value of the tail node
 * NOTE: The tail node mentioned here, means the node before the actual tail.
 * If the given list is null or if the list is empty it returns NODE_DEFAULT value (Minimum Integer Value)
*/
int dbllist_peek_tail_val(dbllist* list) {
    if(list == NULL || list->size == 0) {
        printf(!list? "List cannot be NULL\n": "List is Empty\n");
        return NODE_DEFAULT;
    }
    return list->tail->prev->val;
}

/*
    Before: [(NODE_DEFAULT) <-> 10 <-> 20 <-> 30 <-> (NODE_DEFAULT)]
    dbllist_remove_head(list);
    After: [(NODE_DEFAULT) <-> 20 <-> 30 <-> (NODE_DEFAULT)]

 * NOTE: The First value after NODE_DEFAULT is head (For User) so Node[10] is removed.

 * Removes the head of the given DBL List and frees the memory allocated by it
 * NOTE: The head node mentioned here, means the node after the actual head.
 * If the given list is null or if the list is empty it returns false (i.e) 0.
*/
bool dbllist_remove_head(dbllist* list) {
    if(list == NULL || list->size == 0) {
        printf(!list? "List cannot be NULL\n": "List is Empty\n");
        return false;
    }
    Node* head = list->head->next;
    Node* next = head->next;
    list->head->next = next;
    next->prev = list->head;
    list->size--;
    free(head);
    return true;
}

/*
    Before: [(NODE_DEFAULT) <-> 10 <-> 20 <-> 30 <-> 40 <-> (NODE_DEFAULT)]
    dbllist_move_node_to_tail(list, [20]); ~ 20 is node
    After: [(NODE_DEFAULT) <-> 10 <-> 30 <-> 40 <-> 20 <-> (NODE_DEFAULT)]

 * For a given node in a list, moves the node to the tail of the DBL List
 * NOTE: In User Terms, tail means the node before the actual tail
*/
void dbllist_move_node_to_tail(dbllist* list, Node* node) {
    if(list == NULL || node == NULL) {
        printf("%s cannot be NULL\n", !list && !node? "List & Node": (!list? "List": "Node"));
        return;
    }
    Node* prev = node->prev;
    Node* next = node->next;
    prev->next = next;
    next->prev = prev;
    list->size--;
    dbllist_insert_node_at_tail(list, node);
}

/*
    Before: [(NODE_DEFAULT) <-> 10 <-> 20 <-> 30 <-> 40 <-> (NODE_DEFAULT)]
    dbllist_remove_node(list, [20]);  ~ 20 is node
    After: [(NODE_DEFAULT) <-> 10 <-> 30 <-> 40 <-> (NODE_DEFAULT)]

 * For a given node in a list, Removes the node from the list and frees the memory allocated by it
 * This method removes the node from anywhere on the list.
*/
bool dbllist_remove_node(dbllist* list, Node* node) {
    if(list == NULL || node == NULL) {
        printf("%s cannot be NULL\n", !list && !node? "List & Node": (!list? "List": "Node"));
        return false;
    }
    Node* prev = node->prev;
    Node* next = node->next;
    prev->next = next;
    next->prev = prev;
    list->size--;
    free(node);
    return true;
}

/*
    List: [(NODE_DEFAULT) <-> 10 <-> 20 <-> 30 <-> (NODE_DEFAULT)]
    Output: [10, 20, 30]

 * Prints the list at that current state
 * NOTE: This doesn't print the actual head and tail values (i.e) the nodes with NODE_DEFAULT values at start and end of the list.
*/
void dbllist_print(dbllist* list) {
    if(list == NULL) {
        printf("List cannot be NULL\n");
        return;
    }
    Node* trav = list->head;
    printf("[");
    while(trav->next != list->tail) {
        printf("%d", trav->next->val);
        trav = trav->next;
        if(trav->next != list->tail) {
            printf(",");
        }
    }
    printf("]\n");
}

// Frees the memory allocated by the list
void dbllist_destroy(dbllist* list) {
    if(list == NULL) {
        printf("List cannot be NULL\n");
        return;
    }
    Node* head = list->head;
    while(head != NULL) {
        Node* next = head->next;
        free(head);
        head = next;
    }
    free(list);
}