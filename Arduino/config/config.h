
#ifndef CONFIG_H__
#define CONFIG_H__

#include "lighting_config.h"
#include "motor_config.h"
#include "sd_card_config.h"

// Sand Table Size
#define TABLE_DIM_X_MM 900
#define TABLE_DIM_Y_MM 550

#define SAND_BOX_OFFSET_X_MM 9
#define SAND_BOX_OFFSET_Y_MM 16

#define STEPS_PER_MM 10

// Interboard commuication (movement instructions)
#define SD_CARD_BOARD_I2C_ADDR 14

#define INSTRUCTION_READY (HIGH)
#define INSTRUCTION_NOT_READY (LOW)

#endif //CONFIG_H__
