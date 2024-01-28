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

# -Werror -Wpedantic
CFLAGS += -lm -Wall

ifdef PROFILE
CFLAGS += -pg
endif

# main program rule
main: bin/main.o $(OBJECTS)
	$(CC) -o a.out bin/main.o $(OBJECTS) $(CFLAGS)

# unittesting rule
test: bin/test.o $(OBJECTS)
	$(CC) -o a.out bin/test.o $(OBJECTS) $(CFLAGS) 

# static library rule
libstatic: $(OBJECTS)
	ar -cvq libgifencoder.a $(OBJECTS)

# shared library rule must used -fPIC for all compilations
libshared: CFLAGS += -fPIC
libshared: $(OBJECTS)
	$(CC) -shared -Wl,-soname,libgifencoder.so.1 -o libgifencoder.so.1.0 $(OBJECTS) -lm -Wall -Werror -Wpedantic
	ln -sf libgifencoder.so.1.0 libgifencoder.so.1
	ln -sf libgifencoder.so.1.0 libgifencoder.so

# compile web assembly
wasm: CFLAGS += -D WASM
wasm: $(SOURCE)
	emcc \
    $(SOURCE) $(CFLAGS) \
	-O3 -s WASM=1 \
	-o gifEncoder.js \
	-sMODULARIZE=1 \
	-sFORCE_FILESYSTEM=1 \
	-sNO_EXIT_RUNTIME=1 \
	-sEXPORTED_RUNTIME_METHODS='["ccall", "FS"]' \
	-sALLOW_MEMORY_GROWTH=1 \
	-sEXPORT_NAME='gifEncoder' \

# c file rules, -c src/file.c -o bin/file.o
bin/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CFLAGS)

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
