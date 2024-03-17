/*
    This header file, "utils.h", contains helper functions that are used in the Cache Replacement Algorithms.
    The function implementations for these methods can be found in src/utils.c  
*/

#ifndef UTILS_H
#define UTILS_H

#include "dbllist/dbllist.h"
#include "hashmap/myhashmap.h"

int util_peek_head_value_and_remove(dbllist* list, hashmap* map);
void util_insert_node_at_tail_and_map(int data, dbllist* list, hashmap* map);
void util_remove_from_list_and_map(int data, dbllist* list, hashmap* map);

#endif