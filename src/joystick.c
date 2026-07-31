#include "joystick.h"
void update_joystick_position(struct Joystick *joystick, int x, int y) {
joystick->x = x;
joystick->y = y;
if (sqrt(x*x + y*y) > joystick->radius) {
double angle = atan2(y, x);
joystick->x = joystick->radius * cos(angle);
joystick->y = joystick->radius * sin(angle);
}
}