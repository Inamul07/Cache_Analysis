all: final

final: main.o
	gcc -o final main.o -L. -ldslibrary -Wl,-rpath,.
	./final

main.o: main.c
	gcc -c main.c

dbllist.o: source_files/dbllist.c
	gcc -c source_files/dbllist.c -fpic

hashmap.o: source_files/hashmap.c
	gcc -c source_files/hashmap.c -fpic

makelib: dbllist.o hashmap.o
	gcc dbllist.o hashmap.o -shared -o libdslibrary.so
	rm dbllist.o hashmap.o

clean: 
	rm main.o final
