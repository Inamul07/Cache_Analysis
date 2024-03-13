#ifndef ARC_H
#define ARC_H

typedef struct arc arc_cache;

arc_cache* arc_init(int capacity);
void arc_access(arc_cache* cache, int page);
void arc_analysis(arc_cache* cache);
void arc_put_array(arc_cache* cache, int* pages, int size);
void arc_print_buffer(arc_cache* cache);
void arc_destroy(arc_cache* cache);
double arc_get_hit_ratio(arc_cache* cache);


#endif