# Cache Analysis

### In this Repository
- Least Recently Used (LRU)
- Clock Page Replacement

### Objectives
- This repository aims to analyse different page replacement algorithms.
- This repository contains implementations of the said page replacement algorithms, created in accordance with the relevant research papers.

### References
- [Paging Experiment with the Multics System](https://multicians.org/paging-experiment.pdf)
- [Least Recently Used](https://redis.com/glossary/lru-cache/)

### How to use this repository
- Clone this Repository.
  ```bash
  git clone git@github.com:Inamul07/Cache_Analysis.git
  cd Cache_Analysis
  ```
- Edit the `main.c` file to include the required replacement algorithm.
- Then run this command in terminal,
  ```
  make
  ```

### Algorithm Methods

  > Each algorithm contains a set of methods. <br />
  > To use a method, type the cache name followed by an underscore followed by method name. <br />
  > Syntax: `<cache_name>_init()` <br />
  > Example: `lru_init()`, `clock_init()`

  > Pass appropriate arguments to the methods. <br />
  
  | Algorithm | cache_name |
  |----|----|
  | Least Recently Used | lru |
  | Clock Page Replacement | clock |

  #### Methods
  - `<cache_name>_init(int capacity)`
    - Initializes the respective cache with given capacity.
    - returns `<cache_name>_cache*`.
  - `<cache_name>_access(<cache_name>_cache* cache, int page)`
    - Performs the cache operation.
    - Keeps an internal count of number of hits and misses.
    - returns `void`.
  - `<cache_name>_analysis(<cache_name>_cache* cache)`
    - Prints the buffer, total references, number of hits and misses and hit ratio.
    - returns `void`.
  - `<cache_name>_print_buffer(<cache_name>_cache* cache)`
    - Prints the buffer at that current state.
    - returns `void`.
  - `<cache_name>_put_array(<cache_name>_cache* cache, int pages[], int size)`
    - Performs the operation for each data item from `pages` in a linear fashion.
    - returns `void`.
  - `<cache_name>_destroy(<cache_name>_cache* cache)`
    - free the memory allocated by the `cache`.
    - returns `void`.