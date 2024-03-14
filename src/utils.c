#include "utils.h"

// Removes the head node of the list and removes the node from the hashmap.
// Returns the value of the removed head node.
int util_peek_head_value_and_remove(dbllist* list, hashmap* map) {
    int headVal = dbllist_peek_head_val(list);
    dbllist_remove_head(list);
    hmap_remove(map, headVal);
    return headVal;
}

// Creates a node with the given data.
// Inserts the node at the end (tail) of the list.
// Inserts the node in the hashmap with its respective key (Value of that node).
void util_insert_node_at_tail_and_map(int data, dbllist* list, hashmap* map) {
    Node* node = node_create(data, NULL, NULL);
    dbllist_insert_node_at_tail(list, node);
    hmap_insert(map, data, node);
}

// Removes the node with the given data from the list.
// Removes the node, from the hashmap, associated with this data.
void util_remove_from_list_and_map(int data, dbllist* list, hashmap* map) {
    Node* node = hmap_get(map, data);
    hmap_remove(map, data);
    dbllist_remove_node(list, node);
}