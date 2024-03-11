#ifndef MYHASHMAP_H
#define MYHASHMAP_H

#include <stdbool.h>

#include "hashmap.h"

typedef struct hashmap hashmap;
struct pair;

hashmap* hmap_create();
void hmap_insert(hashmap* map, int key, void* value);
void* hmap_get(hashmap* map, int key);
void* hmap_remove(hashmap* map, int key);
bool hmap_contains(hashmap* map, int key);
void hmap_free(hashmap* map);

#endif