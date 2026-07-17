cmake_minimum_required(VERSION 3.12)
project(CubicBattle)

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. СКАЧИВАНИЕ RAYLIB
include(FetchContent)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        5.0
)

# Настройки для Raylib перед загрузкой
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

if(ANDROID)
    set(PLATFORM "Android" CACHE STRING "" FORCE)
endif()

FetchContent_MakeAvailable(raylib)

# 2. СКАЧИВАНИЕ И СБОРКА LUA
# Lua не имеет встроенного CMake, поэтому мы создаем его сами "на лету"
FetchContent_Declare(
  lua_source
  URL https://www.lua.org/ftp/lua-5.3.6.tar.gz
)

FetchContent_GetProperties(lua_source)
if(NOT lua_source_POPULATED)
    FetchContent_Populate(lua_source)
    
    # Собираем список всех исходников Lua, кроме интерпретатора и компилятора
    file(GLOB LUA_SOURCES "${lua_source_SOURCE_DIR}/src/*.c")
    list(REMOVE_ITEM LUA_SOURCES 
        "${lua_source_SOURCE_DIR}/src/lua.c" 
        "${lua_source_SOURCE_DIR}/src/luac.c"
    )
    
    # Создаем статическую библиотеку Lua
    add_library(lua STATIC ${LUA_SOURCES})
    target_include_directories(lua PUBLIC "${lua_source_SOURCE_DIR}/src")
    
    # Для Android нужно разрешить динамическую загрузку библиотек внутри Lua
    if(ANDROID)
        target_compile_definitions(lua PRIVATE LUA_USE_LINUX)
    endif()
endif()

# 3. ОСНОВНОЙ ПРОЕКТ
# Если мы собираем под Android, это должна быть БИБЛИОТЕКА (SHARED)
# Если под ПК — ИСПОЛНЯЕМЫЙ ФАЙЛ (EXECUTABLE)
if(ANDROID)
    add_library(${PROJECT_NAME} SHARED main.cpp)
else()
    add_executable(${PROJECT_NAME} main.cpp)
endif()

# 4. ПОДКЛЮЧЕНИЕ БИБЛИОТЕК
# Связываем наш проект с Raylib и Lua
target_link_libraries(${PROJECT_NAME} raylib lua)

# 5. СПЕЦИФИКАЦИИ ДЛЯ ANDROID
if(ANDROID)
    # Эти библиотеки обязательны для работы Raylib на Android
    target_link_libraries(${PROJECT_NAME} android log OpenSLES EGL GLESv2)
    
    # Указываем, где искать заголовочные файлы
    target_include_directories(${PROJECT_NAME} PRIVATE 
        ${raylib_SOURCE_DIR}/src 
        ${lua_source_SOURCE_DIR}/src
    )
endif()

# 6. РЕСУРСЫ ДЛЯ ПК (чтобы logic.lua был рядом с .exe)
if(NOT ANDROID)
    file(COPY logic.lua DESTINATION ${CMAKE_BINARY_DIR})
endif()
