#include "raylib.h"
#include <stddef.h>

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

int main() {
    // 1. Инициализация логов
    SetTraceLogLevel(LOG_ALL);
    TraceLog(LOG_INFO, "APP: Запуск CubicBattle4");

    // 2. Инициализация окна (Raylib на Android сам выберет Fullscreen)
    InitWindow(0, 0, "CubicBattle4");
    
    // Ждем, пока окно действительно инициализируется
    while (!IsWindowReady()) {
        // На некоторых телефонах нужно подождать пару кадров
        if (WindowShouldClose()) return 0; 
    }

    TraceLog(LOG_INFO, "APP: Окно готово: %dx%d", GetScreenWidth(), GetScreenHeight());

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // Безопасная загрузка Lua
    bool luaActive = false;
    int dataSize = 0;
    unsigned char* fileData = LoadFileData("logic.lua", &dataSize);
    
    if (fileData != NULL) {
        if (luaL_loadbuffer(L, (const char*)fileData, dataSize, "logic.lua") == LUA_OK) {
            if (lua_pcall(L, 0, 0, 0) == LUA_OK) {
                luaActive = true;
                TraceLog(LOG_INFO, "LUA: Скрипт успешно запущен");
            } else {
                TraceLog(LOG_ERROR, "LUA: Ошибка вызова: %s", lua_tostring(L, -1));
            }
        } else {
            TraceLog(LOG_ERROR, "LUA: Ошибка компиляции: %s", lua_tostring(L, -1));
        }
        UnloadFileData(fileData);
    } else {
        TraceLog(LOG_WARNING, "LUA: Файл logic.lua не найден в assets");
    }

    float rotation = 0.0f;

    // 3. Главный цикл
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        rotation += 90.0f * dt;

        float r = 0.1f, g = 0.5f, b = 0.9f; // Цвет по умолчанию (синеватый)

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

        BeginDrawing();
            ClearBackground(BLACK);
            
            int size = GetScreenWidth() / 3;
            if (size < 100) size = 100;
            
            Rectangle rec = { (float)GetScreenWidth()/2.0f, (float)GetScreenHeight()/2.0f, (float)size, (float)size };
            Vector2 origin = { (float)size/2.0f, (float)size/2.0f };
            
            DrawRectanglePro(rec, origin, rotation, { (unsigned char)(r*255), (unsigned char)(g*255), (unsigned char)(b*255), 255 });
            
            DrawText("RayLib Android OK", 20, 40, 30, RAYWHITE);
            if (!luaActive) DrawText("LUA ERROR", 20, 80, 30, RED);
            else DrawText("LUA OK", 20, 80, 30, GREEN);
            
            DrawFPS(20, GetScreenHeight() - 40);
        EndDrawing();
    }

    TraceLog(LOG_INFO, "APP: Завершение работы");
    lua_close(L);
    CloseWindow();
    return 0;
}
