// This header file, "cache_factory.h", is similar to a factory method.
// The function implementations for these methods can be found in src/cache_factory.c

// Use this file to run any of the available cache replacement algorithms by passing the appropriate cache name in the cache_init() method.

#ifndef CACHE_FACTORY_H
#define CACHE_FACTORY_H

typedef struct cache_ generic_cache;

void* cache_init(char* cacheName, int capacity);
void cache_access(void* cache, int page);
void cache_analysis(void* cache);
void cache_put_array(void* cache, int pages[], int size);
void cache_print_buffer(void* cache);
void cache_destroy(void* cache);
double cache_get_hit_ratio(void* cache);


#endif