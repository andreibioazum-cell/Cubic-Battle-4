#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
int main(int argc, char *argv[]) {
SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
SDL_Window *window = SDL_CreateWindow("Cubic Battle 4", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
SDL_Joystick *joystick = NULL;
if (SDL_NumJoysticks() > 0) {
joystick = SDL_JoystickOpen(0);
}
int fps = 60;
Uint32 last_time = SDL_GetTicks();
while (1) {
SDL_Event event;
while (SDL_PollEvent(&event)) {
if (event.type == SDL_QUIT) {
goto cleanup;
}
if (event.type == SDL_JOYBUTTONDOWN || event.type == SDL_JOYAXISMOTION) {
// Обработка событий джойстика
if (joystick) {
int x = SDL_JoystickGetAxis(joystick, 0);
int y = SDL_JoystickGetAxis(joystick, 1);
printf("Joystick: x=%d, y=%d\n", x, y);
}
}
}
Uint32 current_time = SDL_GetTicks();
float delta_time = (current_time - last_time) / 1000.0f;
last_time = current_time;
int rounded_fps = (int)(1.0f / delta_time + 0.5f);
printf("FPS: %d\n", rounded_fps);
// Остальной код игры...
}
cleanup:
if (joystick) {
SDL_JoystickClose(joystick);
}
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);
SDL_Quit();
return 0;
}