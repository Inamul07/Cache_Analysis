all: final

final: main.o lru.o clock.o
	gcc -o final main.o lru.o clock.o -L. -ldslibrary -Wl,-rpath,.
	./final

main.o: main.c
	gcc -c main.c

dbllist.o: source_files/dbllist.c
	gcc -c source_files/dbllist.c -fpic

hashmap.o: source_files/hashmap.c
	gcc -c source_files/hashmap.c -fpic

lru.o: lru/lru.c
	gcc -c lru/lru.c

clock.o: clock/clock.c
	gcc -Wno-int-conversion -c clock/clock.c

makelib: dbllist.o hashmap.o
	gcc dbllist.o hashmap.o -shared -o libdslibrary.so
	rm dbllist.o hashmap.o

clean: 
	rm main.o lru.o clock.o final
