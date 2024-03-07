
#include "path_calculator.h"

#include <string.h>
#include <stdio.h>

#include "../../utils/utils.h"
#include "../../config/config.h"

// Redefine the stepper motor directions for implementation
// Don't include the arduino header so we can unit test easier
#define FORWARD 1
#define BACKWARD 2

#define NA 0

#define MM_TO_STEPS(mills) (((mills * STEPPER_MOTOR_STEPS) / BELT_TEETH_SPACING_MM) / TIMING_PULLY_TEETH)

#define TABLE_SIZE_X_STEPS MM_TO_STEPS(TABLE_DIM_X_MM)
#define TABLE_SIZE_Y_STEPS MM_TO_STEPS(TABLE_DIM_Y_MM)

// Global Variables
unsigned int current_pos_x = 0;
unsigned int current_pos_y = 0;

unsigned int target_pos_x;
unsigned int target_pos_y;

int delta_x;
int delta_y;

int sign_delta_x;
int sign_delta_y;

int error;

void set_target_pos(unsigned int target_x, unsigned int target_y)
{
    target_pos_x = min(target_x, TABLE_DIM_X_MM);
    target_pos_y = min(target_y, TABLE_DIM_Y_MM);
    
    // set x variables
    if (target_pos_x > current_pos_x) {
        delta_x = target_pos_x - current_pos_x;
        sign_delta_x = 1;
    } else {
        delta_x = current_pos_x - target_pos_x;
        sign_delta_x = -1;
    }
    
    // set y variables
    if (target_pos_y > current_pos_y) {
        delta_y = target_pos_y - current_pos_y;
        sign_delta_y = 1;
    } else {
        delta_y = current_pos_y - target_pos_y;
        sign_delta_y = -1;
    }
    delta_y = -delta_y;
    
    error = delta_x + delta_y;
}

void set_target_position_gcode(const char *instr)
{
    
}

bool at_target()
{
    return (current_pos_x == target_pos_x) && (current_pos_y == target_pos_y);
}

const move_one_instruction_t movement_lookups[] = {
    // X step = -1
    {0, NA,       2, BACKWARD}, // Y step = -1   [down left]
    {1, BACKWARD, 1, BACKWARD}, // Y step =  0   [left]
    {2, BACKWARD, 0, NA      }, // Y step =  1   [up left]
    // X step = 0
    {1, FORWARD,  1, BACKWARD}, // Y step = -1   [down]
    {0, NA,       1, NA      }, // No Move: N/A
    {1, BACKWARD, 1, FORWARD }, // Y step =  1   [up]
    // X step = 1
    {2, FORWARD,  0, NA      }, // Y step = -1   [down right]
    {1, FORWARD,  1, FORWARD }, // Y step =  0   [right]
    {0, NA,       2, FORWARD }, // Y step =  1   [up right]
};

void get_motor_movement_instructions(move_one_instruction_t *move_instr)
{
    memset(move_instr, 0, sizeof(move_one_instruction_t));
    
    if (at_target()) {
        return;
    }
    
    int error_double = 2 * error;
    
    int step_x = 0;
    int step_y = 0;
    
    if (error_double >= delta_y) {
        if (current_pos_x == target_pos_x) {
            return;
        }
        error += delta_y;
        step_x = sign_delta_x;
    }
    
    if (error_double <= delta_x) {
        if (current_pos_y == target_pos_y) {
            return;
        }
        error += delta_x;
        step_y = sign_delta_y;
    }
    
    // Update the current position
    current_pos_x += step_x;
    current_pos_y += step_y;
    
    // Copy move instructions from the table
    int lookup_idx = ((step_x + 1) * 3) + (step_y + 1);
    
    memcpy(move_instr, &movement_lookups[lookup_idx], sizeof(move_one_instruction_t));
}