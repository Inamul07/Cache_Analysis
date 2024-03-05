#include <stdio.h>
#include <stdlib.h>
#include "../header_files/dbllist.h"

typedef struct node {
    int val;
    struct node* prev;
    struct node* next;
} Node;

Node* node_create(int data) {
    Node* node = (Node*) malloc(sizeof(node));
    node->val = data;
    return node;
}

int node_val(Node* node) {
    return node->val;
}

typedef struct dbllist_ {
    Node* head;
    Node* tail;
    int size;
} dbllist;

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
    return list->size;
}

void dbllist_insert(dbllist* list, int data) {
    Node* node = node_create(data);
    node->val = data;
    dbllist_insert_node(list, node);
}

void dbllist_insert_node(dbllist* list, Node* node) {
    Node* prev = list->tail->prev;
    prev->next = node;
    node->prev = prev;
    node->next = list->tail;
    list->tail->prev = node;
    list->size++;
}

Node* dbllist_remove_head(dbllist* list) {
    if(list->size == 0) {
        return NULL;
    }
    Node* head = list->head->next;
    Node* next = head->next;
    list->head->next = next;
    next->prev = list->head;
    list->size--;
    return head;
}

Node* dbllist_remove_node(dbllist* list, Node* node) {
    if(list->size == 0) {
        return NULL;
    }
    Node* prev = node->prev;
    Node* next = node->next;
    prev->next = next;
    next->prev = prev;
    list->size--;
    return node;
}

void dbllist_print(dbllist* list) {
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
    Node* head = list->head;
    while(head != NULL) {
        Node* next = head->next;
        free(head);
        head = next;
    }
    free(list);
}