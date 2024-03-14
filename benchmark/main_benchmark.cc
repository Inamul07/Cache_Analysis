#include <benchmark/benchmark.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

extern "C" {
    #include "lru.h"
    #include "clock.h"
    #include "two_queue.h"
    #include "arc.h"
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

static void BM_LRU(benchmark::State &state) {
    lru_cache* cache = lru_init(state.range(0));
    string filename = getFileName(state.range(1));
    vector<int> pages = getDataSet(filename);

    for (auto _ : state) {
        lru_put_array(cache, pages.data(), pages.size());
    }

    double hitPercent = lru_get_hit_ratio(cache) * 100.0;
    state.counters["Hit Percent [%]"] = hitPercent;

    lru_destroy(cache);
}

static void BM_CLOCK(benchmark::State &state) {
    clock_cache* cache = clock_init(state.range(0));
    string filename = getFileName(state.range(1));
    vector<int> pages = getDataSet(filename);


    for(auto _ : state) {
        clock_put_array(cache, pages.data(), pages.size());
    }

    double hitPercent = clock_get_hit_ratio(cache) * 100.0;
    state.counters["Hit Percent [%]"] = hitPercent;

    clock_destroy(cache);
}

static void BM_TWO_QUEUE(benchmark::State &state) {
    two_queue_cache* cache = two_queue_init(state.range(0));
    string filename = getFileName(state.range(1));
    vector<int> pages = getDataSet(filename);

    for(auto _ : state) {
        two_queue_put_array(cache, pages.data(), pages.size());
    }

    double hitPercent = two_queue_get_hit_ratio(cache) * 100.0;
    state.counters["Hit Percent [%]"] = hitPercent;

    two_queue_destroy(cache);
}

static void BM_ARC(benchmark::State &state) {
    arc_cache* cache = arc_init(state.range(0));
    string filename = getFileName(state.range(1));
    vector<int> pages = getDataSet(filename);

    for(auto _ : state) {
        arc_put_array(cache, pages.data(), pages.size());
    }

    double hitPercent = arc_get_hit_ratio(cache) * 100.0;
    state.counters["Hit Percent[%]"] = hitPercent;

    arc_destroy(cache);
}

BENCHMARK(BM_LRU)->ArgsProduct({{100, 150, 200}, {1000, 10000, 100000}})->Unit(benchmark::kMillisecond)->Complexity();
BENCHMARK(BM_CLOCK)->ArgsProduct({{100, 150, 200}, {1000, 10000, 100000}})->Unit(benchmark::kMillisecond)->Complexity();
BENCHMARK(BM_TWO_QUEUE)->ArgsProduct({{100, 150, 200}, {1000, 10000, 100000}})->Unit(benchmark::kMillisecond)->Complexity();
BENCHMARK(BM_ARC)->ArgsProduct({{100, 150, 200}, {1000, 10000, 100000}})->Unit(benchmark::kMillisecond)->Complexity();

BENCHMARK_MAIN();