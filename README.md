# BZZ

![BZZ Logo](https://github.com/bartossh/bzz/blob/main/artefacts/logo.png)

## Build / install dependencies

BZZ depends on:
 
- [Raylib](https://www.raylib.com/index.html) - raylib is a simple and easy-to-use library to enjoy videogames programming.
- [Tsoding nn.h](https://github.com/tsoding/nn.h) - head-only library only Neural-Network.


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

 - When providing a new files add this  `*.c` files in `TFILES` in `makefile`.
 - Add test in *_test.c file and add it to `TFILES` in `makefile`.
 - Run `make test` to run all the tests. 
 - Run `make memcheck` tu run memcheck tests.

### Running in development mode.
 
 - Add new `*.c` files with new functionalities to `CFILES` in `makefile` 
 - Run for development with `make run`.

## Build

 - To build for your current machine OS and architecture use `make build`.
 - For now only MAC OS is supported.


## Idea

Roll your :game_die: for what this repo will become.
