# add all c files
SOURCE += $(wildcard src/gif/*.c)
SOURCE += $(wildcard src/util/*.c)
SOURCE += $(wildcard src/data_structures/*.c)
SOURCE += $(wildcard src/test/*.c)

# replace .c suffix with .o
OBJECTS = $(SOURCE:src/%.c=bin/%.o)

CC = gcc
DIRS = $(sort $(dir $(wildcard src/*/)))
CFLAGS += $(addprefix -I , $(DIRS)) -I src

# main program rule
main: bin/main.o $(OBJECTS)
	$(CC) -o a.out bin/main.o $(OBJECTS) -lm -Wall -Werror -Wpedantic

# unittesting rule
test: bin/test.o $(OBJECTS)
	$(CC) -o a.out bin/test.o $(OBJECTS) -lm -Wall -Werror -Wpedantic

# for shared library must used -fPIC for all compilations
lib: $(OBJECTS)
	ar -cvq libgifencoder.a $(OBJECTS)

#objcopy --prefix-symbols=gifEncoder_ libgifencoder.a
#$(CC) -shared -o libgifencoder.so $(OBJECTS) -lm -Wall -Werror -Wpedantic

# compile web assembly
wasm: CFLAGS += -D WASM
wasm: $(SOURCE)
	emcc -O3 -s WASM=1 \
    $(SOURCE) $(CFLAGS) -o gifEncoder.html \
	-s NO_EXIT_RUNTIME=1 -s "EXPORTED_RUNTIME_METHODS=['ccall']" -sFORCE_FILESYSTEM

# c file rules, -c src/file.c -o bin/file.o
bin/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ -Wall

# cleaning all generated files
clean:
	-rm libgifencoder.a libgifencoder.so 2> /dev/null
	-rm gifEncoder.html gifEncoder.js gifEncoder.wasm 2> /dev/null
	-rm $(wildcard *.gif) 2> /dev/null
	-rm $(OBJECTS) bin/main.o bin/test.o a.out 2> /dev/null
	-rmdir $(sort $(dir $(OBJECTS))) 2> /dev/null

# for testing modules individually during development
foo: $(OBJECTS)
	gcc -o a.out $(OBJECTS) -lm -Wall -Werror -Wpedantic
