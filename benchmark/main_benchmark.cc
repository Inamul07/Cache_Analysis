#include <benchmark/benchmark.h>
#include <vector>
#include <iostream>
#include <fstream>

#define LRU 0
#define CLOCK 1
#define TWO_QUEUE 2
#define ARC 3

using namespace std;

extern "C" {
    #include "lru.h"
    #include "clock.h"
    #include "two_queue.h"
    #include "arc.h"
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

string getFileName(int count) {
    string prefix = "data/file_";
    string suffix = ".txt";
    return prefix + to_string(count) + suffix;
}

string get_cache_name(int choice) {
    if(choice == LRU) return "lru";
    else if(choice == CLOCK) return "clock";
    else if(choice == TWO_QUEUE) return "two_queue";
    return "arc";
}

static void BM_CACHE(benchmark::State &state) {
    string cacheName = get_cache_name(state.range(0));
    generic_cache* cache = cache_init(cacheName.data(), state.range(1));
    string filename = getFileName(state.range(2));
    vector<int> pages = getDataSet(filename);

    for(auto _ : state) {
        cache_put_array(cache, pages.data(), pages.size());
    }

    double hitRatio = cache_get_hit_ratio(cache) * 100.0;
    state.counters["Hit Percent[%]"] = hitRatio;

    cache_destroy(cache);
    
}

BENCHMARK(BM_CACHE)->ArgsProduct({{LRU}, {100, 150, 200}, {1000, 10000, 100000}})->Unit(benchmark::kMillisecond)->Name("LRU")->Complexity();
BENCHMARK(BM_CACHE)->ArgsProduct({{CLOCK}, {100, 150, 200}, {1000, 10000, 100000}})->Unit(benchmark::kMillisecond)->Name("CLOCK")->Complexity();
BENCHMARK(BM_CACHE)->ArgsProduct({{TWO_QUEUE}, {100, 150, 200}, {1000, 10000, 100000}})->Unit(benchmark::kMillisecond)->Name("TWO_QUEUE")->Complexity();
BENCHMARK(BM_CACHE)->ArgsProduct({{ARC}, {100, 150, 200}, {1000, 10000, 100000}})->Unit(benchmark::kMillisecond)->Name("ARC")->Complexity();

BENCHMARK_MAIN();