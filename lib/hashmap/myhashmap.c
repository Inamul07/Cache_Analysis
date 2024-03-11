// CODE_REVIEW: write comments
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "hashmap.h"
#include "myhashmap.h"

struct pair {
    int key;
    void* value;
};

uint64_t hash_func(const void *item, uint64_t seed0, uint64_t seed1) {
    const struct pair *p = item;
    const int* key = &(p->key);
    return hashmap_sip(key, sizeof(p->key), seed0, seed1);
}

int key_compare(const void *a, const void *b, void *udata) {
    const struct pair *ua = a;
    const struct pair *ub = b;
    if(ua->key > ub->key) {
        return 1;
    } else if(ua->key < ub->key) {
        return -1;
    }
    return 0;
}

// Initializes the hashmap
struct hashmap* hmap_create() {
    return hashmap_new(sizeof(struct pair), 0, 0, 0, hash_func, key_compare, NULL, NULL);
}

// Inserts the key and value into the map
void hmap_insert(hashmap *map, int key, void* node) {
    hashmap_set(map, &(struct pair) { .key=key, .value=node });
}

// Returns the value for a given key found in the map
void* hmap_get(hashmap* map, int key) {
    const struct pair *p;
    p = hashmap_get(map, &key);
    if(p == NULL) {
        return NULL;
    }
    return p->value;
}

// Removes the key and its corresponding value from the map
void* hmap_remove(hashmap* map, int key) {
    const struct pair* p;
    // CODE_REVIEW: this memory will be reused by the hashmap library so make sure it doesn't cause any issue.
    p = hashmap_delete(map, &key);
    if(p == NULL) {
        return NULL;
    }
    return p->value;
}

// Checks if the map contains the given key
bool hmap_contains(hashmap* map, int key) {
    const struct pair *p;
    p = hashmap_get(map, &key);
    if(p == NULL) {
        return false;
    }
    return true;
}

void hmap_free(hashmap* map) {
    hashmap_free(map);
}