# GIF-Encoder 
A GIF (Graphics Interchange Format) encoder written in the C programming language. The codebase can be used as a static library, shared library, or web assembly module. The compiled files for these options are in the release package folder (alternatively they can be easily compiled using the makefile). All public interface functions have been prefixed with `gif_` to avoid any namespace collisions. This library/program was created with a strong error-checking system in mind. You can find the meanings of the error code return values in [src/main.h](src/main.h).

The following gifs were created using my encoder. You can find the code for the ERR70_ GIF in [src/gif/GIFError.c](src/gif/GIFError.c). The code for other tests (including the gradient GIF) is located in [src/test](src/test/). Other generated/encoded GIF examples are located in the [examples](examples/) folder.

<img src="examples/error_70.gif" alt="Error Example"/>
<img src="examples/gradient.gif" alt="Gradient Example"/>


## Compiling
static and shared libraries:
```bash
make clean; make lib -j8
```

web assembly:

```bash
make clean; make wasm -j8
```

## Interface
The public interface of this library can be found in [src/libgifencoder_stub.h](src/libgifencoder_stub.h). This is also the file you can include in your c code when using the static or dynamic library.

## Including a library in your code
In order to use this library in your c code, you must link both math.h and the library .a/.so file. To do the latter you must specify the path to the library. Including both math.h and the libgifencoder.a/.so looks like this:

```bash
gcc main.c -Lpath/to/library -lgifencoder -lm
```

## Acknowledgements
The Graphics Interchange Format(c) is the Copyright property of CompuServe Incorporated.

The lettersAndNumbers.c/h utility contains a copy of the Grand9K Pixel font free for commercial use.
