all: parseMPEG-4.o linkedList.o bitUtility.o printUtility.o src/headers/linkedList.h src/headers/bitUtility.h
	gcc -o a.out parseMPEG-4.o linkedList.o bitUtility.o printUtility.o
parseMPEG-4.o: src/parseMPEG-4.c
	gcc -c src/parseMPEG-4.c
linkedList.o: src/linkedList.c
	gcc -c src/linkedList.c
bitUtility.o: src/bitUtility.c
	gcc -c src/bitUtility.c
printUtility.o: src/printUtility.c
	gcc -c src/printUtility.c
clean: 
	rm parseMPEG-4.o linkedList.o bitUtility.o printUtility.o a.out