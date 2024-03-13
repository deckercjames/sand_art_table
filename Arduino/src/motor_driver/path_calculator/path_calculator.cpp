
#include "path_calculator.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../utils/utils.h"
#include "../../config/config.h"

// Redefine the stepper motor directions for implementation
// Don't include the arduino header so we can unit test easier
#define FORWARD 1
#define BACKWARD 2

#define NA 0

#define MM_TO_STEPS(mills) (((mills * STEPPER_MOTOR_STEPS) / BELT_TEETH_SPACING_MM) / TIMING_PULLY_TEETH)
#define UM100_TO_STEPS(um100s) ((((um100s * STEPPER_MOTOR_STEPS) / BELT_TEETH_SPACING_MM) / TIMING_PULLY_TEETH) / 10)

#define TABLE_SIZE_X_STEPS MM_TO_STEPS(TABLE_DIM_X_MM)
#define TABLE_SIZE_Y_STEPS MM_TO_STEPS(TABLE_DIM_Y_MM)

// Global Variables
unsigned int current_pos_x = 0; // measured in steps
unsigned int current_pos_y = 0;

unsigned int target_pos_x; // measured in steps
unsigned int target_pos_y;

int delta_x;
int delta_y;

int sign_delta_x;
int sign_delta_y;

int error;

void set_target_pos_steps(unsigned int target_x, unsigned int target_y)
{
    target_pos_x = min(target_x, TABLE_SIZE_X_STEPS);
    target_pos_y = min(target_y, TABLE_SIZE_Y_STEPS);
    
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

enum gcode_parse_state  {
    GCODE_STATE_PENDING_INSTR,
    GCODE_STATE_PARSE_INSTR_NUMBER,
    GCODE_STATE_PARSE_INSTR_COMPLETE,
    GCODE_STATE_PENDING_ARG,
    GCODE_STATE_PARSE_ARG,
    GCODE_STATE_PROCESS_ARG,
    GCODE_STATE_SKIP_ARG,
};

void set_target_position_gcode(const char *instr)
{
    int idx = 0;
    char c;
    char current_arg_id; // The arg that is currently being parsed. Either 'X' or 'Y'
    char current_arg_val[10];
    int current_arg_val_idx;
    
    unsigned int new_target_x = target_pos_x;
    unsigned int new_target_y = target_pos_y;
    
    enum gcode_parse_state state = GCODE_STATE_PENDING_INSTR;
    
    // printf("\nhere\n");
    
    while (1)
    {
        c = instr[idx];
        if (c == ';') {
            break;
        }
        
        // printf("state = %d; [%c]\n", state, c);

        switch (state)
        {
            case GCODE_STATE_PENDING_INSTR:
                if (c == ' ') {
                    break;
                }
                if (c != 'G') {
                    return;
                }
                state = GCODE_STATE_PARSE_INSTR_NUMBER;
                break;
            case GCODE_STATE_PARSE_INSTR_NUMBER:
                if (c == '0') {
                    break;
                }
                if (c == '1') {
                    state = GCODE_STATE_PARSE_INSTR_COMPLETE;
                    break;
                }
                return;
            case GCODE_STATE_PARSE_INSTR_COMPLETE:
                if (c != ' ') {
                    return;
                }
                state = GCODE_STATE_PENDING_ARG;
                break;
            case GCODE_STATE_PENDING_ARG:
                if (c == ' ') {
                    break;
                }
                if (c == 'X' || c == 'Y') {
                    state = GCODE_STATE_PARSE_ARG;
                    current_arg_id = c;
                    current_arg_val_idx = 0;
                    break;
                }
                state = GCODE_STATE_SKIP_ARG;
                break;
            case GCODE_STATE_PARSE_ARG:
                if (c == ' ' || c == '\0') {
                    state = GCODE_STATE_PROCESS_ARG;
                    continue;
                }
                if (current_arg_val_idx == sizeof(current_arg_val) - 1) {
                    state = GCODE_STATE_SKIP_ARG;
                    break;
                }
                current_arg_val[current_arg_val_idx++] = c;
                break;
            case GCODE_STATE_PROCESS_ARG:
            {
                current_arg_val[current_arg_val_idx] = '\0';
                // printf("Processing Arg %c '%s'\n", c, current_arg_val);
                float f = atof(current_arg_val);
                f *= 10;
                f = max(f, 0.0f);
                if (current_arg_id == 'X') {
                    new_target_x = UM100_TO_STEPS((unsigned int) f);
                    // printf("Setting x to f%f --> %d\n", f, target_pos_x);
                } else if (current_arg_id == 'Y') {
                    new_target_y = UM100_TO_STEPS((unsigned int) f);
                    // printf("Setting y to f%f --> %d\n", f, target_pos_y);
                }
                state = GCODE_STATE_PENDING_ARG;
                break;
            }
            case GCODE_STATE_SKIP_ARG:
                if (c != ' ') {
                    break;
                }
                state = GCODE_STATE_PENDING_ARG;
                break;
        }
        
        if (c == '\0') {
            break;
        }

        idx++;
    }
    
    set_target_pos_steps(new_target_x, new_target_y);
}

bool at_target()
{
    // printf("(%u, %u) :: (%u, %u)\n", current_pos_x, current_pos_y, target_pos_x, target_pos_y);
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