all: final

final: main.o lru.o clock.o
	gcc -o main main.o lru.o clock.o -L./lib/ -ldslibrary -Wl,-rpath,./lib/
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

makelib: lib/dbllist.o lib/hashmap.o
	gcc lib/dbllist.o lib/hashmap.o -shared -o lib/libdslibrary.so
	rm lib/dbllist.o lib/hashmap.o

clean: 
	rm -f main.o lru.o clock.o main
