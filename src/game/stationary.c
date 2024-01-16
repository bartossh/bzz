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

void bzzRenderStationary(BzzStationary *s)
{
    if (!s) {
        exit(1);
    }

    Vector2 pos = {.x = s->pos.x - s->obj.tx.width/2*s->scale, .y = s->pos.x - s->obj.tx.height/2*s->scale};

    DrawTextureEx(s->obj.tx, pos, 0.0f, s->scale, s->obj.color);
}
