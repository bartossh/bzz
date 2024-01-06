#ifndef PAGE_H
#define PAGE_H

#include <stdbool.h>
#include "raylib.h"
#include "../nn/gym.h"
#include "../nn/nn.h"
#include "../flowers/flowers.h"

typedef struct {
    Texture2D logo;
} PageMain;

/// page_nn_new return new PageMain.
///
PageMain page_main_new(const char *path);

// draw_main_page draws main page in to the screen.
void draw_main_page(PageMain m, Font font);

void cleanup_main_page(PageMain m);

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
    Gym_Plot plot;
} PageBee;

/// page_nn_new return new PageBee.
///
PageBee page_nn_new(void);

/// draw_nn_page draws PageNN in to the screen.
///
void draw_nn_page(PageBee *bee, Font font);

#endif // !PAGE_H


