
#ifndef CONFIG_H__
#define CONFIG_H__

// Sand Table Configurations
#define TABLE_DIM_X_MM 10000
#define TABLE_DIM_Y_MM 10000

// Wiring Configuration
#define LIMIT_SWITCH_X_PIN_INPUT 8
#define LIMIT_SWITCH_Y_PIN_INPUT 6

// Motor Configurations
#define MOTOR_SPEED_RPM 255 // between [1, 225]
#define STEPPER_MOTOR_STEPS 200
#define MOTOR_STEP_TYPE DOUBLE
#define TIMING_PULLY_TEETH 20
#define BELT_TEETH_SPACING_MM 2

// SD CARD Configurations
#define CARD_SELECT_PIN 10
#define LINE_BUFFER_SIZE 128

#endif //CONFIG_H__
