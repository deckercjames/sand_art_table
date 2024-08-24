
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
    MOTOR_STATE_MOVING,
    MOTOR_STATE_THERMO_THROTTLE,
    MOTOR_STATE_INSTRUCTION_PENDING,
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
    
    motor_state = MOTOR_STATE_IDLE;
    
    init_registration();
    
    return true;
}

static void _make_movement(move_instr_t move)
{
    // Set motor directions
    digitalWrite(LEFT_MOTOR_DIR_PIN,  ((move >> 7) & 0x01));
    digitalWrite(RIGHT_MOTOR_DIR_PIN, ((move >> 3) & 0x01));
    
    // One or two steps for each motor
    for (int i = 0; i < 2; i++)
    {
        digitalWrite(LEFT_MOTOR_STEP_PIN,  (move >> (4 + i)) & 0x1);
        digitalWrite(RIGHT_MOTOR_STEP_PIN, (move >> (0 + i)) & 0x1);
        
        delay(MOTOR_SPEED);
        
        digitalWrite(LEFT_MOTOR_STEP_PIN,  LOW);
        digitalWrite(RIGHT_MOTOR_STEP_PIN, LOW);
        
        delay(MOTOR_SPEED);
    }
}

void set_state_register_carriage()
{
    motor_state = MOTOR_STATE_REGISTER_CARRIAGE;
    begin_regestration();
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
                log_info("Motor regestration complete. Transitioning to INSTR_PENDING");
                motor_state = MOTOR_STATE_INSTRUCTION_PENDING;
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
        case MOTOR_STATE_MOVING:
        {
            if (at_target()) {
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
        max(target_location->x_location_steps, MM_TO_STEPS(TABLE_DIM_X_MM)),
        max(target_location->y_location_steps, MM_TO_STEPS(TABLE_DIM_Y_MM))
    );
}

bool at_target()
{
    return path_calculator_at_target();
}