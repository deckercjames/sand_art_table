
#ifndef PATH_CALCULATOR_H__
#define PATH_CALCULATOR_H__

#include <stdint.h>

// Sand Table Configurations
#define TABLE_DIM_X_MM 10000
#define TABLE_DIM_Y_MM 10000

typedef struct move_one_instruction {
    uint16_t steps_1;
    uint8_t dir_1;
    uint16_t steps_2;
    uint8_t dir_2;
} move_one_instruction_t;

void pc_set_target_pos(unsigned int target_x, unsigned int target_y);
bool pc_at_target();
void pc_move_toward_target(move_one_instruction_t *move_instr);

extern unsigned int current_pos_x;
extern unsigned int current_pos_y;

#endif
