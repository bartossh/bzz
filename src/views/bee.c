/// Copyright (c) 2024 Bartosz Lenart

#include <raylib.h>
#include "raylib.h"
#include "views.h"
#include "../assets/assets_loader.h"

BzzMovable bzzMovableNewBee(Color color)
{
    BzzMovable btn = {
        .tx = assetLoad(BeeFlying),
        .color = color,
        .frame = 0,
        .total_frames = 4
    };

    return btn;
}

int bzzRenderMovable(BzzMovable *mvb, Vector2 pos, float rotation)
{
    if (mvb->frame == mvb->total_frames) {
        mvb->frame = 0;
    }

    float width = mvb->tx.width;
    float height = mvb->tx.height / mvb->total_frames;
    float x = pos.x-width/2;
    float y = pos.y+height/2;
    
    Rectangle mvbBounds = {x, y-height, width, height};
    Vector2 mousePoint = GetMousePosition();
    int result = 0;
    float resize = 1.0f;
    if (CheckCollisionPointRec(mousePoint, mvbBounds)) {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            result++;
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            resize = 0.8;
        }
    }
    Rectangle dstRec =  {x, y, width*resize, height*resize};
    Rectangle frameRec = {0.0f, 0.0f, width, height};
    frameRec.y = ((float)mvb->frame)*height;
    Vector2 org = {.x = 0.0f, .y = 0.0f};
    DrawTexturePro(mvb->tx, frameRec, dstRec, org, rotation, mvb->color);
    mvb->frame++;
    return result;
}

void bzzUnloadMovable(BzzMovable mvb)
{
    UnloadTexture(mvb.tx);  
}
