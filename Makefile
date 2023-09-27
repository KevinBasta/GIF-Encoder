# add all c files
SOURCE += $(wildcard src/gif/*.c)
SOURCE += $(wildcard src/util/*.c)
SOURCE += $(wildcard src/data_structures/*.c)

# replace .c suffix with .o
OBJECTS = $(SOURCE:src/%.c=bin/%.o)

# add tests if building test or COMPILE_TESTS is set
COMPILE_TESTS ?= 0
ifneq ($(filter test, $(MAKECMDGOALS)), )
COMPILE_TESTS = 1
endif

ifneq ($(COMPILE_TESTS),0)
TESTS = $(wildcard src/test/*.c)
OBJECTS += $(TESTS:src/%.c=bin/%.o)
$(info $(OBJECTS))
endif

CC = gcc
DIRS = $(sort $(dir $(wildcard src/*/)))
CFLAGS += $(addprefix -I , $(DIRS)) -I src

ifdef DEBUG_PRINTS
CFLAGS += -D PRINT_ENABLE
endif

# main program rule
main: bin/main.o $(OBJECTS)
	$(CC) -o a.out bin/main.o $(OBJECTS) -lm -Wall -Werror -Wpedantic

# unittesting rule
test: bin/test.o $(OBJECTS)
	$(CC) -o a.out bin/test.o $(OBJECTS) -lm -Wall -Werror -Wpedantic

# for shared library must used -fPIC for all compilations
lib: CFLAGS += -fPIC
lib: libstatic libshared

libstatic: $(OBJECTS)
	ar -cvq libgifencoder.a $(OBJECTS)

libshared: $(OBJECTS)
	$(CC) -shared -Wl,-soname,libgifencoder.so.1 -o libgifencoder.so.1.0 $(OBJECTS) -lm -Wall -Werror -Wpedantic
	ln -sf libgifencoder.so.1.0 libgifencoder.so.1
	ln -sf libgifencoder.so.1.0 libgifencoder.so

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
.PHONY: clean cleanbin cleanlib cleanwasm cleangifs
clean: cleanbin cleanlib cleanwasm cleangifs

cleanbin: 
	-rm $(wildcard bin/*/*) $(filter %.o, $(wildcard bin/*)) a.out 2> /dev/null
	-rmdir $(wildcard bin/*/) 2> /dev/null

cleanlib:
	-rm libgifencoder.a libgifencoder.so libgifencoder.so.1 libgifencoder.so.1.0 2> /dev/null

cleanwasm:
	-rm gifencoder.html gifencoder.js gifencoder.wasm 2> /dev/null

cleangifs:
	-rm $(wildcard *.gif) 2> /dev/null

# for testing modules individually during development
foo: $(OBJECTS)
	gcc -o a.out $(OBJECTS) -lm -Wall -Werror -Wpedantic
