all: final

hashmap.o: hashmap/hashmap.c
	gcc -c hashmap/hashmap.c

dbllist.o:
	gcc -c dbllist/dbllist.c

main.o: main.c
	gcc -c main.c

final: main.o hashmap.o dbllist.o
	gcc main.o hashmap.o dbllist.o -o final
	./final


clean: 
	rm main.o hashmap.o dbllist.o final