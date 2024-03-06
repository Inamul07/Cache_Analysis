#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "./clock/clock.h"

int main() {

    clock_cache* cache = clock_init(3);

    int pages[] = {10,15,25,10,30,50,20,35,30,20,50,70};
    int size = sizeof(pages) / sizeof(pages[0]);

    printf("\n");
    clock_put_array(cache, pages, size);
    clock_analysis(cache);
    clock_destroy(cache);
    printf("\n");

    return 0;
}