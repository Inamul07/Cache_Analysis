all: final

final: main.o lru.o clock.o two_queue.o arc.o utils.o cache_factory.o
	gcc -o main main.o lru.o clock.o two_queue.o arc.o utils.o cache_factory.o -L./lib/ -ldbllist -lhashmap -Wl,-rpath,./lib/
	./main

main.o : main.c
	gcc -c -I./include/ main.c

lru.o: src/lru.c
	gcc -c -I./lib/ -I./include/ src/lru.c

clock.o: src/clock.c
	gcc -c -I./lib/ -I./include/ src/clock.c

two_queue.o: src/two_queue.c
	gcc -c -I./lib/ -I./include/ src/two_queue.c

arc.o: src/arc.c
	gcc -c -I./lib/ -I./include/ src/arc.c

utils.o: src/utils.c
	gcc -c -I./lib/ -I./include/ src/utils.c

cache_factory.o: src/cache_factory.c
	gcc -c -I./lib/ -I./include/ src/cache_factory.c

lib/dbllist.o: lib/dbllist/dbllist.c
	gcc -o lib/dbllist.o -c lib/dbllist/dbllist.c -fpic

lib/hashmap.o: lib/hashmap/hashmap.c
	gcc -o lib/hashmap.o -c lib/hashmap/hashmap.c -fpic

lib/myhashmap.o: lib/hashmap/myhashmap.c
	gcc -o lib/myhashmap.o -c lib/hashmap/myhashmap.c -fpic

makelib: lib/dbllist.o lib/hashmap.o lib/myhashmap.o
	gcc lib/dbllist.o -shared -o lib/libdbllist.so
	gcc lib/hashmap.o lib/myhashmap.o -shared -o lib/libhashmap.so
	rm lib/dbllist.o lib/hashmap.o lib/myhashmap.o

clean:
	rm -f main.o lru.o clock.o two_queue.o arc.o utils.o cache_factory.o main

main_benchmark.o: bench_mark/main_benchmark.cc
	g++ -c -I./lib/ -I./include/ bench_mark/main_benchmark.cc

benchmark: main.o lru.o clock.o two_queue.o arc.o utils.o lib/dbllist.o lib/myhashmap.o main_benchmark.o
	g++ -o benchmark main_benchmark.o lru.o clock.o two_queue.o arc.o utils.o lib/dbllist.o lib/myhashmap.o -L./lib/ -ldbllist -lhashmap -Wl,-rpath,./lib/ -lbenchmark -lpthread
	rm main.o main_benchmark.o lru.o clock.o two_queue.o arc.o utils.o lib/dbllist.o lib/myhashmap.o
	./benchmark --benchmark_counters_tabular=true
	rm benchmark
