/*
    This Header File, "two_queue.h", contains the function definitions that are required to implement the "2Q Replacement algorithm".
    The function implementations for these methods can be found in src/two_queue.c  

    TWO_QUEUE OVERVIEW:
    This algorithm consists of 3 lists. Am (LRU List), A1In (FIFO List), A1Out (FIFO List)
    Only Am & A1In are considered as caches, while A1Out is a History Buffer.
    If page is in Am, the page is most to the Most Recently Used (tail) position.
    If the page is in A1Out (it is a miss), the page gets promoted to Am.
    If a page is not in the cache, it is inserted to A1In.
*/

#ifndef TWO_QUEUE_H
#define TWO_QUEUE_H

typedef struct two_queue two_queue_cache;

two_queue_cache* two_queue_init(int capacity);
void two_queue_access(two_queue_cache* cache, int page);
void two_queue_analysis(two_queue_cache* cache);
void two_queue_put_array(two_queue_cache* cache, int pages[], int size);
void two_queue_print_buffer(two_queue_cache* cache);
void two_queue_destroy(two_queue_cache* cache);
double two_queue_get_hit_ratio(two_queue_cache* cache);

#endif