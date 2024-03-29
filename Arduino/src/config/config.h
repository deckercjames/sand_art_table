
#ifndef CONFIG_H__
#define CONFIG_H__

// Sand Table Size
#define TABLE_DIM_X_MM (10000)
#define TABLE_DIM_Y_MM (10000)

// Limit Switches
#define LIMIT_SWITCH_X_PIN_INPUT (8)
#define LIMIT_SWITCH_Y_PIN_INPUT (6)

// Motors
#define MOTOR_SPEED_RPM (255) // between [1, 225]
#define STEPPER_MOTOR_STEPS (200)
#define MOTOR_STEP_TYPE (DOUBLE)
#define TIMING_PULLY_TEETH (20)
#define BELT_TEETH_SPACING_MM (2)
#define MOTOR_OPERATING_TEMP_F (95)
#define MOTOR_TURN_ON_TEMP_F (80)

// SD Card
#define CARD_SELECT_PIN (10)
#define LINE_BUFFER_SIZE (128)

// Lighting
#define LED_STRIP_PIN (8)
#define LED_COUNT (176)

// Thermometer
#define THERMO_I2C_ADDR (0x18) /* This is the default address*/
#define THERMO_RESOLUTION (0)
#define THERMO_CHECK_INTERVAL_SECS (10)
// Mode Resolution SampleTime
// 0    0.5°C       30 ms
// 1    0.25°C      65 ms
// 2    0.125°C     130 ms
// 3    0.0625°C    250 ms

// Cooling Fans
#define FANS_PIN (5)
#define FANS_OFF_THRESHOLD_F (72)
#define FANS_ON_THRESHOLD_F (85)

#endif //CONFIG_H__
