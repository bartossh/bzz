#include <math.h>
#include <time.h>
#include "raylib.h"
#include "view.h"
#include "../nn/gym.h"
#include "../nn/nn.h"
#include "../flowers/flowers.h"

static void viewBeeLearn(Font font, NN nn, Mat t, GymRect r) 
{
    char buffer[256];
    float s = r.h*0.025;
    float pad = r.h*0.01;
    float f[5] = {0}; // TODO: refactor to use FlowersDataset cols - 1 

    for (size_t i = 0; i < t.rows; ++i) {
        for (size_t j = 0; j < t.cols-1; ++j) {
            float v = MatAt(t, i, j);
            f[j] = v;
            RowAt(NNInput(nn), j) = v;
        }
        nnForward(nn);
        snprintf(
            buffer, sizeof(buffer), 
            "%.3f, %.3f, %.3f, %.3f, %.3f == %.3f", // TODO: refactor to use FlowersDataset cols - 1 
            f[0], f[1], f[2], f[3], f[4], RowAt(NNOutout(nn), 0)
        );
        DrawTextEx(font, buffer, CLITERAL(Vector2){r.x, r.y + i*(s+pad)}, s, 0, WHITE);
    }
}

ViewBee viewBeeNew(void)
{
    const size_t arch_template[] = {5, 10, 5, 1};
    const size_t arch_len = ArrayLen(arch_template);
    size_t *arch = NNMalloc(sizeof(arch_template[0])*arch_len);
    for (size_t i = 0; i < arch_len; ++i) {
        arch[i] = arch_template[i];
    }

    const size_t max_epoch = 100*1000;
    const size_t epochs_per_frame = 103;
    size_t epoch = 0;
    const float rate = 1.0f;
    
    Region temp = regionAllocAlloc(8*1024*1024);
     
    FlowersDataset fl = flowersDatasetNew(Basic_5_30);
    Mat t = flowersToMat(fl);
   
    NN nn = nnAlloc(NULL, arch, arch_len);
    nnRand(nn, -1, 1);
    GymPlot plot = {0};

    ViewBee bee = {
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

void drawBeeView(ViewBee *bee, Font font)
{
    if (bee->reset) {
        bee->epoch = 0;
        nnRand(bee->nn, -1, 1);
        bee->plot.count = 0;
    }
    
    for (size_t i = 0; i < bee->epochs_per_frame && !bee->paused && bee->epoch < bee->max_epoch; ++i) {
        NN g = nnBackprop(&bee->temp, bee->nn, bee->t);
        nnLearn(bee->nn, g, bee->rate);
        bee->epoch += 1;
         DaAppend(&(bee->plot), nnCost(bee->nn, bee->t));
    }
    
    BeginDrawing();
    ClearBackground(ORANGE);
        int w = GetScreenWidth();
        int h = GetScreenHeight();
    
        GymRect r;
        r.w = w;
        r.h = h*2/3;
        r.x = 0;
        r.y = h/2 - r.h/2;
    
        GymLayoutBegin(GloHorz, r, 3, 10);
        gymPlot(bee->plot, GymLayoutSlot(), RED);
        gymRenderNN(bee->nn, GymLayoutSlot());
        viewBeeLearn(font, bee->nn, bee->t, GymLayoutSlot());
        GymLayoutEnd();
    
        DrawText("Train your BEE!", 5, 5, h*0.025, GREEN);
        char buffer[256];
        snprintf(
            buffer, sizeof(buffer), 
            "<| Epoch: %zu/%zu, Rate: %f, Cost: %f, Temporary Memory: %zu bytes |>", 
            bee->epoch, bee->max_epoch, bee->rate, nnCost(bee->nn, bee->t), RegionOccupiedBytes(&bee->temp)
        );
        DrawTextEx(font, buffer,CLITERAL(Vector2){.x = 30, .y = 30}, h*0.02, 0, WHITE);
    EndDrawing();
    
    RegionReset(&bee->temp);
}
