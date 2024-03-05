# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>
# include "./header_files/hashmap.h"
#include "./header_files/dbllist.h"
#include "./lru/lru.h"
#include "./clock/clock.h"


int main() {

    clock_cache* cache = clock_init(3);

    int pages[] = {10,15,25,10,30,50,20,35,30,20,50,70};

    int size = sizeof(pages) / sizeof(pages[0]);

    clock_put_array(cache, pages, size);

    clock_analysis(cache);

    dbllist* list = dbllist_create();
    dbllist_insert(list, 10);
    dbllist_print(list);

    return 0;
}