
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

    pinMode(INSTRUCTION_READY_PIN_IN, INPUT);
    pinMode(SIG_INT_PIN_IN, INPUT);
    
    motor_state = MOTOR_STATE_REGISTER_CARRIAGE;

    log_info("Initialization Complete");
}

void loop()
{
    // manage_heat();
    // if (!motors_operable()) {
    //     log_fatal("Motors have overheated. Halting");
    //     release_motors();
    //     HALT;
    // }
    
    // Check toggle switch
    toggle_switch_position_t toggle_pos = check_toggle_switch();
    if (toggle_pos == TOGGLE_POSITION_OFF) {
        return;
    }

    // Main state machine
    switch (motor_state)
    {
        case MOTOR_STATE_REGISTER_CARRIAGE:
        {
            if (registration_complete()) {
                log_info("Registration complete");
                randomSeed(micros() >> 2);
                // If the toggle switch is actually in bezier mode, the state will just switch immediatly next iteration
                motor_state = MOTOR_STATE_START_IMAGE;
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
        case MOTOR_STATE_START_IMAGE:
            // reset the path, so it immediately reads as ready for the next instruction
            path_calculator_reset_target();
            motor_state = MOTOR_STATE_DRAWING_IMAGE;
            // FALLTHROUGH
        case MOTOR_STATE_DRAWING_IMAGE:
        {
            if (toggle_pos == TOGGLE_POSITION_UP) {
                motor_state = MOTOR_STATE_START_BEZIER;
                break;
            }

            bool get_pos_from_wire = false;
            if (digitalRead(SIG_INT_PIN_IN))
            {
                log_info("Interupt signal received");
                get_pos_from_wire = true;
            }
            else if (path_calculator_at_target())
            {
                get_pos_from_wire = true;
            }
            
            if (get_pos_from_wire) {
                if (digitalRead(INSTRUCTION_READY_PIN_IN) != INSTRUCTION_READY) {
                    break;
                }
                location_msg_t location;
                get_next_position_from_wire(&location);
                set_target_pos_steps(location.x_location_steps, location.y_location_steps);
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
            if (toggle_pos == TOGGLE_POSITION_DOWN) {
                motor_state = MOTOR_STATE_START_IMAGE;
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
