all: final

# CODE_REVIEW: give a meaningful object name
final: main.o clock.o
	gcc -o final main.o clock.o -L. -ldslibrary -Wl,-rpath,.
	./final

# CODE_REVIEW: understang -c flag
main.o: main.c
	gcc -c main.c

clock.o: clock/clock.c
	gcc -c clock/clock.c

dbllist.o: source_files/dbllist.c
	gcc -c source_files/dbllist.c -fpic

hashmap.o: source_files/hashmap.c
	gcc -c source_files/hashmap.c -fpic

makelib: dbllist.o hashmap.o
	gcc dbllist.o hashmap.o -shared -o libdslibrary.so
	rm dbllist.o hashmap.o

# CODE_REVIEW: this should never throw error even if files are not found
clean: 
	rm main.o clock.o final
