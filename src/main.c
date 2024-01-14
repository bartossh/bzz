/// Copyright (c) 2024 Bartosz Lenart

#include "raylib.h"
#include "views/views.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

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
    
    Font font = LoadFontEx("./fonts/Anonymous.ttf", 60, NULL, 0);
    
    BzzButton minus_button = bzzButtonNewMinus(0.05f, BLACK);
    BzzButton plus_button = bzzButtonNewPlus(0.05f, BLACK);
    BzzButton bee_button = bzzButtonNewBee(0.09f, ORANGE);
    BzzButton map_button = bzzButtonNewMap(0.09f, ORANGE);
    BzzButton learn_button = bzzButtonNewLearn(0.09f, ORANGE);
    BzzButton update_button = bzzButtonNewUpdate(0.09f, ORANGE);
    BzzButton logo_button = bzzButtonNewLogo(1.0f, ORANGE);

    ViewMenu m = viewMenuNew(font, logo_button);
    BeeParams bee = viewBeeNew(
        font, minus_button, plus_button, learn_button, update_button, map_button, bee_button, 
        inner_layers_count, inner_layers
    );

    SetTargetFPS(70);
    
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
                    inner_layers_count, bee.inner_layers
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
    CloseWindow();
    
    return 0;
}
