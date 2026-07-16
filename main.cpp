#include "raylib.h"
#include <iostream>

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

int main() {
    // Инициализация окна RayLib
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "RayLib + Lua: Rotating 2D Cube");
    SetTargetFPS(60);

    // Инициализация Lua
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    if (luaL_dofile(L, "logic.lua") != LUA_OK) {
        std::cerr << "LUA ERROR: " << lua_tostring(L, -1) << std::endl;
        return -1;
    }

    float rotation = 0.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        rotation += 90.0f * dt; // Вращение 90 градусов в секунду

        // Вызов Lua функции для получения цвета
        lua_getglobal(L, "GetNextColor");
        lua_pushnumber(L, dt);
        
        if (lua_pcall(L, 1, 3, 0) != LUA_OK) {
            std::cerr << "LUA CALL ERROR: " << lua_tostring(L, -1) << std::endl;
        }

        float r = (float)lua_tonumber(L, -3);
        float g = (float)lua_tonumber(L, -2);
        float b = (float)lua_tonumber(L, -1);
        lua_pop(L, 3);

        // Преобразование 0.0-1.0 в 0-255 для RayLib
        Color cubeColor = { (unsigned char)(r * 255), (unsigned char)(g * 255), (unsigned char)(b * 255), 255 };

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Рисуем вращающийся 2D "кубик" (квадрат)
        // DrawRectanglePro требует Rectangle, Vector2 (центр вращения) и угол
        Rectangle rec = { screenWidth / 2.0f, screenHeight / 2.0f, 150.0f, 150.0f };
        Vector2 origin = { 75.0f, 75.0f }; // Центр квадрата (150/2)
        
        DrawRectanglePro(rec, origin, rotation, cubeColor);
        
        DrawText("Logic by Lua, Render by RayLib", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    lua_close(L);
    CloseWindow();

    return 0;
}
