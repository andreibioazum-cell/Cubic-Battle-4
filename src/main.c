// Использование округления FPS
#include "game.h"
int main() {
// Инициализация игры
Game game;
init_game(&game);
float target_fps = 60.0f;
float dt = 1.0f / target_fps;
while (game_is_running(&game)) {
float current_time = get_current_time();
float elapsed_time = current_time - game.last_frame_time;
game.last_frame_time = current_time;
// Округление FPS
int rounded_fps = round_fps(1.0f / elapsed_time);
printf("FPS: %d\n", rounded_fps);
update_player_movement(&game.player, dt);
update_game(&game, dt);
render_game(&game);
}
destroy_game(&game);
return 0;
}