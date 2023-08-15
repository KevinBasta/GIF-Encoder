# built using implicit rules
OBJECTS = main.o \
		  parseMPEG-4.o decodeMPEG-4.o processMPEG-4.o \
		  decodeAVC.o AVCUtility.o \
		  encodeGIF.o \
		  bitUtility.o endianUtility.o \
		  linkedList.o array.o hashmap.o \
		  printUtility.o typesUtility.o \
		  memoryManagement.o

TEST_OBJECTS = test.o \
			   bitUtility.o endianUtility.o \
			   linkedList.o printUtility.o

CC = gcc
CFLAGS += -I src/types 
CFLAGS += -I src/data_structures
CFLAGS += -I src/headers
VPATH = src : src/mpeg : src/gif : src/data_structures : src/util

# main program rule
main: $(OBJECTS)
	gcc -o a.out $(OBJECTS) -lm -Wall -Werror -Wpedantic

# unittesting rule
test: $(TEST_OBJECTS)
	gcc -o a.out $(TEST_OBJECTS) -lm -Wall -Werror -Wpedantic

# cleaning all generated files
clean:
	-rm $(OBJECTS) $(TEST_OBJECTS) a.out 2> /dev/null
