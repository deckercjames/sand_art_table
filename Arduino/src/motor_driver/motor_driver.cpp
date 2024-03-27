
#include "motor_driver.h"

#include <Arduino.h>
#include <Adafruit_MotorShield.h>

#include "../config/config.h"

// The motors
Adafruit_StepperMotor *motor_1;
Adafruit_StepperMotor *motor_2;

bool init_motors()
{
    // Initilize Limit Switches
    pinMode(LIMIT_SWITCH_X_PIN_INPUT, INPUT_PULLUP);
    pinMode(LIMIT_SWITCH_Y_PIN_INPUT, INPUT_PULLUP);
    
    // Initilize Motors
    Adafruit_MotorShield AFMS = Adafruit_MotorShield();
    
    motor_1 = AFMS.getStepper(STEPPER_MOTOR_STEPS, 1);
    motor_2 = AFMS.getStepper(STEPPER_MOTOR_STEPS, 2);
    
    if (!AFMS.begin()) {
        Serial.println(F("Could not start motor sheild"));
        return false;
    }
    
    motor_1->setSpeed(MOTOR_SPEED_RPM);
    motor_2->setSpeed(MOTOR_SPEED_RPM);
    
    return true;
}

void release_motors()
{
    motor_1->release();
    motor_2->release();
}

static bool _register_carriage_axis(unsigned int motor_1_dir, unsigned int motor_2_dir, int limit_pin, unsigned int max_steps)
{
    // Validate motor directions
    if (motor_1_dir != FORWARD && motor_1_dir != BACKWARD)
    {
        Serial.println(F("Can not register axis. Invalid direction for motor 1"));
        return false;
    }
    if (motor_2_dir != FORWARD && motor_2_dir != BACKWARD)
    {
        Serial.println(F("Can not register axis. Invalid direction for motor 2"));
        return false;
    }
    
    unsigned int registration_steps_taken = 0;
    
    // Debounce switch
    delay(1000);

    // Move until limit switch is triggered
    while (digitalRead(limit_pin) == LOW)
    {
        if (registration_steps_taken >= max_steps) {
            Serial.println(F("Max Regestration steps reached"));
            return false;
        }
        motor_1->step(1, motor_1_dir, MOTOR_STEP_TYPE);
        motor_2->step(1, motor_2_dir, MOTOR_STEP_TYPE);
        registration_steps_taken++;
    }
    Serial.println(F("Triggered limit switch"));
    
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
            Serial.println(F("Max Regestration steps reached"));
            return false;
        }
        motor_1->step(1, motor_1_dir_reverse, MOTOR_STEP_TYPE);
        motor_2->step(1, motor_2_dir_reverse, MOTOR_STEP_TYPE);
        registration_steps_taken++;
    }
    Serial.println(F("Released limit switch. Axis register complete."));
    
    return true;
}

/**
 * Regester the carrage with the limit switches
 * 
 * @return true on success, false on failure
*/
bool register_carriage()
{
    Serial.println("Registering Carriage...");
    
    // Move Y to zero
    Serial.println(F("Regestering Y axis..."));
    if (!_register_carriage_axis(FORWARD, BACKWARD, LIMIT_SWITCH_Y_PIN_INPUT, TABLE_DIM_Y_MM)) {
        return false;
    }
    Serial.println(F("Y axis registered."));
    
    // Move X to zero
    Serial.println(F("Regestering X axis..."));
    if (!_register_carriage_axis(BACKWARD, BACKWARD, LIMIT_SWITCH_X_PIN_INPUT, TABLE_DIM_X_MM)) {
        return false;
    }
    Serial.println(F("X axis registered."));
    
    current_pos_x = 0;
    current_pos_y = 0;
    
    Serial.println(F("Carriage registration complete."));
    return true;
}


void move_toward_target(const move_one_instruction_t *move_instr)
{
    motor_1->step(move_instr->steps_1, move_instr->dir_1, MOTOR_STEP_TYPE);
    motor_2->step(move_instr->steps_2, move_instr->dir_2, MOTOR_STEP_TYPE);
}

