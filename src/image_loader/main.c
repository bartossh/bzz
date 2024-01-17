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
            (Candidate){.path = "assets/learn_button.png", .name = "src/assets/learn_button.h"},
            (Candidate){.path = "assets/flower_a.png", .name = "src/assets/flower_a.h"},
            (Candidate){.path = "assets/flower_b.png", .name = "src/assets/flower_b.h"},
            (Candidate){.path = "assets/flower_c.png", .name = "src/assets/flower_c.h"},
            (Candidate){.path = "assets/flower_d.png", .name = "src/assets/flower_d.h"},
            (Candidate){.path = "assets/flower_e.png", .name = "src/assets/flower_e.h"},
            (Candidate){.path = "assets/flower_f.png", .name = "src/assets/flower_f.h"},
            (Candidate){.path = "assets/flower_g.png", .name = "src/assets/flower_g.h"},
            (Candidate){.path = "assets/flower_h.png", .name = "src/assets/flower_h.h"},
            (Candidate){.path = "assets/flower_i.png", .name = "src/assets/flower_i.h"},
            (Candidate){.path = "assets/flower_j.png", .name = "src/assets/flower_j.h"},
            (Candidate){.path = "assets/flower_k.png", .name = "src/assets/flower_k.h"},
            (Candidate){.path = "assets/flower_l.png", .name = "src/assets/flower_l.h"},
            (Candidate){.path = "assets/flower_m.png", .name = "src/assets/flower_m.h"},
            (Candidate){.path = "assets/flower_n.png", .name = "src/assets/flower_n.h"},
            (Candidate){.path = "assets/flower_o.png", .name = "src/assets/flower_o.h"},
            (Candidate){.path = "assets/flower_p.png", .name = "src/assets/flower_p.h"},
            (Candidate){.path = "assets/flower_r.png", .name = "src/assets/flower_r.h"},
            (Candidate){.path = "assets/flower_s.png", .name = "src/assets/flower_s.h"},
            (Candidate){.path = "assets/flower_t.png", .name = "src/assets/flower_t.h"},
            (Candidate){.path = "assets/flower_u.png", .name = "src/assets/flower_u.h"},
            (Candidate){.path = "assets/flower_w.png", .name = "src/assets/flower_w.h"},
            (Candidate){.path = "assets/flower_x.png", .name = "src/assets/flower_x.h"},
            (Candidate){.path = "assets/flower_y.png", .name = "src/assets/flower_y.h"},
            (Candidate){.path = "assets/flower_z.png", .name = "src/assets/flower_z.h"},
            (Candidate){.path = "assets/flower_q.png", .name = "src/assets/flower_q.h"},
            (Candidate){.path = "assets/flower_v.png", .name = "src/assets/flower_v.h"},
            (Candidate){.path = "assets/flower_1.png", .name = "src/assets/flower_1.h"},
            (Candidate){.path = "assets/flower_2.png", .name = "src/assets/flower_2.h"},
            (Candidate){.path = "assets/flower_3.png", .name = "src/assets/flower_3.h"},
            (Candidate){.path = "assets/flower_4.png", .name = "src/assets/flower_4.h"},
            (Candidate){.path = "assets/flower_5.png", .name = "src/assets/flower_5.h"},
            (Candidate){.path = "assets/flower_6.png", .name = "src/assets/flower_6.h"},
            (Candidate){.path = "assets/flower_7.png", .name = "src/assets/flower_7.h"},
            (Candidate){.path = "assets/flower_8.png", .name = "src/assets/flower_8.h"},
            (Candidate){.path = "assets/flower_9.png", .name = "src/assets/flower_9.h"},
            (Candidate){.path = "assets/flower_0.png", .name = "src/assets/flower_0.h"}
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
