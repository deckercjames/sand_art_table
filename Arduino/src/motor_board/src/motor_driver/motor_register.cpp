
#include "motor_register.h"

#include "config.h"
#include "logging.h"
#include "utils.h"

#define LIMIT_SWITCH_DEPRESSED (HIGH)
#define LIMIT_SWITCH_RELEASED (LOW)

#define DEBOUNCE_DELAY_MS 500
#define REGISTRATION_EXTRA_DIST_MILLIS 200

typedef enum {
    REGISTRATION_STATE_START = 0,
    REGISTRATION_STATE_Y_NEG,
    REGISTRATION_STATE_Y_POS,
    REGISTRATION_STATE_X_NEG,
    REGISTRATION_STATE_X_POS,
    REGISTRATION_COMPLETE,
    REGISTRATION_HALT,
} registration_state_t;

registration_state_t registration_state;

typedef enum registration_intruction_signal {
    REG_INSTR_SIG_IMMEDIATE,
    REG_INSTR_SIG_LIMIT_SWITCH,
    REG_INSTR_SIG_STEP_COUNT,
} registration_intruction_signal_t;

typedef struct registration_instruction {
    registration_intruction_signal_t state_end_signal;
    registration_state_t next_state;
    move_instr_t move_instruction;
    uint16_t max_steps;
    int limit_switch_pin;
    int switch_action;
} registration_instruction_t;

registration_instruction_t state_definitions[] = {
    [REGISTRATION_STATE_START] = {
        .state_end_signal = REG_INSTR_SIG_IMMEDIATE,
        .next_state = REGISTRATION_STATE_Y_NEG,
    },
    [REGISTRATION_STATE_Y_NEG] = {
        .state_end_signal = REG_INSTR_SIG_LIMIT_SWITCH,
        .next_state = REGISTRATION_STATE_Y_POS,
        .move_instruction = MOVE_INSTR_DOWN,
        .max_steps = MM_TO_STEPS( TABLE_DIM_Y_MM + REGISTRATION_EXTRA_DIST_MILLIS ),
        .limit_switch_pin = LIMIT_SWITCH_Y_PIN_INPUT,
        .switch_action = LIMIT_SWITCH_DEPRESSED,
    },
    [REGISTRATION_STATE_Y_POS] = {
        .state_end_signal = REG_INSTR_SIG_STEP_COUNT,
        .next_state = REGISTRATION_STATE_X_NEG,
        .move_instruction = MOVE_INSTR_UP,
        .max_steps = MM_TO_STEPS( SAND_BOX_OFFSET_Y_MM ),
    },
    [REGISTRATION_STATE_X_NEG] = {
        .state_end_signal = REG_INSTR_SIG_LIMIT_SWITCH,
        .next_state = REGISTRATION_STATE_X_POS,
        .move_instruction = MOVE_INSTR_LEFT,
        .max_steps = MM_TO_STEPS( TABLE_DIM_X_MM + REGISTRATION_EXTRA_DIST_MILLIS ),
        .limit_switch_pin = LIMIT_SWITCH_X_PIN_INPUT,
        .switch_action = LIMIT_SWITCH_DEPRESSED,
    },
    [REGISTRATION_STATE_X_POS] = {
        .state_end_signal = REG_INSTR_SIG_STEP_COUNT,
        .next_state = REGISTRATION_COMPLETE,
        .move_instruction = MOVE_INSTR_RIGHT,
        .max_steps = MM_TO_STEPS( SAND_BOX_OFFSET_Y_MM ),
    },
};

uint16_t steps_taken_in_state;

void init_registration()
{
    // Initilize Limit Switches
    pinMode(LIMIT_SWITCH_X_PIN_INPUT, INPUT_PULLUP);
    pinMode(LIMIT_SWITCH_Y_PIN_INPUT, INPUT_PULLUP);

    registration_state = REGISTRATION_STATE_START;
    steps_taken_in_state = 0;
}

bool registration_complete()
{
    return registration_state == REGISTRATION_COMPLETE;
}

static void transition_to_next_state()
{
    registration_state = state_definitions[registration_state].next_state;
    steps_taken_in_state = 0;
}

/**
 * @returns true on successful iteration; false otherwise (if motors should be halted)
*/
bool service_register_carriage(move_instr_t *movement)
{
    *movement = MOVE_INSTR_NONE;
    
    if (registration_state == REGISTRATION_COMPLETE) {
        return true;
    }
    
    if (registration_state == REGISTRATION_HALT) {
        return false;
    }
    
    registration_instruction_t *reg_instr = &state_definitions[registration_state];

    if (reg_instr->state_end_signal == REG_INSTR_SIG_IMMEDIATE) {
        transition_to_next_state();
        return true;
    }
    
    if (steps_taken_in_state >= reg_instr->max_steps) {
        if (reg_instr->state_end_signal == REG_INSTR_SIG_STEP_COUNT) {
            log_fatal("Axis step based registration complete");
            transition_to_next_state();
        } else {
            log_fatal("Reach unexpected max steps");
            registration_state = REGISTRATION_HALT;
        }
        return true;
    }
    
    if (digitalRead(reg_instr->limit_switch_pin) == reg_instr->switch_action) {
        log_fatal("Axis switch based registration complete");
        transition_to_next_state();
        return true;
    }
    
    *movement = reg_instr->move_instruction;
    steps_taken_in_state++;

    return true;
}
