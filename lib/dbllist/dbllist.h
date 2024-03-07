#ifndef DBLLIST_H
#define DBLLIST_H

typedef struct node Node;
typedef struct dbllist_ dbllist;

Node* node_create(int);
int node_val(Node*);

dbllist* dbllist_create();
int dbllist_size(dbllist*);
void dbllist_insert(dbllist*, int data);
void dbllist_insert_node(dbllist*, Node* node);
Node* dbllist_remove_head(dbllist*);
Node* dbllist_remove_node(dbllist*, Node*);
void dbllist_print(dbllist*);
void dbllist_destroy(dbllist*);

#endif