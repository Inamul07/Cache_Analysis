# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>
# include "./hashmap/hashmap.h"
#include "./dbllist/dbllist.h"
#include "./dbllist/dbllist.h"


int main() {

    struct hashmap* map = hmap_create();
    hmap_insert(map, 10, 10);

    printf("%d", hmap_get(map, 10));

    return 0;
}