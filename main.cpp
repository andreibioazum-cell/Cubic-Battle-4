#include "raylib.h"

int main() {
    // На Android InitWindow игнорирует цифры и берет весь экран
    InitWindow(0, 0, "CubicBattle");
    
    // Ждем, пока RayLib действительно создаст контекст (важно для Android)
    while (!IsWindowReady()) {
        if (WindowShouldClose()) return 0;
    }

    SetTargetFPS(60);

    float rotation = 0.0f;

    while (!WindowShouldClose()) {
        rotation += 90.0f * GetFrameTime();

        BeginDrawing();
            ClearBackground(BLACK);
            
            float size = GetScreenWidth() / 3.0f;
            DrawRectanglePro(
                { GetScreenWidth()/2.0f, GetScreenHeight()/2.0f, size, size },
                { size/2.0f, size/2.0f },
                rotation,
                MAROON
            );
            
            DrawText("RayLib Android: SUCCESS", 20, 20, 30, GREEN);
            DrawFPS(20, GetScreenHeight() - 50);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
