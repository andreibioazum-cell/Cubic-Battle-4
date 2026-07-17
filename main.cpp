#include "raylib.h"
#include <stddef.h> // Нужно для Lua

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

// Безопасная загрузка Lua из ассетов Android
bool LoadLuaFromAssets(lua_State* L, const char* fileName) {
    int dataSize = 0;
    unsigned char* fileData = LoadFileData(fileName, &dataSize);
    
    if (fileData == NULL) {
        TraceLog(LOG_ERROR, "LUA: Не удалось найти файл %s", fileName);
        return false;
    }

    if (luaL_loadbuffer(L, (const char*)fileData, dataSize, fileName) || lua_pcall(L, 0, 0, 0)) {
        TraceLog(LOG_ERROR, "LUA ERROR: %s", lua_tostring(L, -1));
        UnloadFileData(fileData);
        return false;
    }

    UnloadFileData(fileData);
    TraceLog(LOG_INFO, "LUA: Файл %s успешно загружен", fileName);
    return true;
}

int main() {
    // На Android Raylib сам подстроит разрешение
    InitWindow(800, 450, "Cubic Battle");
    SetTargetFPS(60);

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    bool luaLoaded = LoadLuaFromAssets(L, "logic.lua");

    float rotation = 0.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        rotation += 90.0f * dt;

        float r = 1, g = 0, b = 0; // Цвета по умолчанию

        if (luaLoaded) {
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
        ClearBackground(RAYWHITE);
        
        float size = GetScreenWidth() / 4.0f;
        Rectangle rec = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f, size, size };
        Vector2 origin = { size / 2.0f, size / 2.0f };
        
        DrawRectanglePro(rec, origin, rotation, cubeColor);
        DrawText("Raylib + Lua Android", 10, 10, 20, MAROON);
        
        EndDrawing();
    }

    lua_close(L);
    CloseWindow();
    return 0;
}
