/// Copyright (c) 2024 Bartosz Lenart

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "views/views.h"

bool paused = true;
ScreenView screen = MainMenuScreen;
int inner_layers_count = 1;
int inner_layers[MAX_INNER_LAYERS] = {5};

int main(void) 
{
    srand(time(NULL));
    
    size_t WindowFactor = 100;
    size_t WindowWidth = (16 * WindowFactor);
    size_t WindowHeight = (9 * WindowFactor);
    
    InitWindow(WindowWidth, WindowHeight, "BZZ!");
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    
    Font font = LoadFontEx("./fonts/Anonymous.ttf", 60, NULL, 0);
    
    BzzButton minus_button = bzzButtonNewMinus(0.05f, BLACK);
    BzzButton plus_button = bzzButtonNewPlus(0.05f, BLACK);
    BzzButton bee_button = bzzButtonNewBee(0.09f, ORANGE);
    BzzButton map_button = bzzButtonNewMap(0.09f, ORANGE);
    BzzButton learn_button = bzzButtonNewLearn(0.09f, ORANGE);
    BzzButton update_button = bzzButtonNewUpdate(0.09f, ORANGE);
    BzzButton logo_button = bzzButtonNewLogo(1.0f, ORANGE);
    BzzAnimated bee_movable = bzzAnimatedNewBee(
        ORANGE, CLITERAL(Vector2){.x = w/2, .y = h/2},
        CLITERAL(Vector2){.x = 0.0f, .y = 0.0f}, 
        CLITERAL(Vector2){.x = (float)w, .y = (float)h},
        TopDown
    );

    ViewMenu m = viewMenuNew(font, logo_button);
    BeeParams bee = viewBeeNew(
        font, minus_button, plus_button, learn_button, update_button, map_button, bee_button, 
        bee_movable, inner_layers_count, inner_layers
    );

    SetTargetFPS(60);
    
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    
    while (!WindowShouldClose()) {  
        switch (screen) {
        case MainMenuScreen:
            renderMenuView(m, &screen);
            break;
        case BeeMapScreen:
            renderMapView(&bee, &screen);
            break;
        case BeeTrainScreen:
        default: 
            if (bee.paused && isModified(&bee)) {
                inner_layers_count = bee.inner_layers_count;
                Font font = bee.font;
                viewBeeFree(&bee);
                bee = viewBeeNew(
                    font, minus_button, plus_button, learn_button, update_button, map_button, bee_button, 
                    bee_movable, inner_layers_count, bee.inner_layers
                );
            }
            renderBeeView(&bee, &screen);
            break;
        }
    }
    UnloadFont(font);
    bzzUnloadButton(minus_button);
    bzzUnloadButton(plus_button);
    bzzUnloadButton(bee_button);
    bzzUnloadButton(map_button);
    bzzUnloadButton(learn_button);
    bzzUnloadButton(update_button);
    bzzUnloadButton(logo_button);
    bzzUnloadAnimated(bee_movable);
    CloseWindow();
    
    return 0;
}
