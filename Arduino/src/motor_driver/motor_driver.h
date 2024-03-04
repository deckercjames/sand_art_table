
#ifndef MOTOR_DRIVER_H__
#define MOTOR_DRIVER_H__

bool init_motors();

bool register_carriage();

void set_target_pos(unsigned int target_x, unsigned int target_y);
bool at_target();
void move_toward_target();

#endif //ADD_H__
