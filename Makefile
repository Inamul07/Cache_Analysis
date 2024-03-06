all: final

final: main.o lru.o clock.o
	gcc -o main main.o lru.o clock.o -L. -ldslibrary -Wl,-rpath,.
	./main

main.o: main.c
	gcc -c main.c

clock.o: clock/clock.c
	gcc -c clock/clock.c

dbllist.o: source_files/dbllist.c
	gcc -c source_files/dbllist.c -fpic

hashmap.o: source_files/hashmap.c
	gcc -c source_files/hashmap.c -fpic

lru.o: lru/lru.c
	gcc -c lru/lru.c

makelib: dbllist.o hashmap.o
	gcc dbllist.o hashmap.o -shared -o libdslibrary.so
	rm dbllist.o hashmap.o

clean: 
	rm -f main.o lru.o clock.o main
