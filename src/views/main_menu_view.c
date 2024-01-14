/// Copyright (c) 2024 Bartosz Lenart

#include "views.h"
#include "raylib.h"
#include "../assets/assets_loader.h"

ViewMenu viewMenuNew(Font font, GymButton logo_button)
{
    ViewMenu pm = {
        .font = font,
        .logo_button = logo_button
    };

    return pm;
}

void renderMenuView(ViewMenu m, ScreenView *screen)
{
    if (!screen) {
        exit(1);
        return;
    }
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    float logo_x = w/2 - m.logo_button.tx.width * m.logo_button.scale/2;
    float logo_y = h/2 - m.logo_button.tx.height * m.logo_button.scale/2;
    BeginDrawing(); 
        ClearBackground(OCEAN);
        DrawTextEx(m.font, "BZZ - train your bees to cope in a world full of dangers.",
                   CLITERAL(Vector2){.x = 200, .y = 120 }, 30, 0, ORANGE);
        int pressed = gymRenderButton(m.logo_button, CLITERAL(Vector2){.x = logo_x , .y = logo_y });
        if (pressed) {
           *screen = BeeMapScreen;  
        }
    EndDrawing();
}
