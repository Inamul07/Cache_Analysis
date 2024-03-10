#ifndef MYHASHMAP_H
#define MYHASHMAP_H

#include "hashmap.h"

typedef struct hashmap hashmap;
struct pair;

struct hashmap* hmap_create();
void hmap_insert(struct hashmap*, int, void*);
void* hmap_get(struct hashmap*, int);
void* hmap_remove(struct hashmap*, int);
int hmap_contains(struct hashmap*, int);
void hmap_free(struct hashmap*);

#endif