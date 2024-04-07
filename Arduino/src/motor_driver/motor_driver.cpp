
#include "motor_driver.h"

#include <Arduino.h>
#include <Adafruit_MotorShield.h>

#include "../config/config.h"
#include "../utils/logging.h"

// The motors
Adafruit_StepperMotor *motor_1;
Adafruit_StepperMotor *motor_2;

bool motors_thero_throttling;

bool init_motors()
{
    // Initilize Limit Switches
    pinMode(LIMIT_SWITCH_X_PIN_INPUT, INPUT_PULLUP);
    pinMode(LIMIT_SWITCH_Y_PIN_INPUT, INPUT_PULLUP);
    
    // Initilize Motors
    Adafruit_MotorShield AFMS = Adafruit_MotorShield();
    
    motor_1 = AFMS.getStepper(STEPPER_MOTOR_STEPS, LEFT_MOTOR_IDX);
    motor_2 = AFMS.getStepper(STEPPER_MOTOR_STEPS, RIGHT_MOTOR_IDX);
    
    if (!AFMS.begin()) {
        log_error("Could not start motor sheild");
        return false;
    }
    
    motor_1->setSpeed(MOTOR_SPEED_RPM);
    motor_2->setSpeed(MOTOR_SPEED_RPM);
    
    motors_thero_throttling = false;
    
    return true;
}

void release_motors()
{
    motor_1->release();
    motor_2->release();
}

bool thermo_throtle_motors(float temp_f)
{
    if (temp_f > MOTOR_OPERATING_TEMP_F)
    {
        motors_thero_throttling = true;
        release_motors();
    }
    else if (temp_f < MOTOR_TURN_ON_TEMP_F)
    {
        motors_thero_throttling = false;
        register_carriage();
        // reset the target pos so it is on the target. This avoid incorrect delta
        // calculations form moving without using the move_toward_target_function
        set_target_pos_steps(0, 0);
    }
    
    return motors_thero_throttling;
}

static bool _register_carriage_axis(unsigned int motor_1_dir, unsigned int motor_2_dir, int limit_pin, unsigned int max_steps)
{
    // Validate motor directions
    if (motor_1_dir != FORWARD && motor_1_dir != BACKWARD)
    {
        log_error("Can not register axis. Invalid direction for motor 1");
        return false;
    }
    if (motor_2_dir != FORWARD && motor_2_dir != BACKWARD)
    {
        log_error("Can not register axis. Invalid direction for motor 2");
        return false;
    }
    
    unsigned int registration_steps_taken = 0;
    
    // Debounce switch
    delay(1000);

    // Move until limit switch is triggered
    while (digitalRead(limit_pin) == LOW)
    {
        if (registration_steps_taken >= max_steps) {
            log_error("Max Regestration steps reached on forward");
            return false;
        }
        motor_1->step(1, motor_1_dir, MOTOR_STEP_TYPE);
        motor_2->step(1, motor_2_dir, MOTOR_STEP_TYPE);
        registration_steps_taken++;
    }
    log_debug("Triggered limit switch");
    
    // Debounce switch
    delay(1000);
    
    // Cache the reverse motor directions
    const unsigned int motor_1_dir_reverse = (motor_1_dir == FORWARD) ? BACKWARD : FORWARD;
    const unsigned int motor_2_dir_reverse = (motor_2_dir == FORWARD) ? BACKWARD : FORWARD;
    
    // Move back until limit switch is released
    registration_steps_taken = 0;
    while (digitalRead(limit_pin) == HIGH)
    {
        if (registration_steps_taken >= max_steps) {
            log_error("Max Regestration steps reached on back move");
            return false;
        }
        motor_1->step(1, motor_1_dir_reverse, MOTOR_STEP_TYPE);
        motor_2->step(1, motor_2_dir_reverse, MOTOR_STEP_TYPE);
        registration_steps_taken++;
    }
    log_debug("Released limit switch. Axis register complete.");
    
    return true;
}

/**
 * Regester the carrage with the limit switches
 * 
 * @return true on success, false on failure
*/
bool register_carriage()
{
    log_debug("Registering Carriage...");
    
    // Move Y to zero
    log_debug("Regestering Y axis...");
    if (!_register_carriage_axis(FORWARD, BACKWARD, LIMIT_SWITCH_Y_PIN_INPUT, TABLE_DIM_Y_MM)) {
        return false;
    }
    log_debug("Y axis registered.");
    
    // Move X to zero
    log_debug("Regestering X axis...");
    if (!_register_carriage_axis(BACKWARD, BACKWARD, LIMIT_SWITCH_X_PIN_INPUT, TABLE_DIM_X_MM)) {
        return false;
    }
    log_debug("X axis registered.");
    
    current_pos_x = 0;
    current_pos_y = 0;
    
    log_debug("Carriage registration complete.");
    return true;
}


void move_toward_target(const move_one_instruction_t *move_instr)
{
    motor_1->step(move_instr->steps_1, move_instr->dir_1, MOTOR_STEP_TYPE);
    motor_2->step(move_instr->steps_2, move_instr->dir_2, MOTOR_STEP_TYPE);
}

