/// Copyright (c) 2024 Bartosz Lenart

#include <float.h>
#include <raylib.h>
#include <raymath.h>
#include <string.h>
#include <time.h>
#include "raylib.h"
#include "game.h"
#include "../nn/nn.h"
#include "../levels/levels.h"

BzzBeeGame bzzBzzBeeGameNew(
    Font font, BzzButton minus_button, BzzButton plus_button, BzzButton learn_button, BzzButton update_button,
    BzzButton map_button, BzzButton bee_button, BzzSwarm *swarm, BzzStationaries *stationaries, FlowersDataset fl,
    int inner_layers_count, int inner_layers[MAX_INNER_LAYERS])
{
    const size_t max_epoch = 200 * 1000;
    const size_t epochs_per_frame = 200;
    size_t epoch = 0;
    const float rate = 0.7f;
    int total_layers_count = 2+inner_layers_count; 

    Region temp = regionAllocAlloc(8 * KILO * KILO);
    Mat t = flowersToMat(fl);

    size_t *arch = NNMalloc(sizeof(size_t) * total_layers_count);
    for (int i = 0; i < total_layers_count; ++i) {
        if (i == 0) {
            arch[i] = fl.cols-1;
            continue;
        }
        if (i == total_layers_count-1) {
            arch[i] = 1;
            continue;
        }
        arch[i] = inner_layers[i-1];
    }

    NN nn = nnAlloc(NULL, arch, total_layers_count);
    nnRand(nn, -1, 1);
    BzzPlot plot = {0};

    BzzBeeGame bee = {
        .max_epoch = max_epoch,
        .epochs_per_frame = epochs_per_frame,
        .epoch = epoch,
        .rate = rate,
        .inner_layers_count = inner_layers_count,
        .paused = true,
        .reset = false,
        .modified = false,
        .temp = temp,
        .fl = fl,
        .t = t,
        .nn = nn,
        .plot = plot,
        .font = font,
        .minus_button = minus_button,
        .plus_button = plus_button,
        .learn_button = learn_button,
        .update_button = update_button,
        .map_button = map_button,
        .bee_button = bee_button,
        .swarm = swarm,
        .stationaries = stationaries
    };

    for (int i = 0; i < MAX_INNER_LAYERS; i++) {
        bee.inner_layers[i] = inner_layers[i];
    }

    return bee;
}

void bzzBzzBeeGameFree(BzzBeeGame *bee) 
{
    if (!bee) {
        return;
    }

    bee->max_epoch = 0;
    bee->epochs_per_frame = 0;
    bee->epoch = 0;
    bee->rate = 0.0f;
    bee->inner_layers_count = 0;
    bee->paused = true;
    bee->reset = false;
    bee->modified = false;
    regionFree(&bee->temp);
    flowersDatasetFree(&bee->fl);
    matFree(&bee->t);
    nnFree(&bee->nn);
    bzzPlotFree(&bee->plot);
}

bool isModified(BzzBeeGame *bee)
{
    return  bee->modified;
}
