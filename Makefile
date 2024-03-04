test: main.o dbllist.o hashmap.o
	gcc main.o dbllist.o hashmap.o -o final
	rm dbllist.o hashmap.o main.o
	./final

all: final

main.o: main.c
	gcc -Wno-format -Wno-int-conversion -c main.c

final: main.c
	gcc main.c -o main -L -ldatastructures
	./main

dbllist.o: dbllist/dbllist.c
	gcc -c dbllist/dbllist.c

hashmap.o: hashmap/hashmap.c
	gcc -c hashmap/hashmap.c

lib: dbllist.o hashmap.o
	ar rcs libdatastructures.a dbllist.o hashmap.o

clean: 
	rm main.o final
