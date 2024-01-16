/// Copyright (c) 2024 Bartosz Lenart

#include <math.h>
#include <raylib.h>
#include "raylib.h"
#include "views.h"

void renderMapView(BeeParams *bp, ScreenView *screen)
{
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    BeginDrawing();

    ClearBackground(GRASS);
    
    Vector2 min = {.x = 0.0f, .y = 0.0f};
    Vector2 max = {.x = (float)w, .y = (float)h};

    int swarm_size = bzzSwarmGetSize(bp->swarm); 
    for (int i = 0; i < swarm_size; i++) {
        BzzAnimated *bee = bzzSwarmAt(bp->swarm, i);
        bzzRenderAnimated(bee, min, max);
    }
 
    int pressed = bzzRenderButton(bp->bee_button, CLITERAL(Vector2){.x = w - 50 , .y = 20});
    if (pressed) {
       *screen = BeeTrainScreen;  
    }
    EndDrawing();
}
