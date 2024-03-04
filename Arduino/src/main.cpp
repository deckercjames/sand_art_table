
#include <Arduino.h>

#include "logger/logger.h"
#include "motor_driver/motor_driver.h"
#include "led_strip/led_strip.h"

#define HALT while(1)

void main_setup()
{
    init_logger();
    log_info("Executing Setup **************************************");

    if (!init_motors()) {
        log_fatal("Could not initilize motors");
        HALT;
    }
    log_info("Motors Initilized");
    
    // if (!register_carriage()) {
    //     log_fatal("Could not register motors");
    //     HALT;
    // }
    // HALT;
    
    set_target_pos(200, 200);
    while (!at_target()) {
        move_toward_target();
    }
    log_debug("at pos 1");
    set_target_pos(0, 400);
    while (!at_target()) {
        move_toward_target();
    }
    log_debug("at pos 2");
    set_target_pos(0, 0);
    while (!at_target()) {
        move_toward_target();
    }
    log_debug("at pos 3");
}


void main_loop()
{
    // led_strip_loop();
    // move_toward_target();
}

