/// Copyright (c) 2024 Bartosz Lenart

#include <math.h>
#include <raylib.h>
#include "raylib.h"
#include "game.h"

void renderMapView(BzzBeeGame *bp, ScreenView *screen)
{
    int w = GetScreenWidth();
    BeginDrawing();

    ClearBackground(GRASS);
    

    int stationaries_size = bzzStationariesGetSize(bp->stationaries);
    for (int i = 0; i < stationaries_size; i++) {
        BzzStationary *s = bzzStationariesAt(bp->stationaries, i);
        bzzRenderStationary(s);
    }

    int swarm_size = bzzSwarmGetSize(bp->swarm); 
    for (int i = 0; i < swarm_size; i++) {
        BzzStationary *s = bzzStationariesAt(bp->stationaries, (int)randInRange(0.0f, (float)stationaries_size));
        BzzAnimated *bee = bzzSwarmAt(bp->swarm, i);
        bzzRenderAnimated(bee, CLITERAL(Vector2){.x = s->pos.x+s->obj.tx.width*s->scale, .y = s->pos.y+s->obj.tx.height*s->scale});
    }
 
    int pressed = bzzRenderButton(bp->bee_button, CLITERAL(Vector2){.x = w - 50 , .y = 20});
    if (pressed) {
       *screen = BeeTrainScreen;  
    }
    
    EndDrawing();
}
