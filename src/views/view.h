#ifndef VIEW_H
#define VIEW_H

#include <stdbool.h>
#include "raylib.h"
#include "../nn/gym.h"
#include "../nn/nn.h"
#include "../flowers/flowers.h"

typedef struct {
    Texture2D logo;
} ViewMain;

/// viewBeeNew return new ViewMain.
///
ViewMain viewMainNew(const char *path);

// drawMainView draws main page in to the screen.
void drawMainView(ViewMain m, Font font);

void cleanupMainView(ViewMain m);

/// PageNN holds all the data required to properly update the nn page view.
typedef struct {
    size_t max_epoch;
    size_t epochs_per_frame;
    size_t epoch; 
    float rate;
    bool paused;
    bool reset;
    Region temp;
    FlowersDataset fl;
    Mat t;
    NN nn;
    GymPlot plot;
} ViewBee;

/// viewBeeNew return new ViewBee.
///
ViewBee viewBeeNew(void);

/// drawBeeView draws PageNN in to the screen.
///
void drawBeeView(ViewBee *bee, Font font);

#endif


