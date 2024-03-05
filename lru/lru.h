#ifndef LRU_H_
#define LRU_H_

typedef struct lru_cache_ lru_cache;

lru_cache* lru_init(int);
void lru_access(lru_cache*, int);
void lru_analysis(lru_cache*);
void lru_put_array(lru_cache*, int*, int);
void lru_print_buffer(lru_cache*);
void lru_destroy(lru_cache*);

#endif