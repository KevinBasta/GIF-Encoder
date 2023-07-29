# built using implicit rules
OBJECTS = main.o \
		  parseMPEG-4.o decodeMPEG-4.o processMPEG-4.o decodeAVC.o \
	 	  AVCUtility.o bitUtility.o endianUtility.o linkedList.o printUtility.o typeUtility.o

TEST_OBJECTS = test.o bitUtility.o endianUtility.o linkedList.o printUtility.o

# cflags aren't required here
CC = gcc
CFLAGS += -I src/headers
VPATH = src:src/mpeg:src/gif:src/util:src/headers

# main program rules
main: $(OBJECTS)
	gcc -o a.out $(OBJECTS) -lm -Wall -Werror -Wpedantic

# unittesting rules
test: $(TEST_OBJECTS)
	gcc -o a.out $(TEST_OBJECTS) -lm -Wall -Werror -Wpedantic

# cleaning all generated files
clean:
	-rm $(OBJECTS) $(TEST_OBJECTS) a.out 2> /dev/null
