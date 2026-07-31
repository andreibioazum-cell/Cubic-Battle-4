// Округление FPS
#include <math.h>
int round_fps(float fps) {
return (int)roundf(fps);
}
// Исправление залипания стика джойстика
void update_player_movement(Player* player, float dt) {
// Получение значений стика джойстика
float stick_x = player->input.stick_x;
float stick_y = player->input.stick_y;
// Проверка, находится ли стик внутри кольца
float stick_distance = sqrt(stick_x * stick_x + stick_y * stick_y);
if (stick_distance <= 1.0f) {
// Обновление позиции игрока
player->position.x += stick_x * player->speed * dt;
player->position.y += stick_y * player->speed * dt;
} else {
// Стик находится за пределами кольца, ограничиваем его значение
stick_x = stick_x / stick_distance;
stick_y = stick_y / stick_distance;
player->position.x += stick_x * player->speed * dt;
player->position.y += stick_y * player->speed * dt;
}
}