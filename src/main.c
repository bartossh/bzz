/// Copyright (c) 2024 Bartosz Lenart

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "levels/levels.h"
#include "game/game.h"

//#define PLATFORM_WEB /// uncoment if compiling WASM or pass flag `-DPLATFORM_WEB`

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

void UpdateDrawFrame(float w, float h);

bool paused;
ScreenView screen = MainMenuScreen;
LevelsDataset fl;
ViewMenu m;
BzzBeeGame bee; 
BzzButton minus_button; 
BzzButton plus_button;
BzzButton bee_button; 
BzzButton map_button; 
BzzButton learn_button; 
BzzButton update_button;
BzzButton logo_button;
BzzObject bee_object; 
BzzSwarm swarm;
BzzStationaries stationaries;

const float padding = 50.0f;

inline static void prepareLevelStationaries(BzzStationaries *stationaries, LevelsDataset *dt, float w, float h)
{
    size_t total_flowers_tx = (size_t)bzzGetTotalNumberOfAvaliablefFlowersTextures();
    size_t size = levelsGetFlowersCount(dt);
    for (size_t i = 0; i < size && i < total_flowers_tx; i++) {
        BzzObject o = bzzObjectNewFlower(WHITE, i);
        Vector2 pos = {.x = randInRange(padding, (float)w-padding), .y = randInRange(padding, (float)h-padding)};
        BzzStationary f = bzzStationaryNewFlower(o, pos, 0.08f);
        bool ok = bzzStationariesAppend(stationaries, f);
        if (!ok) {
            exit(1);
        }
    }
}

inline static void prepareLevelBees(BzzSwarm *swarm, int starting_number_of_bees, float w, float h)
{
    for (int i = 0; i < starting_number_of_bees; i++) {
        BzzAnimated bee_movable = bzzAnimatedNewBee(
            bee_object,
            CLITERAL(Vector2){.x = w/2, .y = h/2},
            CLITERAL(Vector2){.x = randInRange(padding, w-padding), .y = randInRange(padding, h-padding)},
            TopDown
        );
        bzzSwarmAppend(swarm, bee_movable);
    }
}

int main(void) 
{
    srand(time(NULL));

    paused = true;
    int starting_number_of_bees = 10;
    int inner_layers_count = 1;
    int inner_layers[MAX_INNER_LAYERS] = {5};
    
    size_t WindowFactor = 80;
    size_t WindowWidth = (16 * WindowFactor);
    size_t WindowHeight = (9 * WindowFactor);
    
    InitWindow(WindowWidth, WindowHeight, "BZZ!");
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    
    fl = levelsDatasetNew(Basic_5_10);
    
    Font font = LoadFontEx("./fonts/Anonymous.ttf", 60, NULL, 0);
    
    minus_button = bzzButtonNewMinus(0.05f, BLACK);
    plus_button = bzzButtonNewPlus(0.05f, BLACK);
    bee_button = bzzButtonNewBee(0.09f, ORANGE);
    map_button = bzzButtonNewMap(0.09f, ORANGE);
    learn_button = bzzButtonNewLearn(0.09f, ORANGE);
    update_button = bzzButtonNewUpdate(0.09f, ORANGE);
    logo_button = bzzButtonNewLogo(1.0f, ORANGE);
    bee_object = bzzObjectNewBee(ORANGE);
    
    stationaries = bzzStationariesNew();
    prepareLevelStationaries(&stationaries, &fl, w, h);
    
    swarm = bzzSwarmNew();
    prepareLevelBees(&swarm, starting_number_of_bees, w, h);
    
    m = viewMenuNew(font, logo_button);
    bee = bzzBzzBeeGameNew(
        font, minus_button, plus_button, learn_button, update_button, map_button, bee_button, 
        &swarm, &stationaries, fl, inner_layers_count, inner_layers
    );
    
    SetTargetFPS(60);
    
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else

    while (!WindowShouldClose()) {  
        UpdateDrawFrame(w, h);
    }
#endif


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

void UpdateDrawFrame(float w, float h)
{   
    switch (screen) {
    case MainMenuScreen:
        renderMenuView(m, &screen);
        break;
    case BeeMapScreen:
        renderMapView(&bee, &screen, w, h, padding);
        break;
    case BeeTrainScreen:
    default: 
        if (bee.paused && isModified(&bee)) {
            Font font = bee.font;
            bzzBzzBeeGameClenup(&bee);
            bee = bzzBzzBeeGameNew(
                font, minus_button, plus_button, learn_button, update_button, map_button, bee_button, 
                &swarm, &stationaries, fl, bee.inner_layers_count, bee.inner_layers
            );
        }
        renderBeeView(&bee, &screen);
        break;
    }
}

