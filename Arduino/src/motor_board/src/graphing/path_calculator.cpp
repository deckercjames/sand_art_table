
#include "path_calculator.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "logging.h"

// Redefine the stepper motor directions for implementation
// Don't include the arduino header so we can unit test easier
#define FORWARD 1
#define BACKWARD 2

// Global Variables
static uint16_t current_pos_x = 0; // measured in steps
static uint16_t current_pos_y = 0;

static uint16_t target_pos_x; // measured in steps
static uint16_t target_pos_y;

static int delta_x;
static int delta_y;

static int sign_delta_x;
static int sign_delta_y;

static int error;

uint16_t get_current_pos_x()
{
    return current_pos_x;
}

uint16_t get_current_pos_y()
{
    return current_pos_y;
}

void set_target_pos_steps(uint16_t target_x, uint16_t target_y)
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

void path_calculator_reset_target()
{
    set_target_pos_steps(current_pos_x, current_pos_y);
}

bool path_calculator_at_target()
{
    return (current_pos_x == target_pos_x) && (current_pos_y == target_pos_y);
}

const move_instr_t movement_lookups[] = {
    // X step = -1
    MOVE_INSTR_DOWN_LEFT,  // Y step = -1   [down left]
    MOVE_INSTR_LEFT,       // Y step =  0   [left]
    MOVE_INSTR_UP_LEFT,    // Y step =  1   [up left]
    // X step = 0
    MOVE_INSTR_DOWN,       // Y step = -1   [down]
    MOVE_INSTR_NONE,       // No Move: N/A
    MOVE_INSTR_UP,         // Y step =  1   [up]
    // X step = 1
    MOVE_INSTR_DOWN_RIGHT, // Y step = -1   [down right]
    MOVE_INSTR_RIGHT,      // Y step =  0   [right]
    MOVE_INSTR_UP_RIGHT,   // Y step =  1   [up right]
};

move_instr_t path_calculator_get_motor_movement_instruction()
{
    if (path_calculator_at_target()) {
        return MOVE_INSTR_NONE;
    }

    int error_double = 2 * error;

    int step_x = 0;
    int step_y = 0;

    if (error_double >= delta_y) {
        if (current_pos_x == target_pos_x) {
            return MOVE_INSTR_NONE;
        }
        error += delta_y;
        step_x = sign_delta_x;
    }

    if (error_double <= delta_x) {
        if (current_pos_y == target_pos_y) {
            return MOVE_INSTR_NONE;
        }
        error += delta_x;
        step_y = sign_delta_y;
    }

    // Update the current position
    current_pos_x += step_x;
    current_pos_y += step_y;

    // lookup the movement instruction
    const int lookup_idx = ((step_x + 1) * 3) + (step_y + 1);
    return movement_lookups[lookup_idx];
}
