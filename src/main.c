#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
int main(int argc, char *argv[]) {
SDL_Init(SDL_INIT_VIDEO);
SDL_Window *window = SDL_CreateWindow("Cubic Battle 4", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
int fps = 60;
Uint32 last_time = SDL_GetTicks();
while (1) {
Uint32 current_time = SDL_GetTicks();
float delta_time = (current_time - last_time) / 1000.0f;
last_time = current_time;
// Обновление игровой логики
// ...
// Отрисовка
SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
SDL_RenderClear(renderer);
// ...
SDL_RenderPresent(renderer);
// Ограничение FPS
int frame_time = (1000 / fps) - (SDL_GetTicks() - current_time);
if (frame_time > 0) {
SDL_Delay(frame_time);
}
}
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);
SDL_Quit();
return 0;
}