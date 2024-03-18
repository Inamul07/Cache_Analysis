#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "cache_factory.h"
#include "lru.h"
#include "clock.h"
#include "two_queue.h"
#include "arc.h"

struct cache_ {
    void* cache;
    cacheType cacheName;
};

/*
    Creates and Initialises the cache, based on the given cache_name.
    If capacity is less than 1. Returns NULL
    SYNTAX: cache_init(LRU / CLOCK / TWO_QUEUE / ARC, <capacity>);
*/
generic_cache* cache_init(cacheType cacheName, int capacity) {
    if(capacity <= 0) {
        printf("Capacity must be greater than 0\n");
        return NULL;
    }
    generic_cache* cache = (generic_cache*) malloc(sizeof(generic_cache));
    cache->cacheName = cacheName;
    if(cache->cacheName == LRU) {
        cache->cache = lru_init(capacity);
    } else if(cache->cacheName == CLOCK) {
        cache->cache = clock_init(capacity);
    } else if(cache->cacheName == TWO_QUEUE) {
        cache->cache = two_queue_init(capacity);
    } else if(cache->cacheName == ARC) {
        cache->cache = arc_init(capacity);
    } else {
        printf("Invalid Cache Type\n");
        free(cache);
        return NULL;
    }
    return cache;
}

// Performs the respective cache algorithm for the given page.
void cache_access(generic_cache* cache, int page) {
    if(cache == NULL) {
        printf("Cache cannot be NULL\n");
        return;
    }
    // Code Review: Using if else ladder is fine but there is better and more readable way for this.
    if(cache->cacheName == LRU) {
        lru_access(cache->cache, page);
    } else if(cache->cacheName == CLOCK) {
        clock_access(cache->cache, page);
    } else if(cache->cacheName == TWO_QUEUE) {
        two_queue_access(cache->cache, page);
    } else if(cache->cacheName == ARC) {
        arc_access(cache->cache, page);
    }
}

/*
    Prints the Buffer, Total Reference Count, Hit Count and Miss Count of the cache at that current state.
    Reference count must be atleast one before calling this method.
*/
void cache_analysis(generic_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be NULL\n");
        return;
    }
    if(cache->cacheName == LRU) {
        lru_analysis(cache->cache);
    } else if(cache->cacheName == CLOCK) {
        clock_analysis(cache->cache);
    } else if(cache->cacheName == TWO_QUEUE) {
        two_queue_analysis(cache->cache);
    } else if(cache->cacheName == ARC) {
        arc_analysis(cache->cache);
    }
}

// Performs the respective Cache operation for each element, from the array, in a linear fashion
void cache_put_array(generic_cache* cache, int pages[], int size) {
    if(cache == NULL) {
        printf("Cache cannot be NULL\n");
        return;
    }
    if(cache->cacheName == LRU) {
        lru_put_array(cache->cache, pages, size);
    } else if(cache->cacheName == CLOCK) {
        clock_put_array(cache->cache, pages, size);
    } else if(cache->cacheName == TWO_QUEUE) {
        two_queue_put_array(cache->cache, pages, size);
    } else if(cache->cacheName == ARC) {
        arc_put_array(cache->cache, pages, size);
    }
}

// Prints the Cache buffer at that current state
void cache_print_buffer(generic_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be NULL\n");
        return;
    }
    if(cache->cacheName == LRU) {
        lru_print_buffer(cache->cache);
    } else if(cache->cacheName == CLOCK) {
        clock_print_buffer(cache->cache);
    } else if(cache->cacheName == TWO_QUEUE) {
        two_queue_print_buffer(cache->cache);
    } else if(cache->cacheName == ARC) {
        arc_print_buffer(cache->cache);
    }
}

// Frees the memory occupied by the cache
void cache_destroy(generic_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be NULL\n");
        return;
    }
    if(cache->cacheName == LRU) {
        lru_destroy(cache->cache);
    } else if(cache->cacheName == CLOCK) {
        clock_destroy(cache->cache);
    } else if(cache->cacheName == TWO_QUEUE) {
        two_queue_destroy(cache->cache);
    } else if(cache->cacheName == ARC) {
        arc_destroy(cache->cache);
    }
    free(cache);
}

/*
    Calculates and returns the hit ratio at that current state.
    Returns -1, if the cache is NULL or if there were no references before
*/
double cache_get_hit_ratio(generic_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be NULL\n");
        return -1.0;
    }
    if(cache->cacheName == LRU) {
        return lru_get_hit_ratio(cache->cache);
    } else if(cache->cacheName == CLOCK) {
        return clock_get_hit_ratio(cache->cache);
    } else if(cache->cacheName == TWO_QUEUE) {
        return two_queue_get_hit_ratio(cache->cache);
    } else if(cache->cacheName == ARC) {
        return arc_get_hit_ratio(cache->cache);
    }
}