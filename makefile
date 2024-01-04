# Copyright (c) 2024 Bartosz Lenart

LIBS = -lm

###
CFLAGS  = -std=c17
CFLAGS += -g
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -pedantic
CFLAGS += -Werror
CFLAGS += -Wmissing-declarations
CFLAGS += -DUNITY_SUPPORT_64 -DUNITY_OUTPUT_COLOR

ASANFLAGS  = -fsanitize=address
ASANFLAGS += -fno-common
ASANFLAGS += -fno-omit-frame-pointer

SOURCE_LIBS = -Ilib

OSX_OPT = -Llib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL ./lib/libraylib.a

CFILES = src/main.c
CFILES += src/bzz.c

TFILES = src/bzz_test.c
TFILES += src/bzz.c

.PHONY: run
run: build
	./build/bzz

.PHONY: build
build:
	@echo Compiling $@
	@$(CC) $(ASANFLAGS) $(CFLAGS) $(SOURCE_LIBS) $(OSX_OPT) $(CFILES) -o build/bzz $(LIBS)
	@echo "Build finished"

.PHONY: test
test: tests.out
	@./test/tests.out
	@echo "Tests passed"

.PHONY: memcheck
memcheck:
	@echo Compiling $@
	@$(CC) $(ASANFLAGS) $(CFLAGS) test-framework/unity.c $(SOURCE_LIBS) $(OSX_OPT) $(TFILES) -o test/memcheck.out $(LIBS)
	@./test/memcheck.out
	@echo "Memory check passed"

tests.out:
	@echo Compiling $@
	@$(CC) $(CFLAGS) $(VENDOR) test-framework/unity.c $(SOURCE_LIBS) $(OSX_OPT) $(TFILES) -o test/tests.out $(LIBS)
