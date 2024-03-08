#ifndef DBLLIST_H
#define DBLLIST_H

typedef struct linked_list_node Node;
typedef struct dbllist_ dbllist;

// CODE_REVIEW: add parameter names in both .c and .h files
Node* node_create(int);
int node_val(Node*);
Node* node_create_with_ptrs(int, Node*, Node*);
void node_destroy(Node*);

dbllist* dbllist_create();
int dbllist_size(dbllist*);
void dbllist_insert_at_tail(dbllist*, int);
void dbllist_insert_node_at_tail(dbllist*, Node*);
void dbllist_move_node_to_tail(dbllist*, Node*);
int dbllist_remove_head(dbllist*);
int dbllist_remove_node(dbllist*, Node*);
int dbllist_peek_head(dbllist*);
int dbllist_peek_tail(dbllist*);
void dbllist_print(dbllist*);
void dbllist_destroy(dbllist*);

#endif