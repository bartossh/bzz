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
    size_t WINDOW_FACTOR = 80;
    size_t WINDOW_WIDTH = (16*WINDOW_FACTOR);
    size_t WINDOW_HEIGHT = (9*WINDOW_FACTOR);



    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "BZZ!");

    PageMain m = page_main_new("./artefacts/logo.png");
    PageBee bee = page_nn_new();

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
            draw_main_page(m, font);
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

            draw_nn_page(&bee, font);
            break;
        }
    }
    cleanup_main_page(m);
    CloseWindow();

    return 0;
}


