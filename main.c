#include <stdio.h>

#include "cache_factory.h"

int main() {

    int pages[] = {7,0,1,2,0,3,0,4,2,3,0,3,2,1,2};
    int size = sizeof(pages) / sizeof(pages[0]);

    printf("\nLRU:\n");
    generic_cache* cache = cache_init("lru", 3);
    cache_put_array(cache, pages, size);
    cache_analysis(cache);
    cache_destroy(cache);
    printf("\n");

    printf("\nCLOCK:\n");
    generic_cache* cache2 = cache_init("clock", 3);
    cache_put_array(cache2, pages, size);
    cache_analysis(cache2);
    cache_destroy(cache2);
    printf("\n");

    generic_cache* cache3 = cache_init("two_queue", 5);
    printf("\nTWO QUEUE:\n");
    cache_put_array(cache3, pages, size);
    cache_analysis(cache3);
    cache_destroy(cache3);
    printf("\n");

    generic_cache* cache4 = cache_init("arc", 3);
    printf("ARC:\n");
    cache_put_array(cache4, pages, size);
    cache_analysis(cache4);
    cache_destroy(cache4);
    printf("\n");

    return 0;
}