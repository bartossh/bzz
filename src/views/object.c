/// Copyright (c) 2024 Bartosz Lenart

#include <raylib.h>
#include "views.h"
#include "../assets/assets_loader.h" 

BzzObject bzzObjectNewBee(Color color)
{
    BzzObject obj = {
        .tx = assetLoad(BeeFlying),
        .color = color
    };

    return obj;
}


void bzzUnloadObject(BzzObject obj)
{
    UnloadTexture(obj.tx);  
}
