#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "./header_files/hashmap.h"
#include "./header_files/dbllist.h"
#include "./lru/lru.h"

int main() {

    printf("\n");

    lru_cache* cache = lru_init(3);

    int seq[] = {10,15,25,10,30,50,20,35,30,20,50,70};
    int size = sizeof(seq) / sizeof(seq[0]);
    lru_put_array(cache, seq, size);
    lru_analysis(cache);

    printf("\n");

    lru_cache* cache2 = lru_init(3);

    int loop[] = {1,5,7,10,12,1,5,7,3,8,1,5,7};
    size = sizeof(loop) / sizeof(loop[0]);
    lru_put_array(cache2, loop, size);
    lru_analysis(cache2);

    printf("\n");

    lru_cache* cache3 = lru_init(3);

    int rand[] = {7,0,1,2,0,3,0,4,2,3,0,3,2,1,2};
    size = sizeof(rand) / sizeof(rand[0]);
    lru_put_array(cache3, rand, size);
    lru_analysis(cache3);

    return 0;
}