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
    char cacheType[];
};

void* cache_init(char* cacheType, int capacity) {
    generic_cache* cache = (generic_cache*) malloc(sizeof(generic_cache));
    strcpy(cache->cacheType, cacheType);
    if(strcmp(cacheType, "lru") == 0) {
        cache->cache = lru_init(capacity);
    } else if(strcmp(cacheType, "clock") == 0) {
        cache->cache = clock_init(capacity);
    } else if(strcmp(cacheType, "two_queue") == 0) {
        cache->cache = two_queue_init(capacity);
    } else if(strcmp(cacheType, "arc") == 0) {
        cache->cache = arc_init(capacity);
    } else {
        printf("Invalid Cache Type\n");
        free(cache);
        return NULL;
    }
    return cache;
}

void cache_access(void* cache, int page) {
    if(cache == NULL) {
        printf("Cache cannot be NULL\n");
        return;
    }
    generic_cache* genCache = (generic_cache*) cache;
    char cacheType[10];
    strcpy(cacheType, genCache->cacheType);
    if(strcmp(cacheType, "lru") == 0) {
        lru_access(genCache->cache, page);
    } else if(strcmp(cacheType, "clock") == 0) {
        clock_access(genCache->cache, page);
    } else if(strcmp(cacheType, "two_queue") == 0) {
        two_queue_access(genCache->cache, page);
    } else if(strcmp(cacheType, "arc") == 0) {
        arc_access(genCache->cache, page);
    }
}

void cache_analysis(void* cache) {
    if(cache == NULL) {
        printf("Cache cannot be NULL\n");
        return;
    }
    generic_cache* genCache = (generic_cache*) cache;
    char cacheType[10];
    strcpy(cacheType, genCache->cacheType);
    if(strcmp(cacheType, "lru") == 0) {
        lru_analysis(genCache->cache);
    } else if(strcmp(cacheType, "clock") == 0) {
        clock_analysis(genCache->cache);
    } else if(strcmp(cacheType, "two_queue") == 0) {
        two_queue_analysis(genCache->cache);
    } else if(strcmp(cacheType, "arc") == 0) {
        arc_analysis(genCache->cache);
    }
}

void cache_put_array(void* cache, int pages[], int size) {
    if(cache == NULL) {
        printf("Cache cannot be NULL\n");
        return;
    }
    generic_cache* genCache = (generic_cache*) cache;
    char cacheType[10];
    strcpy(cacheType, genCache->cacheType);
    if(strcmp(cacheType, "lru") == 0) {
        lru_put_array(genCache->cache, pages, size);
    } else if(strcmp(cacheType, "clock") == 0) {
        clock_put_array(genCache->cache, pages, size);
    } else if(strcmp(cacheType, "two_queue") == 0) {
        two_queue_put_array(genCache->cache, pages, size);
    } else if(strcmp(cacheType, "arc") == 0) {
        arc_put_array(genCache->cache, pages, size);
    }
}

void cache_print_buffer(void* cache) {
    if(cache == NULL) {
        printf("Cache cannot be NULL\n");
        return;
    }
    generic_cache* genCache = (generic_cache*) cache;
    char cacheType[10];
    strcpy(cacheType, genCache->cacheType);
    if(strcmp(cacheType, "lru") == 0) {
        lru_print_buffer(genCache->cache);
    } else if(strcmp(cacheType, "clock") == 0) {
        clock_print_buffer(genCache->cache);
    } else if(strcmp(cacheType, "two_queue") == 0) {
        two_queue_print_buffer(genCache->cache);
    } else if(strcmp(cacheType, "arc") == 0) {
        arc_print_buffer(genCache->cache);
    }
}

void cache_destroy(void* cache) {
    if(cache == NULL) {
        printf("Cache cannot be NULL\n");
        return;
    }
    generic_cache* genCache = (generic_cache*) cache;
    char cacheType[10];
    strcpy(cacheType, genCache->cacheType);
    if(strcmp(cacheType, "lru") == 0) {
        lru_destroy(genCache->cache);
    } else if(strcmp(cacheType, "clock") == 0) {
        clock_destroy(genCache->cache);
    } else if(strcmp(cacheType, "two_queue") == 0) {
        two_queue_destroy(genCache->cache);
    } else if(strcmp(cacheType, "arc") == 0) {
        arc_destroy(genCache->cache);
    }
    free(cache);
}

double cache_get_hit_ratio(void* cache) {
    if(cache == NULL) {
        printf("Cache cannot be NULL\n");
        return -1.0;
    }
    generic_cache* genCache = (generic_cache*) cache;
    char cacheType[10];
    strcpy(cacheType, genCache->cacheType);
    if(strcmp(cacheType, "lru") == 0) {
        return lru_get_hit_ratio(genCache->cache);
    } else if(strcmp(cacheType, "clock") == 0) {
        return clock_get_hit_ratio(genCache->cache);
    } else if(strcmp(cacheType, "two_queue") == 0) {
        return two_queue_get_hit_ratio(genCache->cache);
    } else if(strcmp(cacheType, "arc") == 0) {
        return arc_get_hit_ratio(genCache->cache);
    }
}