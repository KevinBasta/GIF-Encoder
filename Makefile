all: parseMPEG-4.o linkedList.o bitUtility.o printUtility.o linkedList.h bitUtility.h
	gcc -o a.out parseMPEG-4.o linkedList.o bitUtility.o printUtility.o
parseMPEG-4.o: parseMPEG-4.c
	gcc -c parseMPEG-4.c
linkedList.o: linkedList.c
	gcc -c linkedList.c
bitUtility.o: bitUtility.c
	gcc -c bitUtility.c
printUtility.o: printUtility.c
	gcc -c printUtility.c
clean: 
	rm parseMPEG-4.o linkedList.o bitUtility.o printUtility.o a.out