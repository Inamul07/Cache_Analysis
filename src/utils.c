#include "utils.h"

int util_peek_head_value_and_remove(dbllist* list, hashmap* map) {
    int headVal = dbllist_peek_head_val(list);
    dbllist_remove_head(list);
    hmap_remove(map, headVal);
    return headVal;
}

void util_insert_node_at_tail_and_map(int data, dbllist* list, hashmap* map) {
    Node* node = node_create(data, NULL, NULL);
    dbllist_insert_node_at_tail(list, node);
    hmap_insert(map, data, node);
}

void util_remove_from_list_and_map(int data, dbllist* list, hashmap* map) {
    Node* node = hmap_get(map, data);
    dbllist_remove_node(list, node);
    hmap_remove(map, data);
}