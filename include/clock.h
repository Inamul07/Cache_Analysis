// CODE_REVIEW: explain what this library does breifly
#ifndef CLOCK_H_
#define CLOCK_H_

typedef struct clock_node_ clock_node;
typedef struct clock_cache_ clock_cache;

clock_cache* clock_init(int capacity);
void clock_access(clock_cache* cache, int page);
void clock_analysis(clock_cache* cache);
void clock_print_buffer(clock_cache* cache);
void clock_put_array(clock_cache* cache, int* pages, int size);
void clock_destroy(clock_cache* cache);

#endif