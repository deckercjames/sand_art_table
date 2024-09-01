
#ifndef MOTOR_CONFIG_H__
#define MOTOR_CONFIG_H__

// Limit Switches
#define LIMIT_SWITCH_X_PIN_INPUT 10
#define LIMIT_SWITCH_Y_PIN_INPUT 9

// Motors Driver Pins
#define LEFT_MOTOR_STEP_PIN 4
#define LEFT_MOTOR_DIR_PIN 3
#define LEFT_MOTOR_EN_PIN 5
#define RIGHT_MOTOR_STEP_PIN 7
#define RIGHT_MOTOR_DIR_PIN 8
#define RIGHT_MOTOR_EN_PIN 6

// Motor Properties
#define MOTOR_SPEED_ORTH 1000
#define MOTOR_SPEED_DIAG 1414
#define STEPPER_MOTOR_STEPS 200
#define MOTOR_REGISTER_EXTRA_DIST_MM 50

// Temperature
#define THERMOMETER_ANALOG_IN_PIN A3
#define MOTOR_OPERATING_TEMP_F 95

// Communication Input Pin
#define INSTRUCTION_READY_PIN_IN 2
#define SIG_INT_PIN_IN A1

// Cooling Fans
#define FANS_PIN 5
#define FANS_OFF_THRESHOLD_F 72
#define FANS_ON_THRESHOLD_F 85

// Toggle Switch
#define TOGGLE_UP_IN_PIN 11
#define TOGGLE_DOWN_IN_PIN 12

typedef enum toggle_switch_position {
    TOGGLE_POSITION_OFF,
    TOGGLE_POSITION_UP,
    TOGGLE_POSITION_DOWN,
} toggle_switch_position_t;

// Bezier
#define BEZIER_SEGMENT_COUNT 500

#endif //MOTOR_CONFIG_H__