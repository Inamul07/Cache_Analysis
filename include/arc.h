/* 
 * This Header File, "arc.h", contains the function definitions that are required to implement the "Adaptive Replacement Cache Algorithm".
 * The function implementations for these methods can be found in src/arc.c  


 * ARC OVERVIEW:
 * This algorithm contains Four Buffers: T1, T2, B1, B2
 * The Buffers T1 & T2 are considered as cache. While, B1 & B2 (Ghost Caches) stores the history of (evicted) pages.
 * T1 & B1 Buffers are together known as L1 List and T2 & B2 Buffers are together known as L2 List.
 * The T1 Buffer favours Recency and the T2 Buffer favours Frequency.
 * ARC specifies a Tunable Parameter, p, which defines the size of Buffers T1 & T2.
 * The Tunable Parameter, p, is updated only when there is a hit in the Ghost Caches (B1 & B2 Buffers).
 * When there is a hit in B1, The p value is increased (increment by 1, if B1 >= B2, else increment by a factor of B2 (i.e) B2/B1)
 * Wnen there is a hit in B2, The p value is decreased (decrement by 1, if B2 >= B1, else decrement by a factor of B1 (i.e) B1/B2)
 * If the p value is greater, it allows more recent pages into the cache.
 * If the p value is smaller, it allows more frequent pages into the cache.
 
 * ABOUT THE BUFFERS:
 * When there is a large access of unrepeated pages, the size of the T1 buffer grows without affecting the pages in the T2 buffer.
 * Similarly, Where there is repeated reference of certain pages those pages reside in T2 buffer.
 * So, If the referenced pages are non repetitive, those pages are placed in the T1 buffer.
 * When an accessed page is already residing in T1 buffer then that page is promoted to T2 buffer.
 * If the refernced pages are repetive, Then those pages are placed in the T2 buffer.
 * That reference page resides in T2, until it is evicted out due to lack of reference for a long peroid of time.
 * Thus, The ARC algorithm adaptively changes the sizes of the buffers to achieve better cache hits.
*/

#ifndef ARC_H
#define ARC_H

typedef struct arc arc_cache;

arc_cache* arc_init(int capacity);
void arc_access(arc_cache* cache, int page);
void arc_analysis(arc_cache* cache);
void arc_put_array(arc_cache* cache, int pages[], int size);
void arc_print_buffer(arc_cache* cache);
void arc_destroy(arc_cache* cache);
double arc_get_hit_ratio(arc_cache* cache);
double arc_get_hashmap_time(arc_cache* cache);

#endif