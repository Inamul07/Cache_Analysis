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

uint64_t hash_func(const void *item, uint64_t seed0, uint64_t seed1) {
    const struct pair *p = item;
    // CODE_REVIEW: don't allocate memory here
    int* key = (int*) malloc(sizeof(int));
    key[0] = p->key;
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

struct hashmap* hmap_create() {
    return hashmap_new(sizeof(struct pair), 0, 0, 0, hash_func, key_compare, NULL, NULL);
}

void hmap_insert(hashmap *map, int key, void* node) {
    hashmap_set(map, &(struct pair) { .key=key, .value=node });
}

void* hmap_get(hashmap* map, int key) {
    // CODE_REVIEW: no need of pair struct
    const struct pair *p;
    p = hashmap_get(map, &(struct pair){ .key=key });
    if(p == NULL) {
        return NULL;
    }
    return p->value;
}

void* hmap_remove(hashmap* map, int key) {
    // CODE_REVIEW: no need of pair struct
    const struct pair* p;
    p = hashmap_delete(map, &(struct pair) {.key=key});
    if(p == NULL) {
        return NULL;
    }
    return p->value;
}

int hmap_contains(hashmap* map, int key) {
    // CODE_REVIEW: no need of pair struct
    const struct pair *p;
    p = hashmap_get(map, &(struct pair){ .key=key });
    if(p == NULL) {
        return 0;
    }
    return 1;
}

void hmap_free(hashmap* map) {
    hashmap_free(map);
}