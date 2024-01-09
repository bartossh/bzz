/// Copyright (c) 2024 Bartosz Lenart

#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <assert.h>

#ifndef IMAssert
#define IMAssert assert
#endif //IMAssert

void logger(const char *path, const char*name, const char *code_file_path, int code_line)
{
    printf("Exporting file %s to code file %s in: %s:%i\n", path, name, code_file_path, code_line);
}

#define LogError(ls, path, name) (IMAssert(ls), logger(path, name, __FILE__, __LINE__))
#define CandidatesLen(c) sizeof(c)/sizeof(c[0])

inline static void load(const char *path, const char *name) 
{
    Image img = LoadImage(path);
    bool success = ExportImageAsCode(img, name);
    LogError(success, path, name);
    UnloadImage(img);
}

typedef struct {
    const char *path;
    const char *name;
} Candidate;

int main(void)
{
    const Candidate candidates[1] = {
        (Candidate){.path = "assets/logo.png", .name = "src/assets/logo.h"}
    };

    size_t len = CandidatesLen(candidates);
    for (size_t i = 0; i < len; ++i) {
        load(candidates[i].path, candidates[i].name);
    }
    return 0;
}
