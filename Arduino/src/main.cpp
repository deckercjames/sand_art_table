
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
    
    if (!register_carriage()) {
        log_fatal("Could not register motors");
        HALT;
    }
}


void main_loop()
{
    if (at_target()) {
        // set next target
    }
    
    move_one_instruction_t move_instr;
    
    get_motor_movement_instructions(&move_instr);
    move_toward_target(&move_instr);
    
}

