/*  
    This Header File, "arc.h", contains the function definitions that are required to implement the "Adaptive Replacement Cache Algorithm".
    The function implementations for these methods can be found in src/arc.c  


    ARC OVERVIEW:
    This algorithm contains Four Lists: T1, T2, B1, B2
    The lists T1 & T2 are considered as cache. While, B1 & B2 (Ghost Caches) stores the history of (evicted) pages.
    T1 & B1 Lists are together known as L1 and T2 & B2 Lists are together known as L2.
    Refer the ARC algorithm from its research paper. (Given in the "References" section in README).
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