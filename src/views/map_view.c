/// Copyright (c) 2024 Bartosz Lenart

#include <math.h>
#include <raylib.h>
#include "raylib.h"
#include "views.h"

void renderMapView(BeeParams *bee, ScreenView *screen)
{
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    BeginDrawing();

    Vector2 min = {.x = 0.0f, .y = 0.0f};
    Vector2 max = {.x = (float)w, .y = (float)h};

    bzzRenderAnimated(&bee->bee_movable, min, max);

    ClearBackground(GRASS);
    int pressed = bzzRenderButton(bee->bee_button, CLITERAL(Vector2){.x = w - 50 , .y = 20});
    if (pressed) {
       *screen = BeeTrainScreen;  
    }
    EndDrawing();
}
