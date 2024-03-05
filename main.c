# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>
# include "./header_files/hashmap.h"
#include "./header_files/dbllist.h"
#include "./lru/lru.h"
#include "./clock/clock.h"


int main() {

    lru_cache* cache = lru_init(3);

    int pages[] = {7,0,1,2,0,3,0,4,2,3,0,3,2,1,2};
    int size = sizeof(pages) / sizeof(pages[0]);

    lru_put_array(cache, pages, size);

    lru_analysis(cache);

    dbllist* list = dbllist_create();
    dbllist_insert(list, 10);
    dbllist_print(list);

    return 0;
}