#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "dbllist.h"

// CODE_REVIEW: move these examples to top of relevant functions
/*
    dbllist_create()
    -1 <-> -1

    dbblist_insert_at_tail(10)
    -1 <-> 10 <-> -1

    dbblist_insert_at_tail(20)
    -1 <-> 10 <-> 20 <-> -1

    dbllist_peek_head() -> returns 10

    dbllist_peek_tail() -> returns 20

    dbllist_remove_head()
    -1 <-> 20 <-> -1

    dbllist_insert(30)
    -1 <-> 20 <-> 30 <-> -1

    dbllist_move_node_to_tail( [20] ) // 20 is a node
    -1 <-> 30 <-> 20 <-> -1

*/

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

Node* node_create(int data) {
    Node* node = malloc(sizeof(Node));
    node->val = data;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

// CODE_REVIEW: Update name
Node* node_create_with_ptrs(int data, Node* prev, Node* next) {
    Node* node = node_create(data);
    node->prev = prev;
    node->next = next;
    return node;
}

int node_val(Node* node) {
    if(node == NULL) {
        printf("Node is NULL\n");
        // CODE_REVIEW: don't exit
        exit(EXIT_FAILURE);
    }
    return node->val;
}

void node_destroy(Node* node) {
    if(node == NULL) {
        printf("Node cannot be null\n");
        return;
    }
    free(node);
}

dbllist* dbllist_create() {
    dbllist* list = (dbllist*) malloc(sizeof(dbllist));
    list->head = node_create(-1);
    list->tail = node_create(-1);
    list->head->next = list->tail;
    list->tail->prev = list->head;
    list->size = 0;
    return list;
}

int dbllist_size(dbllist* list) {
    if(list == NULL) {
        printf("List is NULL\n");
        exit(EXIT_FAILURE);
    }
    return list->size;
}

void dbllist_insert_at_tail(dbllist* list, int data) {
    if(list == NULL) {
        printf("List cannot be NULL\n");
        return;
    }
    Node* node = node_create(data);
    node->val = data;
    dbllist_insert_node_at_tail(list, node);
}

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

// CODE_REVIEW: change name
int dbllist_peek_head_val(dbllist* list) {
    if(list == NULL || list->size == 0) {
        printf(!list? "List cannot be NULL\n": "List is Empty\n");
        exit(EXIT_FAILURE);
    }
    return list->head->next->val;
}

// CODE_REVIEW: change name
int dbllist_peek_tail_val(dbllist* list) {
    if(list == NULL || list->size == 0) {
        printf(!list? "List cannot be NULL\n": "List is Empty\n");
        exit(EXIT_FAILURE);
    }
    return list->tail->prev->val;
}


// CODE_REVIEW: introduce booleans
int dbllist_remove_head(dbllist* list) {
    if(list == NULL || list->size == 0) {
        printf(!list? "List cannot be NULL\n": "List is Empty\n");
        return 0;
    }
    Node* head = list->head->next;
    Node* next = head->next;
    list->head->next = next;
    next->prev = list->head;
    list->size--;
    free(head);
    return 1;
}

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

int dbllist_remove_node(dbllist* list, Node* node) {
    if(list == NULL || node == NULL) {
        printf("%s cannot be NULL\n", !list && !node? "List & Node": (!list? "List": "Node"));
        return 0;
    }
    Node* prev = node->prev;
    Node* next = node->next;
    prev->next = next;
    next->prev = prev;
    list->size--;
    return 1;
}

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