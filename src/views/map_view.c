/// Copyright (c) 2024 Bartosz Lenart

#include <math.h>
#include <raylib.h>
#include "raylib.h"
#include "views.h"

//inline static float randInRange(float a, float b)
//{
//    float r = (float)rand() / (float)RAND_MAX;
//    return a + r * (b - a); 
//}

void renderMapView(BeeParams *bee, ScreenView *screen)
{
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    BeginDrawing();

    bzzRenderMovable(&bee->bee_movable, CLITERAL(Vector2){.x = w/2, .y = h/2}, 270.0f);

    ClearBackground(GRASS);
    int pressed = bzzRenderButton(bee->bee_button, CLITERAL(Vector2){.x = w - 50 , .y = 20});
    if (pressed) {
       *screen = BeeTrainScreen;  
    }
    EndDrawing();
}
