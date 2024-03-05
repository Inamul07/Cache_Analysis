#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "./lru/lru.h"

int main() {

    printf("\n");

    lru_cache* cache = lru_init(3);

    int pages[] = {7,0,1,2,0,3,0,4,2,3,0,3,2,1,2};
    int size = sizeof(pages) / sizeof(pages[0]);
    lru_put_array(cache, pages, size);
    lru_analysis(cache);
    lru_destroy(cache);

    printf("\n");

    return 0;
}