#include "raylib.h"

int main() {
    // Инициализация окна. 0, 0 автоматически подстроится под экран телефона
    InitWindow(0, 0, "RayLib Android - C++ Only");
    SetTargetFPS(60);

    float rotation = 0.0f;

    // Главный цикл
    while (!WindowShouldClose()) {
        // Логика
        float dt = GetFrameTime();
        rotation += 90.0f * dt;

        // Рисование
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            int screenW = GetScreenWidth();
            int screenH = GetScreenHeight();
            float cubeSize = screenW / 3.0f;
            
            // Рисуем квадрат в центре
            Rectangle rec = { screenW / 2.0f, screenH / 2.0f, cubeSize, cubeSize };
            Vector2 origin = { cubeSize / 2.0f, cubeSize / 2.0f };
            
            DrawRectanglePro(rec, origin, rotation, MAROON);
            
            DrawText("RayLib C++ is Running!", 20, 20, 20, DARKGRAY);
            DrawFPS(20, screenH - 40);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
