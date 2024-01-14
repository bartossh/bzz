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
#define PathLen 256

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

int main(int argc, char* argv[]) {
    if( argc < 3 ) {
        const Candidate candidates[] = {
            (Candidate){.path = "assets/logo.png", .name = "src/assets/logo.h"},
            (Candidate){.path = "assets/plus.png", .name = "src/assets/plus_button.h"},
            (Candidate){.path = "assets/minus.png", .name = "src/assets/minus_button.h"},
            (Candidate){.path = "assets/update_button.png", .name = "src/assets/update_button.h"},
            (Candidate){.path = "assets/honeycomb.png", .name = "src/assets/honeycomb.h"},
            (Candidate){.path = "assets/bee.png", .name = "src/assets/bee_button.h"},
            (Candidate){.path = "assets/bee_flying.png", .name = "src/assets/bee_flying.h"},
            (Candidate){.path = "assets/map_button.png", .name = "src/assets/map_button.h"},
            (Candidate){.path = "assets/learn_button.png", .name = "src/assets/learn_button.h"}
        };

        size_t len = CandidatesLen(candidates);
        for (size_t i = 0; i < len; ++i) {
            load(candidates[i].path, candidates[i].name);
        }
        return 0;
    }
  
    char input_path[PathLen];
    snprintf(input_path, PathLen*sizeof(char), "%s", argv[1]);
    
    char output_path[PathLen];
    snprintf(output_path, PathLen*sizeof(char), "%s", argv[2]);
    load(input_path, output_path);
    
    return 0;
}
