cmake_minimum_required(VERSION 3.12)
project(CubicBattle)

set(CMAKE_CXX_STANDARD 17)

# Использовать статический C++ рантайм (важно для Android)
set(ANDROID_STL c++_static)

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

FetchContent_Declare(
  lua_source
  URL https://www.lua.org/ftp/lua-5.3.6.tar.gz
)
FetchContent_GetProperties(lua_source)
if(NOT lua_source_POPULATED)
    FetchContent_Populate(lua_source)
    file(GLOB LUA_SOURCES "${lua_source_SOURCE_DIR}/src/*.c")
    list(REMOVE_ITEM LUA_SOURCES "${lua_source_SOURCE_DIR}/src/lua.c" "${lua_source_SOURCE_DIR}/src/luac.c")
    add_library(lua STATIC ${LUA_SOURCES})
    target_include_directories(lua PUBLIC "${lua_source_SOURCE_DIR}/src")
endif()

if(ANDROID)
    add_library(${PROJECT_NAME} SHARED main.cpp)
    target_link_libraries(${PROJECT_NAME} raylib lua android log OpenSLES EGL GLESv2)
else()
    add_executable(${PROJECT_NAME} main.cpp)
    target_link_libraries(${PROJECT_NAME} raylib lua)
endif()
