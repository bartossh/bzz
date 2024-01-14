/// Copyright (c) 2024 Bartosz Lenart

#include <float.h>
#include <raylib.h>
#include <raymath.h>
#include <string.h>
#include "raylib.h"
#include "views.h"
#include "../assets/assets_loader.h"

GymButton gymButtonNewMinus(float scale, Color color)
{
    GymButton btn = {
        .tx = assetLoad(MinusButton),
        .color = color,
        .scale = scale,
    };

    return btn;
}

GymButton gymButtonNewPlus(float scale, Color color)
{
    GymButton btn = {
        .tx = assetLoad(PlusButton),
        .color = color,
        .scale = scale,
    };

    return btn;
}


GymButton gymButtonBee(float scale, Color color)
{
    GymButton btn = {
        .tx = assetLoad(BeeButton),
        .color = color,
        .scale = scale,
    };

    return btn;
}

GymButton gymButtonMap(float scale, Color color)
{
    GymButton btn = {
        .tx = assetLoad(MapButton),
        .color = color,
        .scale = scale,
    };

    return btn;
}

int gymRenderButton(GymButton btn, Vector2 pos)
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

void gymUnloadButton(GymButton btn)
{
    UnloadTexture(btn.tx);  
}

