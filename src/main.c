/// Copyright (c) 2024 Bartosz Lenart

#include "raylib.h"
   
int main(void)
{
    InitWindow(1000, 650, "BZZzzzzz!");
    
    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(DARKGREEN);
            DrawText("Yellow from BZZ!", 10, 10, 30, ORANGE);
        EndDrawing();
    }
    
    CloseWindow();
    
    return 0;
}
