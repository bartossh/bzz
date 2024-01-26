/// Copyright (c) 2024 Bartosz Lenart

#include <math.h>
#include <raylib.h>
#include "raylib.h"
#include "game.h"

void renderMapView(BzzBeeGame *bp, ScreenView *screen, float w)
{
    BeginDrawing();
    ClearBackground(GRASS);
    

    int stationaries_size = bzzStationariesGetSize(bp->stationaries);
    for (int i = 0; i < stationaries_size; i++) {
        BzzStationary *s = bzzStationariesAt(bp->stationaries, i);
        bzzRenderStationary(s);
    }

    int swarm_size = bzzSwarmGetSize(bp->swarm); 
    for (int i = 0; i < swarm_size; i++) {
        BzzAnimated *bee = bzzSwarmAt(bp->swarm, i);
        bzzRenderAnimated(bee, bp->stationaries);
        if (bzzIsNewTargetAnimated(bee)) {
            bzzBzzBeeGameUpdateDiscovered(bp, bzzGetTargetIndexAnimated(bee));
        }
    }
 
    int pressed = bzzRenderButton(bp->bee_button, CLITERAL(Vector2){ .x = w - 50 , .y = 20 });
    if (pressed) {
       *screen = BeeTrainScreen;  
    }
    
    EndDrawing();
}
