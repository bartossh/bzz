/// Copyright (c) 2024 Bartosz Lenart

#include "views.h"
#include "raylib.h"
#include "../assets/assets_loader.h"

ViewMenu viewMenuNew(Font font, GymButton bee_button)
{
    ViewMenu pm = {
        .logo = assetLoad(Logo),
        .font = font,
        .bee_button = bee_button,
    };

    return pm;
}

void renderMenuView(ViewMenu m, char *screen)
{
    if (!screen) {
        exit(1);
        return;
    }
    int w = GetScreenWidth();

    BeginDrawing(); 
        ClearBackground(OCEAN);
        DrawTextEx(m.font, "BZZ - train your bees to cope in a world full of dangers.",
                   CLITERAL(Vector2){.x = 200, .y = 120 }, 30, 0, ORANGE);
        int pressed = gymRenderButton(m.bee_button, CLITERAL(Vector2){.x = w - 50 , .y = 20 });
        if (pressed) {
           *screen = 'B';  
        }
        DrawTextureV(m.logo, CLITERAL(Vector2){.x = 550, .y = 150}, ORANGE); 
    EndDrawing();
}

void cleanupMenuView(ViewMenu m)
{
    UnloadTexture(m.logo);
}
