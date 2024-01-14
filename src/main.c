/// Copyright (c) 2024 Bartosz Lenart

#include "raylib.h"
#include "views/views.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

bool paused = true;
char screen = 'M';
int inner_layers_count = 1;
int inner_layers[MAX_INNER_LAYERS] = {5};

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
    GymButton bee_button = gymButtonBee(0.09f, ORANGE);
    GymButton map_button = gymButtonMap(0.09f, ORANGE);

    ViewMenu m = viewMenuNew(font, bee_button);
    ViewBee bee = viewBeeNew(font, minus_button, plus_button, map_button, inner_layers_count, inner_layers);

    SetTargetFPS(70);
    
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    
    while (!WindowShouldClose()) {  
        switch (screen) {
        case 'M':
            renderMenuView(m, &screen);
            break;
        case 'B':
        default:
            if (IsKeyPressed(KEY_SPACE)) {
                bee.paused = !bee.paused;
            }
        
            if (bee.paused && isModified(&bee)) {
                inner_layers_count = bee.inner_layers_count;
                Font font = bee.font;
                viewBeeFree(&bee);
                bee = viewBeeNew(font, minus_button, plus_button, map_button, inner_layers_count, bee.inner_layers);
            }
        
            bee.reset = false;
            if (IsKeyPressed(KEY_R)) {
                bee.reset = true;
            }
        
            renderBeeView(&bee, &screen);
            break;
        }
    }
    cleanupMenuView(m);
    UnloadFont(font);
    gymUnloadButton(minus_button);
    gymUnloadButton(plus_button);
    gymUnloadButton(bee_button);
    gymUnloadButton(map_button);
    CloseWindow();
    
    return 0;
}
