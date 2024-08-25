
#include "motor_driver.h"

#include <Arduino.h>

#include "motor_utils.h"
#include "motor_register.h"
#include "path_calculator.h"
#include "config.h"
#include "logging.h"

#define MOTOR_DISABLED (HIGH)
#define MOTOR_ENABLED (LOW)

typedef enum {
    MOTOR_STATE_IDLE,
    MOTOR_STATE_REGISTER_CARRIAGE,
    MOTOR_STATE_DRAWING,
    MOTOR_STATE_THERMO_THROTTLE,
    MOTOR_STATE_HALT,
} motor_state_t;

motor_state_t motor_state;

bool init_motors()
{
    // Initilize Motors
    pinMode(LEFT_MOTOR_STEP_PIN, OUTPUT);
    pinMode(LEFT_MOTOR_DIR_PIN, OUTPUT);
    pinMode(LEFT_MOTOR_EN_PIN, OUTPUT);
    
    pinMode(RIGHT_MOTOR_STEP_PIN, OUTPUT);
    pinMode(RIGHT_MOTOR_DIR_PIN, OUTPUT);
    pinMode(RIGHT_MOTOR_EN_PIN, OUTPUT);
    
    digitalWrite(LEFT_MOTOR_EN_PIN, MOTOR_ENABLED);
    digitalWrite(RIGHT_MOTOR_EN_PIN, MOTOR_ENABLED);
    
    motor_state = MOTOR_STATE_REGISTER_CARRIAGE;
    
    init_registration();
    
    return true;
}

static void _make_movement(move_instr_t move)
{
    // Set motor directions
    digitalWrite(LEFT_MOTOR_DIR_PIN,  ((move >> 7) & 0x01));
    digitalWrite(RIGHT_MOTOR_DIR_PIN, ((move >> 3) & 0x01));
    
    unsigned int motor_speed_delay = MOTOR_SPEED;
    // If moving diagonally, slow down by a factor of root 2
    if (move == MOVE_UP_LEFT || move == MOVE_UP_RIGHT ||
        move == MOVE_DOWN_LEFT || move == MOVE_DOWN_RIGHT) {
        motor_speed_delay = MOTOR_SPEED_DIAG;
    }
    
    // One or two steps for each motor
    for (int i = 0; i < 2; i++)
    {
        digitalWrite(LEFT_MOTOR_STEP_PIN,  (move >> (4 + i)) & 0x1);
        digitalWrite(RIGHT_MOTOR_STEP_PIN, (move >> (0 + i)) & 0x1);
        
        delayMicroseconds(motor_speed_delay);
        
        digitalWrite(LEFT_MOTOR_STEP_PIN,  LOW);
        digitalWrite(RIGHT_MOTOR_STEP_PIN, LOW);
        
        delayMicroseconds(motor_speed_delay);
    }
}

void release_motors()
{
    digitalWrite(LEFT_MOTOR_EN_PIN, MOTOR_DISABLED);
    digitalWrite(RIGHT_MOTOR_EN_PIN, MOTOR_DISABLED);
}

void service_motors()
{
    switch (motor_state)
    {
        case MOTOR_STATE_IDLE:
            break;
        case MOTOR_STATE_REGISTER_CARRIAGE:
        {
            if (registration_complete()) {
                log_info("Motor regestration complete. Transitioning to MOTOR_STATE_DRAWING");
                motor_state = MOTOR_STATE_DRAWING;
                break;
            }
            
            move_instr_t movement;
            bool register_rc;
            register_rc = service_register_carriage(&movement);
            
            if (!register_rc) {
                log_error("Motor regestration failed");
                release_motors();
                motor_state = MOTOR_STATE_HALT;
                break;
            }
            
            _make_movement(movement);
            break;
        }
        case MOTOR_STATE_DRAWING:
        {
            if (path_calculator_at_target()) {
                break;
            }
            move_instr_t move = path_calculator_get_motor_movement_instruction();
            _make_movement(move);
            break;
        }
        case MOTOR_STATE_HALT:
        case MOTOR_STATE_THERMO_THROTTLE:
            break;
    }
}

void set_target_pos_steps(const location_msg_t *target_location)
{
    path_calculator_set_target_pos_steps(
        min(target_location->x_location_steps, MM_TO_STEPS(TABLE_DIM_X_MM)),
        min(target_location->y_location_steps, MM_TO_STEPS(TABLE_DIM_Y_MM))
    );
}

bool ready_for_next_instr()
{
    return motor_state == MOTOR_STATE_DRAWING && path_calculator_at_target();
}
