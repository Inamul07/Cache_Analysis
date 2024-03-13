// CODE_REVIEW: explain what this library does breifly
#ifndef LRU_H_
#define LRU_H_

typedef struct lru_cache_ lru_cache;

lru_cache* lru_init(int capacity);
void lru_access(lru_cache* cache, int page);
void lru_analysis(lru_cache* cache);
void lru_put_array(lru_cache* cache, int* pages, int size);
void lru_print_buffer(lru_cache* cache);
void lru_destroy(lru_cache* cache);
double lru_get_hit_ratio(lru_cache* cache);

#endif