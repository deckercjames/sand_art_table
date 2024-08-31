
#ifndef CONFIG_H__
#define CONFIG_H__

#include "lighting_config.h"
#include "motor_config.h"
#include "sd_card_config.h"

// Serial
#define SERIAL_BAUD 9600

// Physical Input
#define DEBOUNCE_TIME_MILLIS 20
#define POT_HYSTERESIS 5

// Sand Table Size
#define TABLE_DIM_X_MM 900
#define TABLE_DIM_Y_MM 570

#define SAND_BOX_OFFSET_X_MM 6
#define SAND_BOX_OFFSET_Y_MM 25

#define STEPS_PER_MM 10
#define STEPS_PER_100UM 1

// Interboard commuication (movement instructions)
#define SD_CARD_BOARD_I2C_ADDR 14

#define INSTRUCTION_READY (HIGH)
#define INSTRUCTION_NOT_READY (LOW)

#endif //CONFIG_H__
