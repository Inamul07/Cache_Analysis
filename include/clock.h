// Code Review: Explain more about clock hand usage

// This Header File, "clock.h", contains the function definitions that are required to implement the "Clock Replacement algorithm".
// The function implementations for these methods can be found in src/clock.c 

// CLOCK OVERVIEW:
// This algorithm contains a clock hand (currIdx) that points to a page in the cache.
// Each page in cache has an additional reference bit.
// If the refernced page is in cache, its reference bit is set to one.
// If the page is not in the cache, the clock hand finds a page with a refernce bit of 0. 
// Then the page is replaced with the referenced page and the reference bit of that pages is set to 1.

#ifndef CLOCK_H_
#define CLOCK_H_

typedef struct clock_node_ clock_node;
typedef struct clock_cache_ clock_cache;

clock_cache* clock_init(int capacity);
void clock_access(clock_cache* cache, int page);
void clock_analysis(clock_cache* cache);
void clock_print_buffer(clock_cache* cache);
void clock_put_array(clock_cache* cache, int pages[], int size);
void clock_destroy(clock_cache* cache);
double clock_get_hit_ratio(clock_cache* cache);

#endif