
#include "lighting_board.h"

#include <Arduino.h>

#include "utils/logging.h"
#include "config/config.h"
#include "led_strip/led_strip.h"

#define HALT while(1)

void lighting_board_setup()
{
    init_led_strip();
    log_info("Initialized LED Strips");
}

void lighting_board_loop()
{
    update_led_strip();
}
