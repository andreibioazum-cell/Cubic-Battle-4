cmake_minimum_required(VERSION 3.10)
project(CubicBattle)

set(CMAKE_CXX_STANDARD 17)

# Настройки Raylib
set(PLATFORM "Desktop" CACHE STRING "Platform to build for")
if(ANDROID)
    set(PLATFORM "Android")
endif()

# Скачиваем Raylib через FetchContent (автоматизация)
include(FetchContent)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        5.0
)
FetchContent_MakeAvailable(raylib)

# Скачиваем Lua
FetchContent_Declare(
  lua
  URL https://www.lua.org/ftp/lua-5.3.6.tar.gz
)
FetchContent_GetProperties(lua)
if(NOT lua_POPULATED)
    FetchContent_Populate(lua)
    # Минимальная сборка Lua из исходников
    file(GLOB LUA_SOURCES "${lua_SOURCE_DIR}/src/*.c")
    list(REMOVE_ITEM LUA_SOURCES "${lua_SOURCE_DIR}/src/lua.c" "${lua_SOURCE_DIR}/src/luac.c")
    add_library(lua STATIC ${LUA_SOURCES})
    target_include_directories(lua PUBLIC "${lua_SOURCE_DIR}/src")
endif()

# Основное приложение
add_executable(${PROJECT_NAME} main.cpp)

# Подключаем библиотеки
target_link_libraries(${PROJECT_NAME} raylib lua)

# Для Android добавляем необходимые системные библиотеки
if(PLATFORM STREQUAL "Android")
    target_link_libraries(${PROJECT_NAME} android log OpenSLES EGL GLESv2)
endif()

# Копируем logic.lua в папку с билдом
file(COPY logic.lua DESTINATION ${CMAKE_BINARY_DIR})
