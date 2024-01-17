/// Copyright (c) 2024 Bartosz Lenart

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "levels/levels.h"
#include "game/game.h"

bool paused = true;
ScreenView screen = MainMenuScreen;
int starting_number_of_bees = 10;
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

    FlowersDataset fl = flowersDatasetNew(Location_6_60);
    
    Font font = LoadFontEx("./fonts/Anonymous.ttf", 60, NULL, 0);
    
    BzzButton minus_button = bzzButtonNewMinus(0.05f, BLACK);
    BzzButton plus_button = bzzButtonNewPlus(0.05f, BLACK);
    BzzButton bee_button = bzzButtonNewBee(0.09f, ORANGE);
    BzzButton map_button = bzzButtonNewMap(0.09f, ORANGE);
    BzzButton learn_button = bzzButtonNewLearn(0.09f, ORANGE);
    BzzButton update_button = bzzButtonNewUpdate(0.09f, ORANGE);
    BzzButton logo_button = bzzButtonNewLogo(1.0f, ORANGE);
    BzzObject bee_object = bzzObjectNewBee(ORANGE);

    int total_flowers_tx = bzzGetTotalNumberOfAvaliablefFlowersTextures();
    BzzStationaries stationaries = bzzStationariesNew();
    for (int i = 0; i < total_flowers_tx; i++) {
        BzzObject o = bzzObjectNewFlower(WHITE, i);
        Vector2 pos = {.x = randInRange(0.0f, (float)w), .y = randInRange(0.0f, (float)h)};
        BzzStationary f = bzzStationaryNewFlower(o, pos, 0.08f);
        bool ok = bzzStationariesAppend(&stationaries, f);
        if (!ok) {
            exit(1);
        }
    }

    int stationaries_size = bzzStationariesGetSize(&stationaries);
    BzzSwarm swarm = bzzSwarmNew();
    for (int i = 0; i < starting_number_of_bees; i++) {
        BzzStationary *s = bzzStationariesAt(&stationaries, (int)randInRange(0.0f, (float)stationaries_size));
        BzzAnimated bee_movable = bzzAnimatedNewBee(
            bee_object,
            CLITERAL(Vector2){.x = w/2, .y = h/2},
            CLITERAL(Vector2){.x = s->pos.x+s->obj.tx.width*s->scale, .y = s->pos.y+s->obj.tx.height*s->scale},
            TopDown
        );
        bzzSwarmAppend(&swarm, bee_movable);
    }

    ViewMenu m = viewMenuNew(font, logo_button);
    BzzBeeGame bee = bzzBzzBeeGameNew(
        font, minus_button, plus_button, learn_button, update_button, map_button, bee_button, 
        &swarm, &stationaries, fl, inner_layers_count, inner_layers
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
                bzzBzzBeeGameFree(&bee);
                bee = bzzBzzBeeGameNew(
                    font, minus_button, plus_button, learn_button, update_button, map_button, bee_button, 
                    &swarm, &stationaries, fl, inner_layers_count, bee.inner_layers
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
    bzzUnloadObject(bee_object);
    CloseWindow();
    
    return 0;
}
