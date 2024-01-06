#include "view.h"
#include "raylib.h"

PageMain page_main_new(const char *path)
{
    Image image = LoadImage(path);
    Texture2D logo = LoadTextureFromImage(image);      // Image converted to texture, uploaded to GPU memory (VRAM)
    UnloadImage(image);
    PageMain pm = {
        .logo = logo,
    };

    return pm;
}

void draw_main_page(PageMain m, Font font)
{
    BeginDrawing(); 
        int h = GetScreenHeight();
        ClearBackground(DARKGREEN);
        DrawTextEx(font, "Welcome from bzz!" ,CLITERAL(Vector2){.x = 30, .y = 30}, h*0.02, 0, ORANGE);
        DrawTextureEx(m.logo, CLITERAL(Vector2){.x = 50, .y = 50}, 0.0f, h*0.4, DARKGREEN);
    EndDrawing();
}
