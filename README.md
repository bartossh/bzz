# BZZ

Bzz is to bzzzzz. I don't know why, what or when.

![BZZ Logo](https://github.com/bartossh/bzz/blob/main/artefacts/logo.png)


## Build / install dependencies

BZZ depends on:
 
- [Raylib](https://www.raylib.com/index.html) - raylib is a simple and easy-to-use library to enjoy videogames programming.
- [Tsoding nn.h](https://github.com/tsoding/nn.h) - head-only Neural-Network library.


### Raylib - build from source

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


### Tsoding nn.h

Copy `https://github.com/tsoding/nn.h/blob/master/nn.h` to `./lib/` folder in this repo root.             


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

- To build for your current machine OS and architecture run command: `make build`.
 - Now only MAC OS is supported, this will change.


## Idea

Roll your :game_die: for this repo becoming something of a value.
