/*
 * This header file, "myhashmap.h", acts as a wrapper file for the "hashmap.h" file.
 * The function implementations for these methods can be found in lib/hashmap/myhashmap.c
 * This file contains simplified function names and parameter values, that call the actual functions defined in the "hashmap.h" file.
*/

#ifndef MYHASHMAP_H
#define MYHASHMAP_H

#include <stdbool.h>

#include "hashmap.h"

typedef struct hashmap hmap;
typedef struct myhashmap hashmap;
struct pair;

hashmap* hmap_create();
void hmap_insert(hashmap* map, int key, void* value);
void* hmap_get(hashmap* map, int key);
bool hmap_remove(hashmap* map, int key);
bool hmap_contains(hashmap* map, int key);
void hmap_free(hashmap* map);
double hmap_get_time_taken(hashmap* map);

#endif