/*
    This header file, "cache_factory.h", is similar to a factory method.
    The function implementations for these methods can be found in src/cache_factory.c

    Use this file to run any of the available cache replacemnt algorithms by passing the appropriate cache name in the cache_init() method.

    Implemented Cache Replacement Policies:
        - Least Recently Used (LRU)
        - Clock Page Replacement (CLOCK)
        - 2Q Page Replacement (TWO_QUEUE)
        - Adaptive Replacement Cache (ARC)
*/

#ifndef CACHE_FACTORY_H
#define CACHE_FACTORY_H

typedef struct cache_ generic_cache;

typedef enum cacheType {LRU, CLOCK, TWO_QUEUE, ARC} cacheType;

generic_cache* cache_init(enum cacheType cacheName, int capacity);
void cache_access(generic_cache* cache, int page);
void cache_analysis(generic_cache* cache);
void cache_put_array(generic_cache* cache, int pages[], int size);
void cache_print_buffer(generic_cache* cache);
void cache_destroy(generic_cache* cache);
double cache_get_hit_ratio(generic_cache* cache);


#endif