
#ifndef PATH_CALCULATOR_H__
#define PATH_CALCULATOR_H__

#include <stdint.h>

#include "../../utils/utils.h"

#define TABLE_SIZE_X_STEPS MM_TO_STEPS(TABLE_DIM_X_MM)
#define TABLE_SIZE_Y_STEPS MM_TO_STEPS(TABLE_DIM_Y_MM)

typedef struct move_one_instruction {
    uint16_t steps_1;
    uint8_t dir_1;
    uint16_t steps_2;
    uint8_t dir_2;
} move_one_instruction_t;

void set_target_pos_steps(unsigned int target_x, unsigned int target_y);
bool at_target();
void get_motor_movement_instructions(move_one_instruction_t *move_instr);
void set_target_position_gcode(const char *instr);

extern unsigned int current_pos_x;
extern unsigned int current_pos_y;

#endif
