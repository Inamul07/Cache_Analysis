#ifndef CLOCK_H_
#define CLOCK_H_

typedef struct clock_node_ clock_node;
typedef struct clock_cache_ clock_cache;

clock_cache* clock_init(int);
void clock_access(clock_cache*, int);
void clock_analysis(clock_cache*);
void clock_print_buffer(clock_cache*);
void clock_put_array(clock_cache*, int*, int);
void clock_destroy(clock_cache*);

#endif