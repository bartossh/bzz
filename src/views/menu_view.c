#include "view.h"
#include "raylib.h"

const char menu[] = 
" MENU: \n"
" KEY B - Train your Bee.\n"
" KEY M - Main manu.\n";

ViewManu viewManuNew(const char *path)
{
    Image image = LoadImage(path);
    Texture2D logo = LoadTextureFromImage(image); 
    UnloadImage(image);
    ViewManu pm = {
        .logo = logo,
    };

    return pm;
}

void drawManuView(ViewManu m, Font font)
{
    BeginDrawing(); 
        ClearBackground(OCEAN);
        DrawTextEx(font, "YELLOW FROM BZZ GAME." ,CLITERAL(Vector2){.x = 50, .y = 20}, 50, 0, ORANGE);
        DrawTextEx(font, menu, CLITERAL(Vector2){.x = 80, .y = 100}, 30, 0, ORANGE);
        DrawTextureV(m.logo, CLITERAL(Vector2){.x = 550, .y = 150}, ORANGE); 
    EndDrawing();
}


void cleanupManuView(ViewManu m)
{
    UnloadTexture(m.logo);
}
