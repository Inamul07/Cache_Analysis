#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include "hashmap.h"
#include "myhashmap.h"

struct pair {
    int key;
    void* value;
};

// This method gets the key-value pair as parameter with 2 seed values.
// It generates and returns the hashcode for that key.
uint64_t hash_func(const void *item, uint64_t seed0, uint64_t seed1) {
    const struct pair *p = item;
    const int* key = &(p->key);
    return hashmap_sip(key, sizeof(p->key), seed0, seed1);
}

// This method compares the key.
// if Key_A is greater than Key_B it returns 1.
// If Key_A is lesser than Key_B it returns -1.
// If both the keys are equal it returns 0. 
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

// Initializes and returns an empty hashmap
hashmap* hmap_create() {
    return hashmap_new(sizeof(struct pair), 0, 0, 0, hash_func, key_compare, NULL, NULL);
}

// Inserts the key and value into the map
void hmap_insert(hashmap *map, int key, void* node) {
    hashmap_set(map, &(struct pair) { .key=key, .value=node });
}

// Returns the value for a given key found in the map.
// If the value is not found in the hashmap, Returns NULL.
void* hmap_get(hashmap* map, int key) {
    const struct pair *p;
    p = hashmap_get(map, &key);
    if(p == NULL) {
        return NULL;
    }
    return p->value;
}

// Removes the key and its corresponding value from the map.
// Return true, if the key and value are successfully removed.
// Returns false, if the key is not found in the map
bool hmap_remove(hashmap* map, int key) {
    const struct pair* p;
    p = hashmap_delete(map, &key);
    if(p == NULL) {
        return false;
    }
    return true;
}

// Checks if the map contains the given key
// Returns true, if the key is in the map
// Returns false, if the key is not found in the map
bool hmap_contains(hashmap* map, int key) {
    const struct pair *p;
    p = hashmap_get(map, &key);
    if(p == NULL) {
        return false;
    }
    return true;
}

// Free the memory allocated by the map
void hmap_free(hashmap* map) {
    hashmap_free(map);
}