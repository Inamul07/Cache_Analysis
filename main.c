#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "include/lru.h"
#include "include/clock.h"

int main() {

    int size;

    printf("\n");

    lru_cache* cache = lru_init(3);

    int pages[] = {7,0,1,2,0,3,0,4,2,3,0,3,2,1,2};
    size = sizeof(pages) / sizeof(pages[0]);
    lru_put_array(cache, pages, size);

    lru_analysis(cache);
    lru_destroy(cache);

    printf("\n");

    clock_cache* cache2 = clock_init(3);

    int pages2[] = {10,15,25,10,30,50,20,35,30,20,50,70};
    size = sizeof(pages2) / sizeof(pages2[0]);

    printf("\n");
    clock_put_array(cache2, pages2, size);
    clock_analysis(cache2);
    clock_destroy(cache2);
    printf("\n");

    return 0;
}