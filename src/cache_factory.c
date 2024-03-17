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
    // Code Review: 0 sized array? you are not allocating memory for this so check how this works. After that change this to enum
    char cacheName[];
};

/*
    Creates and Initialises the cache, based on the given cache_name.
    If capacity is less than 1. Returns NULL
*/
generic_cache* cache_init(char* cacheName, int capacity) {
    if(capacity <= 0) {
        printf("Capacity must be greater than 0\n");
        return NULL;
    }
    generic_cache* cache = (generic_cache*) malloc(sizeof(generic_cache));
    strcpy(cache->cacheName, cacheName);
    if(strcmp(cacheName, "lru") == 0) {
        cache->cache = lru_init(capacity);
    } else if(strcmp(cacheName, "clock") == 0) {
        cache->cache = clock_init(capacity);
    } else if(strcmp(cacheName, "two_queue") == 0) {
        cache->cache = two_queue_init(capacity);
    } else if(strcmp(cacheName, "arc") == 0) {
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
    generic_cache* genCache = (generic_cache*) cache;
    char cacheName[10];
    strcpy(cacheName, genCache->cacheName);
    // Code Review: Using if else ladder is fine but there is better and more readable way for this.
    if(strcmp(cacheName, "lru") == 0) {
        lru_access(genCache->cache, page);
    } else if(strcmp(cacheName, "clock") == 0) {
        clock_access(genCache->cache, page);
    } else if(strcmp(cacheName, "two_queue") == 0) {
        two_queue_access(genCache->cache, page);
    } else if(strcmp(cacheName, "arc") == 0) {
        arc_access(genCache->cache, page);
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
    generic_cache* genCache = (generic_cache*) cache;
    char cacheName[10];
    strcpy(cacheName, genCache->cacheName);
    if(strcmp(cacheName, "lru") == 0) {
        lru_analysis(genCache->cache);
    } else if(strcmp(cacheName, "clock") == 0) {
        clock_analysis(genCache->cache);
    } else if(strcmp(cacheName, "two_queue") == 0) {
        two_queue_analysis(genCache->cache);
    } else if(strcmp(cacheName, "arc") == 0) {
        arc_analysis(genCache->cache);
    }
}

// Performs the respective Cache operation for each element, from the array, in a linear fashion
void cache_put_array(generic_cache* cache, int pages[], int size) {
    if(cache == NULL) {
        printf("Cache cannot be NULL\n");
        return;
    }
    generic_cache* genCache = (generic_cache*) cache;
    char cacheName[10];
    strcpy(cacheName, genCache->cacheName);
    if(strcmp(cacheName, "lru") == 0) {
        lru_put_array(genCache->cache, pages, size);
    } else if(strcmp(cacheName, "clock") == 0) {
        clock_put_array(genCache->cache, pages, size);
    } else if(strcmp(cacheName, "two_queue") == 0) {
        two_queue_put_array(genCache->cache, pages, size);
    } else if(strcmp(cacheName, "arc") == 0) {
        arc_put_array(genCache->cache, pages, size);
    }
}

// Prints the Cache buffer at that current state
void cache_print_buffer(generic_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be NULL\n");
        return;
    }
    generic_cache* genCache = (generic_cache*) cache;
    char cacheName[10];
    strcpy(cacheName, genCache->cacheName);
    if(strcmp(cacheName, "lru") == 0) {
        lru_print_buffer(genCache->cache);
    } else if(strcmp(cacheName, "clock") == 0) {
        clock_print_buffer(genCache->cache);
    } else if(strcmp(cacheName, "two_queue") == 0) {
        two_queue_print_buffer(genCache->cache);
    } else if(strcmp(cacheName, "arc") == 0) {
        arc_print_buffer(genCache->cache);
    }
}

// Frees the memory occupied by the cache
void cache_destroy(generic_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be NULL\n");
        return;
    }
    generic_cache* genCache = (generic_cache*) cache;
    char cacheName[10];
    strcpy(cacheName, genCache->cacheName);
    if(strcmp(cacheName, "lru") == 0) {
        lru_destroy(genCache->cache);
    } else if(strcmp(cacheName, "clock") == 0) {
        clock_destroy(genCache->cache);
    } else if(strcmp(cacheName, "two_queue") == 0) {
        two_queue_destroy(genCache->cache);
    } else if(strcmp(cacheName, "arc") == 0) {
        arc_destroy(genCache->cache);
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
    generic_cache* genCache = (generic_cache*) cache;
    char cacheName[10];
    strcpy(cacheName, genCache->cacheName);
    if(strcmp(cacheName, "lru") == 0) {
        return lru_get_hit_ratio(genCache->cache);
    } else if(strcmp(cacheName, "clock") == 0) {
        return clock_get_hit_ratio(genCache->cache);
    } else if(strcmp(cacheName, "two_queue") == 0) {
        return two_queue_get_hit_ratio(genCache->cache);
    } else if(strcmp(cacheName, "arc") == 0) {
        return arc_get_hit_ratio(genCache->cache);
    }
}