
#ifndef PATH_CALCULATOR_H__
#define PATH_CALCULATOR_H__

#include <stdint.h>

#include "../../utils/utils.h"

#define TABLE_SIZE_X_STEPS MM_TO_STEPS(TABLE_DIM_X_MM)
#define TABLE_SIZE_Y_STEPS MM_TO_STEPS(TABLE_DIM_Y_MM)

void set_target_pos_steps(uint16_t target_x_steps, uint16_t target_y_steps);
bool at_target();
uint8_t get_motor_movement_instruction();

extern unsigned int current_pos_x;
extern unsigned int current_pos_y;

#endif
