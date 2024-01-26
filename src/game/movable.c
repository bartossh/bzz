/// Copyright (c) 2024 Bartosz Lenart

#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include "game.h"

#define PAUSE_TIME 120

inline static bool vector2Equals(Vector2 a, Vector2 b)
{
    return a.x == b.x && a.y == b.y;
}

inline static void nextPosition(BzzAnimated *b, BzzStationaries *s)
{
    if (vector2Equals(b->pos, b->target) && b->pause_time) {
        b->pause_time--;
        if (b->pause_time == PAUSE_TIME) {
            b->is_new_trg = true;
        }
        return;
    } else if (vector2Equals(b->pos, b->target)) {
        int s_size = bzzStationariesGetSize(s);
        float speed = randInRange(1.5f, 2.0f);
        b->trg_idx = (int)randInRange(0.0f, (float)s_size);
        BzzStationary* fl = bzzStationariesAt(s, b->trg_idx);
        b->target = bzzGetCenterStationary(fl);
        b->speed = speed; 
        b->pause_time = PAUSE_TIME;
    }

    b->pos = Vector2MoveTowards(b->pos, b->target, b->speed);
}

inline static void calcDirection(BzzAnimated *b)
{
    if (vector2Equals(b->pos, b->target) && b->pause_time) { 
       b->dir += randInRange(-10.0f, 10.0f);
       return ;
    }
    b->dir = Vector2Angle(b->pos, b->target);
}

inline static float calcResize(BzzAnimated *b)
{
    if (vector2Equals(b->pos, b->target) && b->pause_time) { 
       return 0.85;
    }
    return 1.0f;
}

BzzAnimated bzzAnimatedNewBee(BzzObject obj, Vector2 start, Vector2 next, int idx, AnimationLayout l)
{
    float speed = randInRange(1.0f, 3.0f);
    BzzAnimated a = {
        .obj = obj,
        .layout = l,
        .is_new_trg = false,
        .frame = 0,
        .total_frames = 4,
        .pause_time = PAUSE_TIME,
        .trg_idx = idx,
        .speed = speed,
        .dir = 0.0f,
        .pos = start,
        .target = next
    };

    return a;
}

bool bzzIsNewTargetAnimated(BzzAnimated* b)
{
    if (!b) {
        return false;
    }
    bool result = b->is_new_trg;
    b->is_new_trg = false;
    return result;
}

int bzzGetTargetIndexAnimated(BzzAnimated* b)
{
    return b->trg_idx;
}

void bzzRenderAnimated(BzzAnimated *b, BzzStationaries *s)
{
    if (!b) {
        exit(1);
    }

    if (b->frame == b->total_frames) {
        b->frame = 0;
    }

    nextPosition(b, s);
    calcDirection(b);
    float resize = calcResize(b); 
    float rot = 0.0f;
    switch (b->layout) {
    case TopDown:
        rot = - 90.0f;
        break;
    case LeftRight:
    default:
        rot = 0.0f;
    }

    float width = b->obj.tx.width;
    float height = b->obj.tx.height / b->total_frames;
    float x = b->pos.x;
    float y = b->pos.y;

    Rectangle dstRec =  {x, y, width*resize, height*resize};
    Rectangle frameRec = {0.0f, 0.0f, width, height};
    frameRec.y = ((float)b->frame)*height;
    Vector2 org = {.x = width/2, .y = height/2};
    DrawTexturePro(b->obj.tx, frameRec, dstRec, org, b->dir+rot, b->obj.color);
    b->frame++;
}

int bzzCheckCollision(BzzAnimated *b, Rectangle r)
{
    (void) b;
    (void) r;
    return 0;
}
