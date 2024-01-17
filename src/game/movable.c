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

inline static void nextPosition(BzzAnimated *b, Vector2 next)
{
    if (vector2Equals(b->pos, b->target) && b->pause_time) {
        b->pause_time--;
        return;
    } else if (vector2Equals(b->pos, b->target)) {
        float speed = randInRange(1.0f, 3.0f);
        b->speed = speed;
        b->pause_time = PAUSE_TIME; 
        b->target.x = next.x;
        b->target.y = next.y;
    }

    b->pos = Vector2MoveTowards(b->pos, b->target, b->speed);
}

inline static void calcDirection(BzzAnimated *b)
{
    if (vector2Equals(b->pos, b->target) && b->pause_time) { 
       b->dir += randInRange(-5.0f, 5.0f);
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

BzzAnimated bzzAnimatedNewBee(BzzObject obj, Vector2 start, Vector2 next, AnimationLayout l)
{
    float speed = randInRange(1.0f, 3.0f);
    BzzAnimated a = {
        .obj = obj,
        .layout = l,
        .frame = 0,
        .total_frames = 4,
        .pause_time = PAUSE_TIME,
        .speed = speed,
        .dir = 0.0f,
        .pos = start,
        .target = next
    };

    return a;
}

void bzzRenderAnimated(BzzAnimated *b, Vector2 next)
{
    if (!b) {
        exit(1);
    }

    if (b->frame == b->total_frames) {
        b->frame = 0;
    }

    nextPosition(b, next);
    calcDirection(b);
    float resize = calcResize(b);
    float width = b->obj.tx.width;
    float height = b->obj.tx.height / b->total_frames;
    float x = b->pos.x-width/2;
    float y = b->pos.y-height/2;
    
    float rot = 0.0f;
    switch (b->layout) {
    case TopDown:
        rot = - 90.0f;
        break;
    case LeftRight:
    default:
        rot = 0.0f;
    }

    Rectangle dstRec =  {x, y, width*resize, height*resize};
    Rectangle frameRec = {0.0f, 0.0f, width, height};
    frameRec.y = ((float)b->frame)*height;
    Vector2 org = {.x = 0.0f, .y = 0.0f};
    DrawTexturePro(b->obj.tx, frameRec, dstRec, org, b->dir+rot, b->obj.color);
    b->frame++;
}

int bzzCheckCollision(BzzAnimated *b, Rectangle r)
{
    (void) b;
    (void) r;
    return 0;
}
