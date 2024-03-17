all: final clean

OBJECTS = lru.o clock.o two_queue.o arc.o utils.o cache_factory.o
LIBRARY = -L./lib/ -ldbllist -lhashmap -Wl,-rpath,./lib/

final: main.o $(OBJECTS)
	gcc -o main main.o $(OBJECTS) $(LIBRARY)
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
	rm -f main.o $(OBJECTS) main

main_benchmark.o: benchmark/main_benchmark.cc
	g++ -c -I./lib/ -I./include/ benchmark/main_benchmark.cc

benchmark: main.o $(OBJECTS) main_benchmark.o
	g++ -o bench main_benchmark.o $(OBJECTS) $(LIBRARY) -lbenchmark -lpthread
	rm main.o main_benchmark.o $(OBJECTS)
	./bench --benchmark_counters_tabular=true
	rm bench
