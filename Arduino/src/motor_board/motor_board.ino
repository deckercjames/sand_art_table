
#include <Arduino.h>
#include <Wire.h>

#include "config.h"
#include "utils.h"
#include "logging.h"
#include "src/motor_driver/motor_driver.h"
#include "src/motor_driver/motor_register.h"
#include "src/heat_manager/heat_manager.h"
#include "src/i2c_comm/i2c_comm.h"
#include "src/input/toggle_switch.h"
#include "src/graphing/bezier.h"
#include "src/graphing/path_calculator.h"

#define HALT do { release_motors(); while(1); } while(0)

typedef enum {
    MOTOR_STATE_IDLE,
    MOTOR_STATE_REGISTER_CARRIAGE,
    MOTOR_STATE_START_IMAGE,
    MOTOR_STATE_DRAWING_IMAGE,
    MOTOR_STATE_START_BEZIER,
    MOTOR_STATE_DRAWING_BEZIER,
} motor_state_t;

motor_state_t motor_state;


void setup()
{
    LOG_INIT(SERIAL_BAUD);

    init_i2c_communcation();

    init_heat_manager();

    init_toggle_switch();

    init_motors();
    log_info("Motors Initilized");

    pinMode(INSTRUCTION_READY_PIN_IN, INPUT);
    pinMode(SIG_INT_PIN_IN, INPUT);
    
    motor_state = MOTOR_STATE_REGISTER_CARRIAGE;
}

static void _attempt_get_position_from_wire()
{
    if (digitalRead(INSTRUCTION_READY_PIN_IN) != INSTRUCTION_READY) {
        return;
    }
    
    location_msg_t location;
    get_next_position_from_wire(&location);
    set_target_pos_steps(location.x_location_steps, location.y_location_steps);
}

static bool _set_state_based_on_toggle_pos(toggle_switch_position_t toggle_pos)
{
    switch (toggle_pos)
    {
        case TOGGLE_POSITION_OFF:
            motor_state = MOTOR_STATE_IDLE;
            break;
        case TOGGLE_POSITION_UP:
            motor_state = MOTOR_STATE_START_BEZIER;
            break;
        case TOGGLE_POSITION_DOWN:
            motor_state = MOTOR_STATE_START_IMAGE;
            break;
    }
}

static bool _process_toggle_input()
{
    toggle_switch_position_t toggle_pos;
    bool toggle_pos_changed = check_toggle_switch(&toggle_pos);
    if (!toggle_pos_changed) {
        return false;
    }
    
    _set_state_based_on_toggle_pos(toggle_pos);
    return true;
}

void loop()
{
    // manage_heat();
    // if (!motors_operable()) {
    //     log_fatal("Motors have overheated. Halting");
    //     HALT;
    // }

    switch (motor_state)
    {
        case MOTOR_STATE_REGISTER_CARRIAGE:
        {
            toggle_switch_position_t toggle_pos;
            check_toggle_switch(&toggle_pos);
            if (toggle_pos == TOGGLE_POSITION_OFF) {
                break;
            }
            if (registration_complete()) {
                log_info("Registration complete");
                randomSeed(micros());
                motor_state = MOTOR_STATE_IDLE;
                break;
            }
            move_instr_t movement;
            bool register_rc = service_register_carriage(&movement);
            if (!register_rc) {
                log_error("Motor registration failed");
                release_motors();
                HALT;
            }
            service_motors(movement);
            break;
        }
        case MOTOR_STATE_IDLE:
        {
            toggle_switch_position_t toggle_pos;
            check_toggle_switch(&toggle_pos);
            _set_state_based_on_toggle_pos(toggle_pos);
            break;
        }
        case MOTOR_STATE_START_IMAGE:
            // reset the path, so it immediately reads as ready for the next instruction
            path_calculator_reset_target();
            motor_state = MOTOR_STATE_DRAWING_IMAGE;
            // FALLTHROUGH
        case MOTOR_STATE_DRAWING_IMAGE:
        {
            if (_process_toggle_input()) {
                break;
            }

            if (digitalRead(SIG_INT_PIN_IN))
            {
                log_info("Interupt signal received");
                _attempt_get_position_from_wire();
            }
            else if (path_calculator_at_target())
            {
                log_info("Motors are ready for next instruction");
                _attempt_get_position_from_wire();
                
            }
            
            move_instr_t movement = path_calculator_get_motor_movement_instruction();
            service_motors(movement);
            break;
        }
        case MOTOR_STATE_START_BEZIER:
            // reset the path, so it immediately reads as ready for the next instruction
            path_calculator_reset_target();
            start_bezier_path(get_current_pos_x(), get_current_pos_y());
            motor_state = MOTOR_STATE_DRAWING_BEZIER;
            // FALLTHROUGH
        case MOTOR_STATE_DRAWING_BEZIER:
        {
            if (_process_toggle_input()) {
                break;
            }

            if (path_calculator_at_target()) {
                location_msg_t location;
                get_next_bezier_point(&location);
                set_target_pos_steps(location.x_location_steps, location.y_location_steps);
            }
            
            move_instr_t movement = path_calculator_get_motor_movement_instruction();
            service_motors(movement);
            break;
        }
    }

}
