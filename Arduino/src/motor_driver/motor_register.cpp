
#include "motor_register.h"

#include "motor_utils.h"
#include "../config/config.h"
#include "../utils/logging.h"

#define LIMIT_SWITCH_DEPRESSED (HIGH)
#define LIMIT_SWITCH_RELEASED (LOW)

#define DEBOUNCE_DELAY_MS 500
#define REGISTRATION_EXTRA_DIST_MILLIS 200

typedef enum {
    REGISTRATION_STATE_IDLE,
    REGISTRATION_STATE_START,
    REGISTRATION_STATE_Y_NEG,
    REGISTRATION_STATE_Y_DELAY,
    REGISTRATION_STATE_Y_POS,
    REGISTRATION_STATE_X_NEG,
    REGISTRATION_STATE_X_DELAY,
    REGISTRATION_STATE_X_POS,
    REGISTRATION_COMPLETE,
    REGISTRATION_STATE_HALT,
} registration_state_t;

registration_state_t registration_state;

unsigned long delay_until_millis;
uint16_t regestration_steps_taken;

void init_registration()
{
    // Initilize Limit Switches
    pinMode(LIMIT_SWITCH_X_PIN_INPUT, INPUT_PULLUP);
    pinMode(LIMIT_SWITCH_Y_PIN_INPUT, INPUT_PULLUP);
    
    log_debug_value("limit x value", digitalRead(LIMIT_SWITCH_X_PIN_INPUT));
    log_debug_value("limit y value", digitalRead(LIMIT_SWITCH_Y_PIN_INPUT));
}

void begin_regestration()
{
    registration_state = REGISTRATION_STATE_START;
}

bool registration_complete()
{
    return registration_state == REGISTRATION_STATE_IDLE;
}

/**
 * @returns true on successful iteration; false otherwise (if motors should be halted)
*/
static bool _register_axis(move_instr_t *movement, int limit_switch_pin, int target_switch_state, move_instr_t direction, registration_state_t next_state, uint16_t max_registration_steps)
{
    if (digitalRead(limit_switch_pin) == target_switch_state) {
        if (target_switch_state == LIMIT_SWITCH_DEPRESSED) {
            log_debug("Limit switch pressed");
        } else {
            log_debug("Limit switch released. Axis registration complete");
        }
        delay_until_millis = millis() + DEBOUNCE_DELAY_MS;
        regestration_steps_taken = 0;
        registration_state = next_state;
        return true;
    }
    
    if (regestration_steps_taken > max_registration_steps) {
        log_error("Max registration steps reached");
        registration_state = REGISTRATION_STATE_HALT;
        return false;
    }
    
    regestration_steps_taken++;
    
    *movement = direction;
    return true;
}

/**
 * @returns true on successful iteration; false otherwise (if motors should be halted)
*/
bool service_register_carriage(move_instr_t *movement)
{
    *movement = 0;

    switch(registration_state)
    {
        case REGISTRATION_STATE_IDLE:
            break;
        case REGISTRATION_STATE_START:
            log_debug("Beginning to register carriage...");
            registration_state = REGISTRATION_STATE_Y_NEG;
            // FALLTHROUGH

        case REGISTRATION_STATE_Y_NEG:
            return _register_axis(
                movement,
                LIMIT_SWITCH_Y_PIN_INPUT,
                LIMIT_SWITCH_DEPRESSED,
                MOVE_DOWN,
                REGISTRATION_STATE_Y_DELAY,
                MM_TO_STEPS( TABLE_DIM_Y_MM + REGISTRATION_EXTRA_DIST_MILLIS )
            );
        case REGISTRATION_STATE_Y_DELAY:
            if (millis() < delay_until_millis) {
                break;
            }
            registration_state = REGISTRATION_STATE_Y_POS;
            log_debug("Debounce delay complete. Reversing driection.");
            break;
        case REGISTRATION_STATE_Y_POS:
            return _register_axis(
                movement,
                LIMIT_SWITCH_Y_PIN_INPUT,
                LIMIT_SWITCH_RELEASED,
                MOVE_UP,
                REGISTRATION_STATE_X_NEG,
                MM_TO_STEPS( REGISTRATION_EXTRA_DIST_MILLIS )
            );

        case REGISTRATION_STATE_X_NEG:
            return _register_axis(
                movement,
                LIMIT_SWITCH_X_PIN_INPUT,
                LIMIT_SWITCH_DEPRESSED,
                MOVE_LEFT,
                REGISTRATION_STATE_X_DELAY,
                MM_TO_STEPS( TABLE_DIM_X_MM + REGISTRATION_EXTRA_DIST_MILLIS )
            );
        case REGISTRATION_STATE_X_DELAY:
            if (millis() < delay_until_millis) {
                break;
            }
            registration_state = REGISTRATION_STATE_X_POS;
            log_debug("Debounce delay complete. Reversing driection.");
            break;
        case REGISTRATION_STATE_X_POS:
            return _register_axis(
                movement,
                LIMIT_SWITCH_X_PIN_INPUT,
                LIMIT_SWITCH_RELEASED,
                MOVE_RIGHT,
                REGISTRATION_COMPLETE,
                MM_TO_STEPS( REGISTRATION_EXTRA_DIST_MILLIS )
            );
        case REGISTRATION_COMPLETE:
            log_info("Carriage registration complete.");
            registration_state = REGISTRATION_STATE_IDLE;
        
        case REGISTRATION_STATE_HALT:
        default:
            return false;
    }
    
    return true;
}
