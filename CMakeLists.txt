cmake_minimum_required(VERSION 3.12)
project(CubicBattle)

set(CMAKE_CXX_STANDARD 17)
set(ANDROID_STL c++_static)

# ВАЖНО: Устанавливаем платформу ДО того, как скачиваем RayLib
set(PLATFORM "Android" CACHE STRING "Raylib platform")
set(GRAPHICS "GRAPHICS_API_OPENGL_ES_20" CACHE STRING "Graphics API")

include(FetchContent)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        5.0
)

# Отключаем лишнее в RayLib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
set(USE_AUDIO      OFF CACHE BOOL "" FORCE) # Для теста отключим звук, чтобы не было лишних ошибок

FetchContent_MakeAvailable(raylib)

# СБОРКА НАШЕГО ПРОЕКТА
if(ANDROID)
    add_library(${PROJECT_NAME} SHARED main.cpp)
    # Принудительно добавляем макрос PLATFORM_ANDROID для нашего кода
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_ANDROID)
    target_link_libraries(${PROJECT_NAME} raylib android log EGL GLESv2)
else()
    add_executable(${PROJECT_NAME} main.cpp)
    target_link_libraries(${PROJECT_NAME} raylib)
endif()
