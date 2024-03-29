
#ifndef MOTOR_DRIVER_H__
#define MOTOR_DRIVER_H__

#include "path_calculator/path_calculator.h"

bool init_motors();
void release_motors();

bool register_carriage();

void set_target_pos(unsigned int target_x, unsigned int target_y);
bool at_target();
void move_toward_target(const move_one_instruction_t *move_instr);

#endif //ADD_H__
