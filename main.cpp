#include "raylib.h"
#include <iostream>
#include <string>

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

// Функция для загрузки Lua скрипта из ресурсов (работает на Android и ПК)
bool LoadLuaFromAssets(lua_State* L, const char* fileName) {
    int dataSize = 0;
    unsigned char* fileData = LoadFileData(fileName, &dataSize);
    if (fileData == NULL) return false;

    int result = luaL_loadbuffer(L, (const char*)fileData, dataSize, fileName) || lua_pcall(L, 0, 0, 0);
    UnloadFileData(fileData);

    if (result != LUA_OK) {
        TraceLog(LOG_ERROR, "LUA ERROR: %s", lua_tostring(L, -1));
        return false;
    }
    return true;
}

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    // На Android RayLib игнорирует размеры и делает Fullscreen
    InitWindow(screenWidth, screenHeight, "RayLib + Lua: Rotating 2D Cube");
    SetTargetFPS(60);

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // Загружаем через RayLib-функцию (важно для Android!)
    if (!LoadLuaFromAssets(L, "logic.lua")) {
        TraceLog(LOG_ERROR, "Failed to load logic.lua");
    }

    float rotation = 0.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        rotation += 90.0f * dt;

        lua_getglobal(L, "GetNextColor");
        lua_pushnumber(L, dt);
        
        if (lua_pcall(L, 1, 3, 0) != LUA_OK) {
            TraceLog(LOG_ERROR, "LUA CALL ERROR: %s", lua_tostring(L, -1));
        }

        float r = (float)lua_tonumber(L, -3);
        float g = (float)lua_tonumber(L, -2);
        float b = (float)lua_tonumber(L, -1);
        lua_pop(L, 3);

        Color cubeColor = { (unsigned char)(r * 255), (unsigned char)(g * 255), (unsigned char)(b * 255), 255 };

        BeginDrawing();
        ClearBackground(RAYWHITE);

        Rectangle rec = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f, 150.0f, 150.0f };
        Vector2 origin = { 75.0f, 75.0f };
        
        DrawRectanglePro(rec, origin, rotation, cubeColor);
        DrawText("Logic by Lua, Render by RayLib", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    lua_close(L);
    CloseWindow();
    return 0;
}
