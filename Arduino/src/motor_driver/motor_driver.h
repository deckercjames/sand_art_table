
#ifndef MOTOR_DRIVER_H__
#define MOTOR_DRIVER_H__

void init_motors();

void register_carriage();

void set_new_target(unsigned int target_x, unsigned int target_y);
bool at_target();
void move_toward_target();

#endif //ADD_H__
