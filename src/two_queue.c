#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "two_queue.h"
#include "hashmap/myhashmap.h"
#include "dbllist/dbllist.h"

struct two_queue {
    dbllist* am;
    dbllist* a1in;
    dbllist* a1out;
    hashmap* amMap;
    hashmap* a1inMap;
    hashmap* a1outMap;
    int amSize, a1inSize, a1outSize;
    int hitCount, missCount;
};

two_queue_cache* two_queue_init(int amSize, int a1inSize, int a1outSize) {
    if(amSize <= 0 || a1inSize <= 0 || a1outSize <= 0) {
        printf("Capacity must be greater than 0\n");
        return NULL;
    }
    two_queue_cache* cache = malloc(sizeof(two_queue_cache));
    cache->am = dbllist_create();
    cache->a1in = dbllist_create();
    cache->a1out = dbllist_create();
    cache->amMap = hmap_create();
    cache->a1inMap = hmap_create();
    cache->a1outMap = hmap_create();
    cache->amSize = amSize;
    cache->a1inSize = a1inSize;
    cache->a1outSize = a1outSize;
    cache->hitCount = 0;
    cache->missCount = 0;
}

int peek_val_and_remove_head(dbllist* list, hashmap* map) {
    int headVal = dbllist_peek_head_val(list);
    dbllist_remove_head(list);
    hmap_remove(map, headVal);
    return headVal;
}

void create_and_insert_node_at_tail(int data, dbllist* list, hashmap* map) {
    Node* node = node_create(data);
    dbllist_insert_node_at_tail(list, node);
    hmap_insert(map, data, node);
}

void two_queue_access(two_queue_cache* cache, int page) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    if(hmap_contains(cache->amMap, page)) { // page in Am (hit)
        Node* node = hmap_get(cache->amMap, page);
        dbllist_move_node_to_tail(cache->am, node);
        cache->hitCount++;
    } else if(hmap_contains(cache->a1inMap, page)) { // page in A1In (hit)
        cache->hitCount++;
    } else if(hmap_contains(cache->a1outMap, page)) { // page in A1Out (miss)
        Node* node = hmap_get(cache->a1outMap, page);
        dbllist_remove_node(cache->a1out, node);
        hmap_remove(cache->a1outMap, page);

        int amCurrSize = dbllist_size(cache->am);
        if(amCurrSize == cache->amSize) {
            int headVal = peek_val_and_remove_head(cache->am, cache->amMap);
        }

        create_and_insert_node_at_tail(page, cache->am, cache->amMap);

        cache->missCount++;
    } else { // Page not in Cache (miss)
        int a1inCurrSize = dbllist_size(cache->a1in);
        if(a1inCurrSize == cache->a1inSize) {
            int headVal = peek_val_and_remove_head(cache->a1in, cache->a1inMap);

            int a1outCurrSize = dbllist_size(cache->a1out);
            if(a1outCurrSize == cache->a1outSize) {
                peek_val_and_remove_head(cache->a1out, cache->a1outMap);
            }

            create_and_insert_node_at_tail(headVal, cache->a1out, cache->a1outMap);
        }

        create_and_insert_node_at_tail(page, cache->a1in, cache->a1inMap);

        cache->missCount++;
    }
}

void two_queue_put_array(two_queue_cache* cache, int* pages, int size) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    for(int i = 0; i < size; i++) {
        two_queue_access(cache, pages[i]);
    }
}

void two_queue_print_buffer(two_queue_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    printf("Am => ");
    dbllist_print(cache->am);
    printf("A1In => ");
    dbllist_print(cache->a1in);
    printf("A1Out => ");
    dbllist_print(cache->a1out);
}

void two_queue_analysis(two_queue_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    printf("Buffer: \n");
    two_queue_print_buffer(cache);
    int totalReference = cache->hitCount + cache->missCount;
    printf("Total References = %d\nHit Count = %d\nMiss Count = %d\n", totalReference, cache->hitCount, cache->missCount);
    float hitRatio = (cache->hitCount * 1.0) / (totalReference);
    printf("Hit Ratio = %f\n", hitRatio);
}

void two_queue_destroy(two_queue_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    dbllist_destroy(cache->am);
    dbllist_destroy(cache->a1in);
    dbllist_destroy(cache->a1out);
    hmap_free(cache->amMap);
    hmap_free(cache->a1inMap);
    hmap_free(cache->a1outMap);
    free(cache);
}