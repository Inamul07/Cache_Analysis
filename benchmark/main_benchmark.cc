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

/*
 * This methods reads the file then stores and returns the contents of the file as a vector.
 * If the given file name is not found then it returns an empty vector.
*/
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

/*
 * This method takes count and access pattern as parameters and returns a file path.
 * Example: If count = 100, accessPattern = SEQ
    It returns data/file_seq_100.txt
 * If invalid access pattern is passed, this returns an empty string
*/
// Code_Review: make accessPattern enum
string getFileName(int count, int accessPattern) {
    string prefix = "data/file_";
    string suffix = ".txt";
    string pattern;
    if(accessPattern == SEQ) pattern = "seq";
    else if(accessPattern == LOOP) pattern = "loop";
    else if(accessPattern == RAND) pattern = "rand";
    else {
        cout << "Invalid Access Pattern" << endl;
        //Code_Review: Exit here
        return "";
    }
    return prefix + pattern + "_" + to_string(count) + suffix;
}

/*
 * Returns a string that can be used to name a benchmark for the given values.
 * It takes cache size, number of reference pages, cache type and access pattern type as parameters.
 * Example: If cacheSize = 10, numOfPages = 100, type = ARC, accessPattern = LOOP
    It returns "ARC_LOOP/10/100".
*/
string getBenchmarkName(int cacheSize, int numOfPages, cacheType type, int accessPattern) {
    string cacheName;
    switch(type) {
        case LRU:
            cacheName = "LRU";
            break;
        case CLOCK:
            cacheName = "CLOCK";
            break;
        case TWO_QUEUE:
            cacheName = "TWO_QUEUE";
            break;
        case ARC:
            cacheName = "ARC";
            break;
        default:
            return "";
    }

    string pattern;
    switch(accessPattern) {
        case SEQ:
            pattern = "_SEQ/";
            break;
        case LOOP:
            pattern = "_LOOP/";
            break;
        case RAND:
            pattern = "_RAND/";
            break;
        default:
            return "";
    }

    return cacheName + pattern + to_string(cacheSize) + "/" + to_string(numOfPages);
}

// Benchmark Function
static void BM_CACHE(benchmark::State &state, int cacheSize, int numOfPages, cacheType cacheName, int accessPattern) {
    generic_cache* cache = cache_init(cacheName, cacheSize);
    string filename = getFileName(numOfPages, accessPattern);
    vector<int> pages = getDataSet(filename);

    for(auto _ : state) {
        cache_put_array(cache, pages.data(), pages.size());
    }

    double hitRatio = cache_get_hit_ratio(cache) * 100.0;
    state.counters["Hit Ratio[%]"] = hitRatio;
    // Code Review: avg needed?
    state.counters["Hashmap Time"] = (cache_get_hashmap_time(cache) * 1000) / state.iterations();

    cache_destroy(cache);
    
}

// A Helper Benchmark To Seperate between the Cache Benchmarks...
static void BM_SEP(benchmark::State &state) {
    for(auto _ : state) {
        
    }
    // Code Review: just return?
}

int main(int argv, char** args) {

    vector<int> cacheSizes = {100, 150, 200};
    vector<int> pageCounts = {10000, 100000, 1000000};
    vector<cacheType> cacheTypes = {LRU, CLOCK, TWO_QUEUE, ARC};
    vector<int> accessPatterns = {SEQ, LOOP, RAND};

    for(int accessPattern : accessPatterns) {
        for(cacheType cacheName : cacheTypes) {
            for(int cacheSize : cacheSizes) {
                for(int numOfPages : pageCounts) {
                    // Registering a Benchmark with different cacheSize, numOfPages, cacheName and accessPattern
                    benchmark::RegisterBenchmark(getBenchmarkName(cacheSize, numOfPages, cacheName, accessPattern), BM_CACHE, cacheSize, numOfPages, cacheName, accessPattern)
                        ->Unit(benchmark::kMillisecond);
                }
            }
            benchmark::RegisterBenchmark("SEPERATOR", BM_SEP)->Iterations(1);
        }
    }

    // Initialising the Benchmark
    ::benchmark::Initialize(&argv, args);

    // Running the Registered Benchmarks
    ::benchmark::RunSpecifiedBenchmarks();
}