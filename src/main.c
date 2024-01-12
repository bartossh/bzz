/// Copyright (c) 2024 Bartosz Lenart

#include "raylib.h"
#include "views/views.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

bool paused = true;
char screen = 'M';

int main(void) 
{
    srand(time(NULL));
    
    size_t WindowFactor = 100;
    size_t WindowWidth = (16 * WindowFactor);
    size_t WindowHeight = (9 * WindowFactor);
    
    InitWindow(WindowWidth, WindowHeight, "BZZ!");
    
    Font font = LoadFontEx("./fonts/Anonymous.ttf", 60, NULL, 0);
    
    GymButton minus_button = gymButtonNewMinus(0.05f, BLACK);
    GymButton plus_button = gymButtonNewPlus(0.05f, BLACK);

    ViewMenu m = viewMenuNew(font);
    ViewBee bee = viewBeeNew(font, minus_button, plus_button);

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
            drawMenuView(m);
            break;
        case 'B':
        default:
            if (IsKeyPressed(KEY_SPACE)) {
                bee.paused = !bee.paused;
            }
        
            if (bee.paused && IsKeyPressed(KEY_L)) {
                Font font = bee.font;
                viewBeeFree(&bee);
                bee = viewBeeNew(font, minus_button, plus_button);
            }
        
            bee.reset = false;
            if (IsKeyPressed(KEY_R)) {
                bee.reset = true;
            }
        
            drawBeeView(&bee);
            break;
        }
    }
    cleanupMenuView(m);
    UnloadFont(font);
    gymUnloadButton(minus_button);
    gymUnloadButton(plus_button);
    CloseWindow();
    
    return 0;
}
