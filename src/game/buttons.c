/// Copyright (c) 2024 Bartosz Lenart

#include <raylib.h>
#include "raylib.h"
#include "game.h"
#include "../assets/assets_loader.h"

BzzButton bzzButtonNewMinus(float scale, Color color)
{
    BzzButton btn = {
        .tx = assetLoad(MinusButton),
        .color = color,
        .scale = scale,
    };

    return btn;
}

BzzButton bzzButtonNewPlus(float scale, Color color)
{
    BzzButton btn = {
        .tx = assetLoad(PlusButton),
        .color = color,
        .scale = scale,
    };

    return btn;
}


BzzButton bzzButtonNewBee(float scale, Color color)
{
    BzzButton btn = {
        .tx = assetLoad(BeeButton),
        .color = color,
        .scale = scale,
    };

    return btn;
}

BzzButton bzzButtonNewMap(float scale, Color color)
{
    BzzButton btn = {
        .tx = assetLoad(MapButton),
        .color = color,
        .scale = scale,
    };

    return btn;
}

BzzButton bzzButtonNewLearn(float scale, Color color)
{
    BzzButton btn = {
        .tx = assetLoad(LearnButton),
        .color = color,
        .scale = scale,
    };

    return btn;
}

BzzButton bzzButtonNewUpdate(float scale, Color color)
{
    BzzButton btn = {
        .tx = assetLoad(UpdateButton),
        .color = color,
        .scale = scale,
    };

    return btn;
}

BzzButton bzzButtonNewLogo(float scale, Color color)
{
    BzzButton btn = {
        .tx = assetLoad(Logo),
        .color = color,
        .scale = scale,
    };

    return btn;
}


int bzzRenderButton(BzzButton btn, Vector2 pos)
{ 
    float width = btn.tx.width * btn.scale;
    float height = btn.tx.height *btn.scale;
    
    Rectangle btnBounds = { pos.x, pos.y, width, height };
    Vector2 mousePoint = GetMousePosition();

    float resize = 1.0f;
    int result = 0;

    if (CheckCollisionPointRec(mousePoint, btnBounds)) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            resize = 0.9;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            result++;
        }    
    }

    Vector2 p = { .x = pos.x + (width - width * resize)/2.0f, .y = pos.y + (height - height * resize)/2.0f };
    
    DrawTextureEx(btn.tx, p, 0.0f, btn.scale*resize, btn.color);

    return result;
}

void bzzUnloadButton(BzzButton btn)
{
    UnloadTexture(btn.tx);  
}

