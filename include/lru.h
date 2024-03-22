/*
 * This Header File, "lru.h", contains the function definitions that are required to implement the "Least Recently Used (LRU) algorithm".
 * The function implementations for these methods can be found in src/lru.c    
    
 * LRU OVERVIEW:
 * If the requested page is not available in the cache, the LRU algorithm adds the page at the Most Recently Used (tail) position in the cache.
 * If the requested page is available in the cache, then the page is moved to the Most Recently Used (tail) position in the cache.
 * When cache size is full, the Least Recently Used (head) page is removed (evicted) from the cache.
*/

#ifndef LRU_H_
#define LRU_H_

typedef struct lru_cache_ lru_cache;

lru_cache* lru_init(int capacity);
void lru_access(lru_cache* cache, int page);
void lru_analysis(lru_cache* cache);
void lru_put_array(lru_cache* cache, int pages[], int size);
void lru_print_buffer(lru_cache* cache);
void lru_destroy(lru_cache* cache);
double lru_get_hit_ratio(lru_cache* cache);

#endif