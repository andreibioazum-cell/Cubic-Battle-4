#include "raylib.h"
#include <stddef.h>

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

int main() {
    // 1. Инициализация
    InitWindow(0, 0, "Cubic Battle 4"); // 0, 0 заставит Raylib взять весь экран
    SetTargetFPS(60);

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // 2. Безопасная загрузка Lua
    bool luaActive = false;
    int dataSize = 0;
    unsigned char* fileData = LoadFileData("logic.lua", &dataSize);
    
    if (fileData != NULL) {
        if (luaL_loadbuffer(L, (const char*)fileData, dataSize, "logic.lua") == LUA_OK) {
            if (lua_pcall(L, 0, 0, 0) == LUA_OK) {
                luaActive = true;
            }
        }
        UnloadFileData(fileData);
    }

    float rotation = 0.0f;

    // 3. Главный цикл (на Android он должен работать, пока окно активно)
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        rotation += 90.0f * dt;

        float r = 0.5f, g = 0.5f, b = 0.5f; // Серый цвет, если Lua не работает

        if (luaActive) {
            lua_getglobal(L, "GetNextColor");
            lua_pushnumber(L, dt);
            if (lua_pcall(L, 1, 3, 0) == LUA_OK) {
                r = (float)lua_tonumber(L, -3);
                g = (float)lua_tonumber(L, -2);
                b = (float)lua_tonumber(L, -1);
                lua_pop(L, 3);
            }
        }

        Color cubeColor = { (unsigned char)(r * 255), (unsigned char)(g * 255), (unsigned char)(b * 255), 255 };

        BeginDrawing();
            ClearBackground(BLACK);
            
            int size = GetScreenWidth() / 3;
            if (size == 0) size = 200; // Защита, если экран еще не определился
            
            Rectangle rec = { (float)GetScreenWidth()/2, (float)GetScreenHeight()/2, (float)size, (float)size };
            Vector2 origin = { (float)size/2, (float)size/2 };
            
            DrawRectanglePro(rec, origin, rotation, cubeColor);
            
            if (!luaActive) {
                DrawText("LUA NOT LOADED", 10, 80, 40, RED);
            }
            DrawText("RayLib Android Works!", 10, 30, 40, GREEN);
        EndDrawing();
    }

    lua_close(L);
    CloseWindow();
    return 0;
}
