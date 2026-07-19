cmake_minimum_required(VERSION 3.12)
project(CubicBattle)

set(CMAKE_CXX_STANDARD 17)
set(ANDROID_STL c++_static)

# СКАЧИВАНИЕ RAYLIB
include(FetchContent)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        5.0
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

if(ANDROID)
    set(PLATFORM "Android" CACHE STRING "" FORCE)
    if(ANDROID_ABI STREQUAL "armeabi-v7a")
        add_definitions(-DSTBI_NO_SIMD -DSTBIR_NO_SIMD)
    endif()
endif()

FetchContent_MakeAvailable(raylib)

# СБОРКА ПРОЕКТА
if(ANDROID)
    add_library(${PROJECT_NAME} SHARED main.cpp)
    target_link_libraries(${PROJECT_NAME} raylib android log OpenSLES EGL GLESv2)
else()
    add_executable(${PROJECT_NAME} main.cpp)
    target_link_libraries(${PROJECT_NAME} raylib)
endif()
