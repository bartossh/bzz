# BZZ

Bzz is to bzzzzz. Teach your bee to bzzzzz safely.

![BZZ Logo](https://github.com/bartossh/bzz/blob/main/artefacts/logo.png)


## Build / install dependencies

BZZ depends on:
 
- [Raylib](https://www.raylib.com/index.html) - raylib is a simple and easy-to-use library to enjoy videogames programming.

### Raylib - build from source OSX developemnt

Download version [4.0.0](https://github.com/raysan5/raylib/releases/tag/4.0.0) -> source code.

```sh
tar -xf raylib-4.0.0.tar.gz
```

```sh
cd raylib-4.0.0
```

```sh
cmake -Wno-dev .
```

```sh
cmake --build .
```

Finally:

Copy `libraylib.a`, `raudio.h`. `raylib.h`, `raymath.h`, `rlgl.h` to `./lib/` folder in this repo root.


### Raylib - Web Assembly compilation with emscripten

Follow instruction from [Emscripten web page](https://emscripten.org/docs/getting_started/downloads.html).

If you are using OSX just brew the Emscripten `brew install emscripten`.

Build raylib using emscripten compiler.

```sh
emcc -c rcore.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
emcc -c rshapes.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
emcc -c rtextures.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
emcc -c rtext.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
emcc -c rmodels.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
emcc -c utils.c -Os -Wall -DPLATFORM_WEB
emcc -c raudio.c -Os -Wall -DPLATFORM_WEB

emar rcs libraylib.a rcore.o rshapes.o rtextures.o rtext.o rmodels.o utils.o raudio.o
```

Copy `libraylib.a`, `raudio.h`. `raylib.h`, `raymath.h`, `rlgl.h` to `./wasm_lib/` folder in this repo root.

You are ready to go.

## Development

This software is still in the beginning of development stage. Keep your expectations low.

### Testing 

 - When creating new files, add this  `*.c` files into `TFILES` in `makefile`.
 - Create tests in `*_test.c` file, and add this file into `TFILES` in `makefile`.
 - Run command: `make test`, to run all the tests.
 - Run command: `make memcheck`, tu run memcheck tests.

### Running in development mode.
 
 - Add new `*.c` files with new functionalities to `CFILES` in `makefile`.
 - For development run command: `make run`.

## Build

### OS PLATFORM

- To build for your current machine OS and architecture run command: `make build`.
- Now only MAC OS is supported, this will change.

### WASM

- To build for Wab Assembly run command: `make wasm`.
- To check build `cd` in to `web_build` directory and run `python -m http.server 8080` go in your browser to [http://localhost:8080](http://localhost:8080)


## Idea

Roll your :game_die: for this repo becoming something of a value.

## Thanks and inspiration

 - Thanks to [rexim](https://github.com/rexim) for Tsoding/nn.h -> `https://github.com/tsoding/nn.h/blob/master/nn.h` an amazing Neural Network library and great streaming on how to code and hack things in `C`. I was truly inspired. I will blend this library in to my code simply because of the project requirements.
 - [Lingon Studios](https://twitter.com/lingonstudios) for high level description of graphical design.

