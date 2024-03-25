/*
 * This Header File, "two_queue.h", contains the function definitions that are required to implement the "2Q Replacement algorithm".
 * The function implementations for these methods can be found in src/two_queue.c  

 * TWO_QUEUE OVERVIEW:
 * This algorithm consists of 3 lists. Am (LRU List), A1In (FIFO List), A1Out (FIFO List)
 * Only Am & A1In are considered as caches, while A1Out is a History Buffer.
 * If page is in Am, the page is most to the Most Recently Used (tail) position.
 * If the page is in A1In, we do nothing. This is to avoid pages, with correlated reference, into the Am List.
 * If the page is in A1Out (it is a miss), the page gets promoted to Am.
 * If a page is not in the cache, it is inserted to A1In.
 
 * CONSTRAINTS:
 * When Am List is set too small, frequent pages gets evicted out of the cache often.
 * When A1In is set too large, more frequent pages takes long time to get into Am List.
 * When A1In is set too small , it allows pages with correlated reference into cache and pollutes Am List.
 * When the A1Out Queue is too small, it rarely promotes pages to Am List.
 * 
 * NOTE: Correlated Reference refers to a page that is referenced continously for a period of time and then it is never accessed again.
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
double two_queue_get_hashmap_time(two_queue_cache* cache);

#endif