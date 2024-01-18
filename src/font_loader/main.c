/// Copyright (c) 2024 Bartosz Lenart

#include <raylib.h>
#include <rlgl.h>
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
    Font font = LoadFontEx(path, 60, NULL, 0);
    bool success = ExportFontAsCode(font, name);
    LogError(success, path, name);
    UnloadFont(font);
}

typedef struct {
    const char *path;
    const char *name;
} Candidate;

int main(int argc, char* argv[])
{
    if( argc < 3 ) {
        const Candidate candidates[] = {
            (Candidate){.path = "fonts/Anonymous.ttf", .name = "src/assets/font_anonymous.h"}
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
