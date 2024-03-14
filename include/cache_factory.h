#ifndef CACHE_FACTORY_H
#define CACHE_FACTORY_H

typedef struct cache_ generic_cache;

void* cache_init(char* cacheType, int capacity);
void cache_access(void* cache, int page);
void cache_analysis(void* cache);
void cache_put_array(void* cache, int pages[], int size);
void cache_print_buffer(void* cache);
void cache_destroy(void* cache);
double cache_get_hit_ratio(void* cache);


#endif