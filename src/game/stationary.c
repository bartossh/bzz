/// Copyright (c) 2024 Bartosz Lenart

#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include "game.h"

BzzStationary bzzStationaryNewFlower(BzzObject obj, Vector2 pos, float scale)
{
    BzzStationary s = {
        .obj = obj,
        .pos = pos,
        .scale = scale,
    };

    return s;
}

int bzzRenderStationary(BzzStationary *s)
{
    if (!s) {
        exit(1);
    }
    
    float width = s->obj.tx.width * s->scale;
    float height = s->obj.tx.height * s->scale; 
    Rectangle btnBounds = {s->pos.x, s->pos.y, width, height};
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
    
    Vector2 pos = {.x = s->pos.x + (width - width * resize)/2.0f, .y = s->pos.y + (height - height * resize)/2.0f};
    
    DrawTextureEx(s->obj.tx, pos, 0.0f, s->scale*resize, s->obj.color);

    return result;
}

Vector2 bzzGetCenterStationary(BzzStationary *s)
{
    Vector2 pos = {
        .x = s->pos.x + s->obj.tx.width*s->scale/2,
        .y = s->pos.y + s->obj.tx.height*s->scale/2
    };
    return pos;
}
