
#ifndef PATH_CALCULATOR_H__
#define PATH_CALCULATOR_H__

#include <stdint.h>

#include "utils.h"

#define TABLE_SIZE_X_STEPS MM_TO_STEPS(TABLE_DIM_X_MM)
#define TABLE_SIZE_Y_STEPS MM_TO_STEPS(TABLE_DIM_Y_MM)

uint16_t get_current_pos_x();
uint16_t get_current_pos_y();

void set_target_pos_steps(uint16_t target_x_steps, uint16_t target_y_steps);
void path_calculator_reset_target();
bool path_calculator_at_target();
move_instr_t path_calculator_get_motor_movement_instruction();

#endif
