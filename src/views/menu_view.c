/// Copyright (c) 2024 Bartosz Lenart

#include "view.h"
#include "raylib.h"
#include "../assets/assets_loader.h"

const char menu[] = 
" MENU: \n"
" KEY B - Train your Bee.\n"
" KEY M - Main manu.\n";

ViewManu viewManuNew(Font font)
{
    ViewManu pm = {
        .logo = assetLoad(Logo),
        .font = font,
    };

    return pm;
}

void drawManuView(ViewManu m)
{
    BeginDrawing(); 
        ClearBackground(OCEAN);
        DrawTextEx(m.font, "YELLOW FROM BZZ GAME." ,CLITERAL(Vector2){.x = 50, .y = 20}, 50, 0, ORANGE);
        DrawTextEx(m.font, menu, CLITERAL(Vector2){.x = 80, .y = 100}, 30, 0, ORANGE);
        DrawTextureV(m.logo, CLITERAL(Vector2){.x = 550, .y = 150}, ORANGE); 
    EndDrawing();
}


void cleanupManuView(ViewManu m)
{
    UnloadTexture(m.logo);
}
