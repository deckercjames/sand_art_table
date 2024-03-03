
#include <Arduino.h>

#include "logger/logger.h"
#include "motor_driver/motor_driver.h"
#include "led_strip/led_strip.h"

void main_setup()
{
    logger_init();
    log_info("Executing Setup **************************************");

    init_motors();
    log_info("Motors Initilized");
    
    // init_led_strip();
    
    register_carriage();
    
    // set_new_target(100, 100);
}


void main_loop()
{
    // led_strip_loop();
    // move_toward_target();
}

