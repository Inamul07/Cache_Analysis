#include <benchmark/benchmark.h>
#include <vector>
#include <iostream>
#include <fstream>

#define SEQ 0
#define LOOP 1
#define RAND 2

using namespace std;

extern "C" {
    #include "cache_factory.h"
}

vector<int> getDataSet(string &filename) {
    ifstream file(filename);
    vector<int> dataSet;
    string row;

    if(!file.is_open()) {
        cout<<"File NULL :-("<<endl;
        return dataSet;
    }
    while(getline(file, row)) {
        dataSet.push_back(stoi(row));
    }

    file.close();
    return dataSet;
}

string getFileName(int count, int accessPattern) {
    string prefix = "data/file_";
    string suffix = ".txt";
    string pattern;
    if(accessPattern == SEQ) pattern = "seq";
    else if(accessPattern == LOOP) pattern = "loop";
    else if(accessPattern == RAND) pattern = "rand";
    else pattern = "none";
    return prefix + pattern + "_" + to_string(count) + suffix;
}

static void BM_CACHE(benchmark::State &state) {
    // Code Review: Read state params to meaningful variables first and use it later
    cacheType cacheName = (cacheType) state.range(2);
    generic_cache* cache = cache_init(cacheName, state.range(0));
    string filename = getFileName(state.range(1), state.range(3));
    vector<int> pages = getDataSet(filename);

    for(auto _ : state) {
        cache_put_array(cache, pages.data(), pages.size());
    }

    state.SetComplexityN(state.range(1));

    double hitRatio = cache_get_hit_ratio(cache) * 100.0;
    state.counters["Hit Percent[%]"] = hitRatio;

    cache_destroy(cache);
    
}

// Code Review: Store cache sizes, data set size into vectors and use it.
// Code Review: Make this into a single line... use loops
BENCHMARK(BM_CACHE)->ArgsProduct({{100, 150, 200}, {10000, 100000, 1000000}, {LRU}, {SEQ}})->Unit(benchmark::kMillisecond)->Name("LRU_SEQ")->Complexity();
BENCHMARK(BM_CACHE)->ArgsProduct({{100, 150, 200}, {10000, 100000, 1000000}, {CLOCK}, {SEQ}})->Unit(benchmark::kMillisecond)->Name("CLOCK_SEQ")->Complexity();
BENCHMARK(BM_CACHE)->ArgsProduct({{100, 150, 200}, {10000, 100000, 1000000}, {TWO_QUEUE}, {SEQ}})->Unit(benchmark::kMillisecond)->Name("TWO_QUEUE_SEQ")->Complexity();
BENCHMARK(BM_CACHE)->ArgsProduct({{100, 150, 200}, {10000, 100000, 1000000}, {ARC}, {SEQ}})->Unit(benchmark::kMillisecond)->Name("ARC_SEQ")->Complexity();

BENCHMARK(BM_CACHE)->ArgsProduct({{100, 150, 200}, {10000, 100000, 1000000}, {LRU}, {LOOP}})->Unit(benchmark::kMillisecond)->Name("LRU_LOOP")->Complexity();
BENCHMARK(BM_CACHE)->ArgsProduct({{100, 150, 200}, {10000, 100000, 1000000}, {CLOCK}, {LOOP}})->Unit(benchmark::kMillisecond)->Name("CLOCK_LOOP")->Complexity();
BENCHMARK(BM_CACHE)->ArgsProduct({{100, 150, 200}, {10000, 100000, 1000000}, {TWO_QUEUE}, {LOOP}})->Unit(benchmark::kMillisecond)->Name("TWO_QUEUE_LOOP")->Complexity();
BENCHMARK(BM_CACHE)->ArgsProduct({{100, 150, 200}, {10000, 100000, 1000000}, {ARC}, {LOOP}})->Unit(benchmark::kMillisecond)->Name("ARC_LOOP")->Complexity();

BENCHMARK(BM_CACHE)->ArgsProduct({{100, 150, 200}, {10000, 100000, 1000000}, {LRU}, {RAND}})->Unit(benchmark::kMillisecond)->Name("LRU_RAND")->Complexity();
BENCHMARK(BM_CACHE)->ArgsProduct({{100, 150, 200}, {10000, 100000, 1000000}, {CLOCK}, {RAND}})->Unit(benchmark::kMillisecond)->Name("CLOCK_RAND")->Complexity();
BENCHMARK(BM_CACHE)->ArgsProduct({{100, 150, 200}, {10000, 100000, 1000000}, {TWO_QUEUE}, {RAND}})->Unit(benchmark::kMillisecond)->Name("TWO_QUEUE_RAND")->Complexity();
BENCHMARK(BM_CACHE)->ArgsProduct({{100, 150, 200}, {10000, 100000, 1000000}, {ARC}, {RAND}})->Unit(benchmark::kMillisecond)->Name("ARC_RAND")->Complexity();

BENCHMARK_MAIN();