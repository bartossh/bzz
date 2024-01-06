#include <math.h>
#include <time.h>
#include "raylib.h"
#include "view.h"
#include "../nn/gym.h"
#include "../nn/nn.h"
#include "../flowers/flowers.h"

void verify_nn_gate(Font font, NN nn, Mat t, Gym_Rect r) 
{
    char buffer[256];
    float s = r.h*0.025;
    float pad = r.h*0.01;
    float f[5] = {0}; // TODO: refactor to use FlowersDataset cols - 1 

    for (size_t i = 0; i < t.rows; ++i) {
        for (size_t j = 0; j < t.cols-1; ++j) {
            float v = MAT_AT(t, i, j);
            f[j] = v;
            ROW_AT(NN_INPUT(nn), j) = v;
        }
        nn_forward(nn);
        snprintf(
            buffer, sizeof(buffer), 
            "%.3f, %.3f, %.3f, %.3f, %.3f == %.3f", // TODO: refactor to use FlowersDataset cols - 1 
            f[0], f[1], f[2], f[3], f[4], ROW_AT(NN_OUTPUT(nn), 0)
        );
        DrawTextEx(font, buffer, CLITERAL(Vector2){r.x, r.y + i*(s+pad)}, s, 0, WHITE);
    }
}

PageBee page_nn_new(void)
{
    const size_t arch_template[] = {5, 15, 1};
    const size_t arch_len = ARRAY_LEN(arch_template);
    size_t *arch = NN_MALLOC(sizeof(arch_template[0])*arch_len);
    for (size_t i = 0; i < arch_len; ++i) {
        arch[i] = arch_template[i];
    }

    const size_t max_epoch = 100*1000;
    const size_t epochs_per_frame = 103;
    size_t epoch = 0;
    const float rate = 1.0f;
    
    Region temp = region_alloc_alloc(8*1024*1024);
     
    FlowersDataset fl = flowers_dataset_new(Basic_5_30);
    Mat t = flowers_to_mat(fl);
   
    NN nn = nn_alloc(NULL, arch, arch_len);
    nn_rand(nn, -1, 1);
    Gym_Plot plot = {0};

    PageBee bee = {
        .max_epoch = max_epoch,
        .epochs_per_frame = epochs_per_frame,
        .epoch = epoch, 
        .rate = rate,
        .paused = true,
        .reset = false,
        .temp = temp,
        .fl = fl,
        .t = t,
        .nn = nn,
        .plot = plot,
    };

    return bee;
}

void draw_nn_page(PageBee *bee, Font font)
{
    if (bee->reset) {
        bee->epoch = 0;
        nn_rand(bee->nn, -1, 1);
        bee->plot.count = 0;
    }
    
    for (size_t i = 0; i < bee->epochs_per_frame && !bee->paused && bee->epoch < bee->max_epoch; ++i) {
        NN g = nn_backprop(&bee->temp, bee->nn, bee->t);
        nn_learn(bee->nn, g, bee->rate);
        bee->epoch += 1;
         da_append(&(bee->plot), nn_cost(bee->nn, bee->t));
    }
    
    BeginDrawing();
    ClearBackground(ORANGE);
        int w = GetScreenWidth();
        int h = GetScreenHeight();
    
        Gym_Rect r;
        r.w = w;
        r.h = h*2/3;
        r.x = 0;
        r.y = h/2 - r.h/2;
    
        gym_layout_begin(GLO_HORZ, r, 3, 10);
        gym_plot(bee->plot, gym_layout_slot(), RED);
        gym_render_nn(bee->nn, gym_layout_slot());
        verify_nn_gate(font, bee->nn, bee->t, gym_layout_slot());
        gym_layout_end();
    
        DrawText("Yellow from BZZ!", 5, 5, h*0.025, BLACK);
        char buffer[256];
        snprintf(
            buffer, sizeof(buffer), 
            " | Epoch: %zu/%zu, Rate: %f, Cost: %f, Temporary Memory: %zu bytes | ", 
            bee->epoch, bee->max_epoch, bee->rate, nn_cost(bee->nn, bee->t), region_occupied_bytes(&bee->temp)
        );
        DrawTextEx(font, buffer,CLITERAL(Vector2){.x = 30, .y = 30}, h*0.02, 0, WHITE);
    EndDrawing();
    
    region_reset(&bee->temp);
}
