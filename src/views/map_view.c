/// Copyright (c) 2024 Bartosz Lenart

#include <raylib.h>
#include "raylib.h"
#include "views.h"


void renderMapView(BeeParams *bee, ScreenView *screen)
{
    int w = GetScreenWidth();
    BeginDrawing(); 
    ClearBackground(GRASS);
    int pressed = bzzRenderButton(bee->bee_button, CLITERAL(Vector2){.x = w - 50 , .y = 20});
    if (pressed) {
       *screen = BeeTrainScreen;  
    }
    EndDrawing();
}
