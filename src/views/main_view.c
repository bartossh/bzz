#include "view.h"
#include "raylib.h"

ViewMain viewMainNew(const char *path)
{
    Image image = LoadImage(path);
    Texture2D logo = LoadTextureFromImage(image); 
    UnloadImage(image);
    ViewMain pm = {
        .logo = logo,
    };

    return pm;
}

void drawMainView(ViewMain m, Font font)
{
    BeginDrawing(); 
        int h = GetScreenHeight();
        ClearBackground(DARKGREEN);
        DrawTextEx(font, "Yellow from Bzz!" ,CLITERAL(Vector2){.x = 200, .y = 20}, h*0.03, 0, ORANGE);
        DrawTextureV(m.logo, CLITERAL(Vector2){.x = 250, .y = 40}, ORANGE);
    EndDrawing();
}


void cleanupMainView(ViewMain m)
{
    UnloadTexture(m.logo);
}
