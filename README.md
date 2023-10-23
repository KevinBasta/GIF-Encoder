# GIF-Encoder 
A GIF (Graphics Interchange Format) encoder written in the C programming language. The api can be used to create gifs with code as shown in the various examples in [src/test](src/test/). The codebase can be used as a static library, shared library, or web assembly module. The compiled files for these options are in the release package folder (alternatively they can be easily compiled using the makefile). All public interface functions have been prefixed with `gif_` to avoid any namespace collisions. This library/program was created with a strong error-checking system in mind. You can find the meanings of the integer error return values in [src/main.h](src/main.h). Valgrind was used to extensively test this library/program, no memory leaks are present.

The following gifs were created using my encoder. You can find the code for the ERR70_ GIF in [src/gif/GIFError.c](src/gif/GIFError.c). The code for other tests (including the gradient GIF) is located in [src/test](src/test/). A typing effect utility is located in [src/util/typeTextUtility.c](src/util/typeTextUtility.c). Other generated/encoded GIF examples are located in the [examples](examples/) folder.

<img src="examples/error_70.gif" alt="Error Example"/>
<img src="examples/gradient.gif" alt="Gradient Example"/>


## Compiling
static library:
```bash
make libstatic -j8
```

shared library:
```bash
make libshared -j8
```

web assembly ([emscripten](https://emscripten.org/docs/getting_started/downloads.html) required):
```bash
make wasm -j8
```

## Interface
The public interface of this library can be found in [src/libgifencoder_stub.h](src/libgifencoder_stub.h). This is also the file you can include in your c code when using the static or dynamic library.

## Including a library in your code
In order to use this library in your c code, you must link both math.h and the library .a/.so file. To do the latter you must specify the path to the library. Including both math.h and the libgifencoder.a/.so looks like this:

```bash
gcc main.c -Lpath/to/library -lgifencoder -lm
```

For using the shared .so library, an additional step is needed for allowing the library to be found during run time. This step is described in detail [here](http://www.yolinux.com/TUTORIALS/LibraryArchives-StaticAndDynamic.html#:~:text=LDP%3A%20Shared%20libraries-,Library%20Path%3A,-In%20order%20for).

## Acknowledgements
The Graphics Interchange Format(c) is the Copyright property of CompuServe Incorporated.

The lettersAndNumbers.c/h utility contains a copy of the Grand9K Pixel font free for commercial use.
