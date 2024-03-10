#ifndef TWO_QUEUE_H
#define TWO_QUEUE_H

typedef struct two_queue two_queue_cache;

two_queue_cache* two_queue_init(int amSize, int a1inSize, int a1outSize);
void two_queue_access(two_queue_cache* cache, int page);
void two_queue_analysis(two_queue_cache* cache);
void two_queue_put_array(two_queue_cache* cache, int* pages, int size);
void two_queue_print_buffer(two_queue_cache* cache);
void two_queue_destroy(two_queue_cache* cache);


#endif