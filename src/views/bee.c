 /// Copyright (c) 2024 Bartosz Lenart

#include <raylib.h>
#include "raylib.h"
#include "views.h"
#include "../assets/assets_loader.h"

BzzMovable bzzMovableNewBee(float scale, Color color)
{
    BzzMovable btn = {
        .tx = assetLoad(BeeFlying),
        .color = color,
        .scale = scale,
        .frame = 0,
        .total_frames = 4
    };

    return btn;
}

void bzzUnloadMovable(BzzMovable mvb)
{
    UnloadTexture(mvb.tx);  
}
