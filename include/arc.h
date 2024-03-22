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


#endif