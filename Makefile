all: main.o parseMPEG-4.o processMPEG-4.o linkedList.o typesUtility.o bitUtility.o \
	 printUtility.o src/headers/linkedList.h src/headers/bitUtility.h src/headers/printUtility.h \
	 src/headers/parseMPEG-4.h src/headers/processMPEG-4.h src/headers/typesUtility.h 
	gcc -o a.out main.o parseMPEG-4.o processMPEG-4.o linkedList.o typesUtility.o bitUtility.o printUtility.o -Wall -Werror -Wpedantic

test: test.o bitUtility.o linkedList.o printUtility.o
	gcc -o a.out test.o bitUtility.o linkedList.o printUtility.o

main.o: src/main.c
	gcc -c src/main.c
parseMPEG-4.o: src/parseMPEG-4.c
	gcc -c src/parseMPEG-4.c
processMPEG-4.o: src/processMPEG-4.c
	gcc -c src/processMPEG-4.c
linkedList.o: src/linkedList.c
	gcc -c src/linkedList.c
typesUtility.o: src/typesUtility.c
	gcc -c src/typesUtility.c
bitUtility.o: src/bitUtility.c
	gcc -c src/bitUtility.c
printUtility.o: src/printUtility.c
	gcc -c src/printUtility.c
clean: 
	rm main.o parseMPEG-4.o processMPEG-4.o linkedList.o typesUtility.o bitUtility.o printUtility.o a.out


test.o: src/test.c
	gcc -c src/test.c
ct:
	rm test.o bitUtility.o linkedList.o

