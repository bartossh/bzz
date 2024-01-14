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
    
    GymButton minus_button = gymButtonNewMinus(0.05f, BLACK);
    GymButton plus_button = gymButtonNewPlus(0.05f, BLACK);
    GymButton bee_button = gymButtonNewBee(0.09f, ORANGE);
    GymButton map_button = gymButtonNewMap(0.09f, ORANGE);
    GymButton learn_button = gymButtonNewLearn(0.09f, ORANGE);
    GymButton update_button = gymButtonNewUpdate(0.09f, ORANGE);
    GymButton logo_button = gymButtonNewLogo(1.0f, ORANGE);

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
    gymUnloadButton(minus_button);
    gymUnloadButton(plus_button);
    gymUnloadButton(bee_button);
    gymUnloadButton(map_button);
    gymUnloadButton(learn_button);
    gymUnloadButton(update_button);
    gymUnloadButton(logo_button);
    CloseWindow();
    
    return 0;
}
