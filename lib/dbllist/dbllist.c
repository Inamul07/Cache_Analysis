#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>

#include "dbllist.h"

// CODE_REVIEW: improve comments

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

// Creates and Initializes a Node with prev and next pointers
Node* node_create(int data, Node* prev, Node* next) {
    Node* node = malloc(sizeof(Node));
    node->val = data;
    node->prev = prev;
    node->next = next;
    return node;
}

// Gets and returns the value of a given node
int node_val(Node* node) {
    if(node == NULL) {
        printf("Node is NULL\n");
        // CODE_REVIEW: Make a macro for node default value....
        return INT_MIN;
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

// Creates and Initializes a Doubly Linked List
dbllist* dbllist_create() {
    dbllist* list = (dbllist*) malloc(sizeof(dbllist));
    // CODE_REVIEW: use default node value....
    list->head = node_create(-1, NULL, NULL);
    list->tail = node_create(-1, NULL, NULL);
    list->head->next = list->tail;
    list->tail->prev = list->head;
    list->size = 0;
    return list;
}

/*
    // CODE_REVIEW: Mention your dbl logic with head and tail with default values
    List = [-1 <-> 10 <-> 20 <-> -1]
    Output = 2
*/
// Returns the size of the given DBL List
int dbllist_size(dbllist* list) {
    if(list == NULL) {
        printf("List is NULL\n");
        // CODE_REVIEW: Make a macro for node default value....
        return INT_MIN;
    }
    return list->size;
}

/*
    Before: [-1 <-> 10 <-> 20 <-> -1]
    dbllist_insert_at_tail(list, 30);
    After: [-1 <-> 10 <-> 20 <-> 30 <-> -1]
*/
// Creates and Inserts a Node with the given data to the tail of the DBL List
void dbllist_insert_at_tail(dbllist* list, int data) {
    if(list == NULL) {
        printf("List cannot be NULL\n");
        return;
    }
    Node* node = node_create(data, NULL, NULL);
    node->val = data;
    dbllist_insert_node_at_tail(list, node);
}

/*
    Before: [-1 <-> 10 <-> 20 <-> -1]
    dbllist_insert_node_at_tail(list, [30]);  ~ 30 is node 
    After: [-1 <-> 10 <-> 20 <-> 30 <-> -1]
*/
// Inserts the given node to the tail of the DBL List
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
    List = [-1 <-> 1 <-> 2 <-> 3 <-> -1]
    Output = 1
*/
// Gets the value of head Node
int dbllist_peek_head_val(dbllist* list) {
    if(list == NULL || list->size == 0) {
        printf(!list? "List cannot be NULL\n": "List is Empty\n");
        exit(EXIT_FAILURE);
    }
    return list->head->next->val;
}

/* 
    List = [-1 <-> 1 <-> 2 <-> 3 <-> -1]
    Output = 3
*/
// Gets the value of the tail node
int dbllist_peek_tail_val(dbllist* list) {
    if(list == NULL || list->size == 0) {
        printf(!list? "List cannot be NULL\n": "List is Empty\n");
        exit(EXIT_FAILURE);
    }
    return list->tail->prev->val;
}

/*
    Before: [-1 <-> 10 <-> 20 <-> 30 <-> -1]
    dbllist_remove_head(list);
    After: [-1 <-> 20 <-> 30 <-> -1]
*/
// Removes the head of the given DBL List and frees the memory allocated by it
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
    Before: [-1 <-> 10 <-> 20 <-> 30 <-> 40 <-> -1]
    dbllist_move_node_to_tail(list, [20]); ~ 20 is node
    After: [-1 <-> 10 <-> 30 <-> 40 <-> 20 <-> -1]
*/
// For a given node in a list, moves the node to the tail of the DBL List
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
    Before: [-1 <-> 10 <-> 20 <-> 30 <-> 40 <-> -1]
    dbllist_remove_node(list, [20]);  ~ 20 is node
    After: [-1 <-> 10 <-> 30 <-> 40 <-> -1]
*/
// For a given node in a list, Removes the node from the list and frees the memory allocated by it
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

// Prints the list at that current state
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