/// Copyright (c) 2024 Bartosz Lenart

#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "raylib.h"
#include "views/view.h"

bool paused = true;
char screen = 'M';

int main(void)
{
    srand(time(NULL));
    size_t WindowFactor = 100;
    size_t WindowWidth = (16*WindowFactor);
    size_t WindowHeight = (9*WindowFactor);

    InitWindow(WindowWidth, WindowHeight, "BZZ!");

    Font font = LoadFontEx("./fonts/Anonymous.ttf", 60, NULL, 0);
    
    ViewManu m = viewManuNew("./artefacts/logo.png", font);
    ViewBee bee = viewBeeNew(font);

    SetTargetFPS(80);

    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_B)) {
            screen = 'B';
        }
        
        if (IsKeyPressed(KEY_M)) {
            screen = 'M';
        }

        switch (screen) {
        case 'M':
            drawManuView(m);
            break;
        case 'B':
        default:
            if (IsKeyPressed(KEY_SPACE)) {
                bee.paused = !bee.paused;
            }

            bee.reset = false;
            if (IsKeyPressed(KEY_R)) {
                bee.reset = true;
            }

            drawBeeView(&bee);
            break;
        }
    }
    cleanupManuView(m);
    CloseWindow();

    return 0;
}


