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
    size_t WindowFactor = 80;
    size_t WindowWidth = (16*WindowFactor);
    size_t WindowHeight = (9*WindowFactor);



    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WindowWidth, WindowHeight, "BZZ!");

    ViewMain m = viewMainNew("./artefacts/logo.png");
    ViewBee bee = viewBeeNew();

    SetTargetFPS(60);

    Font font = LoadFontEx("./fonts/Anonymous.ttf", 60, NULL, 0);
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
            drawMainView(m, font);
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

            drawBeeView(
                    &bee, font);
            break;
        }
    }
    cleanupMainView(m);
    CloseWindow();

    return 0;
}


