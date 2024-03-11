#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lru.h"
#include "clock.h"
#include "two_queue.h"
#include "arc.h"

int main() {

    int pages[] = {7,0,1,2,0,3,0,4,2,3,0,3,2,1,2};
    int size = sizeof(pages) / sizeof(pages[0]);

    printf("\nLRU:\n");
    lru_cache* cache = lru_init(3);
    lru_put_array(cache, pages, size);
    lru_analysis(cache);
    lru_destroy(cache);
    printf("\n");

    printf("\nCLOCK:\n");
    clock_cache* cache2 = clock_init(3);
    clock_put_array(cache2, pages, size);
    clock_analysis(cache2);
    clock_destroy(cache2);
    printf("\n");

    two_queue_cache* cache3 = two_queue_init(3, 2, 3);
    printf("\nTWO QUEUE:\n");
    two_queue_put_array(cache3, pages, size);
    two_queue_analysis(cache3);
    two_queue_destroy(cache3);
    printf("\n");

    arc_cache* cache4 = arc_init(3);
    printf("ARC:\n");
    arc_put_array(cache4, pages, size);
    arc_analysis(cache4);
    arc_destroy(cache4);
    printf("\n");

    return 0;
}