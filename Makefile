main:main.o \
	parseMPEG-4.o decodeMPEG-4.o processMPEG-4.o decodeAVC.o \
	AVCUtility.o bitUtility.o endianUtility.o linkedList.o printUtility.o typeUtility.o \
	src/headers/types.h
	gcc -o a.out main.o \
	parseMPEG-4.o decodeMPEG-4.o processMPEG-4.o decodeAVC.o \
    AVCUtility.o bitUtility.o endianUtility.o linkedList.o printUtility.o typeUtility.o \
	-lm -Wall -Werror -Wpedantic

main.o: src/main.c
	gcc -c src/main.c

parseMPEG-4.o: src/mpeg/parseMPEG-4.c src/headers/parseMPEG-4.h
	gcc -c src/mpeg/parseMPEG-4.c

decodeMPEG-4.o: src/mpeg/decodeMPEG-4.c src/headers/decodeMPEG-4.h
	gcc -c src/mpeg/decodeMPEG-4.c

processMPEG-4.o: src/mpeg/processMPEG-4.c src/headers/processMPEG-4.h
	gcc -c src/mpeg/processMPEG-4.c

decodeAVC.o: src/mpeg/decodeAVC.c src/headers/decodeAVC.h
	gcc -c src/mpeg/decodeAVC.c

AVCUtility.o: src/util/AVCUtility.c src/headers/AVCUtility.h
	gcc -c src/util/AVCUtility.c

bitUtility.o: src/util/bitUtility.c src/headers/bitUtility.h
	gcc -c src/util/bitUtility.c

endianUtility.o: src/util/endianUtility.c src/headers/endianUtility.h
	gcc -c src/util/endianUtility.c

linkedList.o: src/util/linkedList.c src/headers/linkedList.h
	gcc -c src/util/linkedList.c

printUtility.o: src/util/printUtility.c src/headers/printUtility.h
	gcc -c src/util/printUtility.c

typeUtility.o: src/util/typeUtility.c src/headers/typeUtility.h
	gcc -c src/util/typeUtility.c

clean: 
	rm	main.o \
	parseMPEG-4.o decodeMPEG-4.o processMPEG-4.o decodeAVC.o \
	AVCUtility.o bitUtility.o endianUtility.o linkedList.o printUtility.o typeUtility.o \
	a.out



test: test.o bitUtility.o endianUtility.o linkedList.o printUtility.o
	gcc -o a.out test.o bitUtility.o endianUtility.o linkedList.o printUtility.o -lm
test.o: src/test.c
	gcc -c src/test.c
ct:
	rm a.out test.o bitUtility.o linkedList.o printUtility.o

