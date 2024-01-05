/// Copyright (c) 2024 Bartosz Lenart

//#include "raylib.h"
//   
//int main(void)
//{
//    InitWindow(1000, 650, "BZZ!");
//    Font font = LoadFontEx("./fonts/Anonymous.ttf", 60, NULL, 0);
//    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
//    
//    while (!WindowShouldClose())
//    {
//        BeginDrawing();
//            ClearBackground(DARKGREEN);
//            DrawText("Yellow from BZZ!", 10, 10, 30, ORANGE);
//        EndDrawing();
//    }
//    
//    CloseWindow();
//    
//    return 0;
//}


#include <math.h>
#include <time.h>

#define GYM_IMPLEMENTATION
#include "nn/gym.h"

#include "nn/nn.h"
#include "flowers/flowers.h"

size_t arch[] = {5, 8, 1};
const size_t max_epoch = 100*1000;
const size_t epochs_per_frame = 103;
const float rate = 1.0f;
bool paused = true;

void verify_nn_gate(Font font, NN nn, Mat t, Gym_Rect r)
{
    char buffer[256];
    float s = r.h*0.025;
    float pad = r.h*0.01;
    float f[5] = {0};

    for (size_t i = 0; i < 10; ++i) {
        for (size_t j = 0; j < 5; ++j) {
            float v = MAT_AT(t, i, j);
            f[j] = v;
            ROW_AT(NN_INPUT(nn), j) = v;
        }
        nn_forward(nn);
        snprintf(
            buffer, sizeof(buffer), 
            "%.3f, %.3f, %.3f, %.3f, %.3f == %.3f", 
            f[0], f[1], f[2], f[3], f[4], ROW_AT(NN_OUTPUT(nn), 0)
        );
        DrawTextEx(font, buffer, CLITERAL(Vector2){r.x, r.y + i*(s+pad)}, s, 0, WHITE);
    }
}

int main(void)
{
    srand(time(NULL));
    Region temp = region_alloc_alloc(256*1024*1024);

    //Mat t = mat_alloc(NULL, 20, 6);
    //for (size_t row = 0; row < 20; ++row) {
    //        for (size_t coll = 0; coll < 5; ++coll) {
    //            MAT_AT(t, row, coll) = rand_float();
    //    }
    //    MAT_AT(t, row, 5) = rand_float() > 0.5f ? 1.0f : 0.0f;
    //}
    
    Mat t = flowers_basic_matrix();

    NN nn = nn_alloc(NULL, arch, ARRAY_LEN(arch));
    nn_rand(nn, -1, 1);

    size_t WINDOW_FACTOR = 80;
    size_t WINDOW_WIDTH = (16*WINDOW_FACTOR);
    size_t WINDOW_HEIGHT = (9*WINDOW_FACTOR);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "BZZ!");
    SetTargetFPS(60);

    Font font = LoadFontEx("./fonts/Anonymous.ttf", 60, NULL, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    Gym_Plot plot = {0};

    size_t epoch = 0;
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            paused = !paused;
        }
        if (IsKeyPressed(KEY_R)) {
            epoch = 0;
            nn_rand(nn, -1, 1);
            plot.count = 0;
        }

        for (size_t i = 0; i < epochs_per_frame && !paused && epoch < max_epoch; ++i) {
            NN g = nn_backprop(&temp, nn, t);
            nn_learn(nn, g, rate);
            epoch += 1;
            da_append(&plot, nn_cost(nn, t));
        }

        BeginDrawing();
        ClearBackground(DARKGREEN);
        {
            int w = GetScreenWidth();
            int h = GetScreenHeight();

            Gym_Rect r;
            r.w = w;
            r.h = h*2/3;
            r.x = 0;
            r.y = h/2 - r.h/2;

            gym_layout_begin(GLO_HORZ, r, 3, 10);
            gym_plot(plot, gym_layout_slot(), RED);
            gym_render_nn(nn, gym_layout_slot());
            verify_nn_gate(font, nn, t, gym_layout_slot());
            gym_layout_end();

            DrawText("Yellow from BZZ!", 5, 5, h*0.025, ORANGE);
            char buffer[256];
            snprintf(
                buffer, sizeof(buffer), 
                " | Epoch: %zu/%zu, Rate: %f, Cost: %f, Temporary Memory: %zu bytes | ", 
                epoch, max_epoch, rate, nn_cost(nn, t), region_occupied_bytes(&temp)
            );
            DrawTextEx(font, buffer,CLITERAL(Vector2){.x = 30, .y = 30}, h*0.02, 0, WHITE);
        }
        EndDrawing();

        region_reset(&temp);
    }

    return 0;
}


