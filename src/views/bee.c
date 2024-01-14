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
    
    Rectangle mvbBounds = { pos.x, pos.y, width, height };
    Vector2 mousePoint = GetMousePosition();
    int result = 0;
    if (CheckCollisionPointRec(mousePoint, mvbBounds)) {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            result++;
        }    
    }
    Rectangle dstRec =  {pos.x, pos.y, width, height};
    Rectangle frameRec = { 0.0f, 0.0f, width, height };
    frameRec.y = (float)mvb->frame*height;
    Vector2 org = {.x = 0, .y = 0};
    //DrawTextureRec(mvb->tx, frameRec, pos, mvb->color);
    DrawTexturePro(mvb->tx, frameRec, dstRec, org, rotation, mvb->color);
    mvb->frame++;
    return result;
}

void bzzUnloadMovable(BzzMovable mvb)
{
    UnloadTexture(mvb.tx);  
}
