// CODE_REVIEW: explain what this library does breifly
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

#endif