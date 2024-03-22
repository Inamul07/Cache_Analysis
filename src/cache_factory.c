#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "cache_factory.h"
#include "lru.h"
#include "clock.h"
#include "two_queue.h"
#include "arc.h"

struct cache_ {
    void* cache;
    void* (*init)(int capacity);
    void (*access)(void* cache, int page);
    void (*analysis)(void* cache);
    void (*put_array)(void* cache, int pages[], int size);
    void (*print_buffer)(void* cache);
    void (*destroy)(void* cache);
    double (*get_hit_ratio)(void* cache);
};

/*
 * Creates and Initialises the cache, based on the given cache_name.
 * If capacity is less than 1. Returns NULL
 * SYNTAX: cache_init(LRU / CLOCK / TWO_QUEUE / ARC, <capacity>);
*/
generic_cache* cache_init(cacheType cacheName, int capacity) {
    generic_cache* cache = malloc(sizeof(generic_cache));
    switch(cacheName) {
        case LRU:
            cache->init = (void* (*)(int)) lru_init;
            cache->access = (void (*)(void*, int)) lru_access;
            cache->analysis = (void (*)(void*)) lru_analysis;
            cache->put_array = (void (*)(void*, int*, int)) lru_put_array;
            cache->print_buffer = (void (*)(void*)) lru_print_buffer;
            cache->destroy = (void (*)(void*)) lru_destroy;
            cache->get_hit_ratio = (double (*)(void*)) lru_get_hit_ratio;
            break;
        case CLOCK:
            cache->init = (void* (*)(int)) clock_init;
            cache->access = (void (*)(void*, int)) clock_access;
            cache->analysis = (void (*)(void*)) clock_analysis;
            cache->put_array = (void (*)(void*, int*, int)) clock_put_array;
            cache->print_buffer = (void (*)(void*)) clock_print_buffer;
            cache->destroy = (void (*)(void*)) clock_destroy;
            cache->get_hit_ratio = (double (*)(void*)) clock_get_hit_ratio;
            break;
        case TWO_QUEUE:
            cache->init = (void* (*)(int)) two_queue_init;
            cache->access = (void (*)(void*, int)) two_queue_access;
            cache->analysis = (void (*)(void*)) two_queue_analysis;
            cache->put_array = (void (*)(void*, int*, int)) two_queue_put_array;
            cache->print_buffer = (void (*)(void*)) two_queue_print_buffer;
            cache->destroy = (void (*)(void*)) two_queue_destroy;
            cache->get_hit_ratio = (double (*)(void*)) two_queue_get_hit_ratio;
            break;
        case ARC:
            cache->init = (void* (*)(int)) arc_init;
            cache->access = (void (*)(void*, int)) arc_access;
            cache->analysis = (void (*)(void*)) arc_analysis;
            cache->put_array = (void (*)(void*, int*, int)) arc_put_array;
            cache->print_buffer = (void (*)(void*)) arc_print_buffer;
            cache->destroy = (void (*)(void*)) arc_destroy;
            cache->get_hit_ratio = (double (*)(void*)) arc_get_hit_ratio;
            break;
        default:
            cache = NULL;
    }

    if(cache == NULL) {
        printf("Invalid Cache Type\n");
    } else {
        cache->cache = cache->init(capacity);
    }
    return cache;
}

// Performs the respective cache algorithm for the given page.
void cache_access(generic_cache* cache, int page) {
    if(cache == NULL) {
        printf("Cache cannot be NULL\n");
        return;
    }
    cache->access(cache->cache, page);
}

/*
 * Prints the Buffer, Total Reference Count, Hit Count and Miss Count of the cache at that current state.
 * Reference count must be atleast one before calling this method.
*/
void cache_analysis(generic_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be NULL\n");
        return;
    }
    cache->analysis(cache->cache);
}

// Performs the respective Cache operation for each element, from the array, in a linear fashion
void cache_put_array(generic_cache* cache, int pages[], int size) {
    if(cache == NULL) {
        printf("Cache cannot be NULL\n");
        return;
    }
    cache->put_array(cache->cache, pages, size);
}

// Prints the Cache buffer at that current state
void cache_print_buffer(generic_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be NULL\n");
        return;
    }
    cache->print_buffer(cache->cache);
}

// Frees the memory occupied by the cache
void cache_destroy(generic_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be NULL\n");
        return;
    }
    cache->destroy(cache->cache);
}

/*
 * Calculates and returns the hit ratio at that current state.
 * Returns -1, if the cache is NULL or if there were no references before
*/
double cache_get_hit_ratio(generic_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be NULL\n");
        return 0;
    }
    cache->get_hit_ratio(cache->cache);
}