all: final

final: main.o lru.o clock.o
	gcc -o main main.o lru.o clock.o -L./lib/ -ldbllist -lhashmap -Wl,-rpath,./lib/
	./main

main.o: main.c
	gcc -c main.c

lru.o: src/lru.c
	gcc -c -I./lib/ -I. src/lru.c

clock.o: src/clock.c
	gcc -c -I./lib/ -I. src/clock.c

lib/dbllist.o: lib/dbllist/dbllist.c
	gcc -o lib/dbllist.o -c lib/dbllist/dbllist.c -fpic

lib/hashmap.o: lib/hashmap/hashmap.c
	gcc -o lib/hashmap.o -c lib/hashmap/hashmap.c -fpic

lib/hmap.o: lib/hashmap/hmap.c
	gcc -o lib/hmap.o -c lib/hashmap/hmap.c -fpic

makelib: lib/dbllist.o lib/hashmap.o lib/hmap.o
	gcc lib/dbllist.o -shared -o lib/libdbllist.so
	gcc lib/hashmap.o lib/hmap.o -shared -o lib/libhashmap.so
	rm lib/dbllist.o lib/hashmap.o lib/hmap.o

clean: 
	rm -f main.o lru.o clock.o main
