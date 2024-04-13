
#ifndef MOTOR_DRIVER_H__
#define MOTOR_DRIVER_H__

#include "path_calculator/path_calculator.h"

bool init_motors();
void release_motors();

bool register_carriage();

bool thermo_throtle_motors(float temp_f);

void set_target_pos(unsigned int target_x, unsigned int target_y);
bool at_target();
void move_toward_target();

#endif //ADD_H__
