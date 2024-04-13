
#include <Arduino.h>

#include "sd_card/sd_card.h"
#include "motor_driver/motor_driver.h"
#include "led_strip/led_strip.h"
#include "config/config.h"
#include "thermometrics/thermo.h"
#include "cooling_fans/fans.h"
#include "utils/logging.h"

#define HALT do { release_motors(); while(1); } while(0)

void main_setup()
{
    Serial.begin(9600);
    log_debug("Executing Setup **************************************");

    init_led_strip();
    
    if (!init_motors()) {
        log_fatal("Could not initilize motors");
        HALT;
    }
    log_info("Motors Initilized");
    
    // if (!init_sd_card()) {
    //     log_fatal("Could not init sd card");
    //     HALT;
    // }
    
    if (!register_carriage()) {
        log_fatal("Could not register motors");
        HALT;
    }
    log_info("Carriage registration complete.");
    
    // if (!init_thermometer()) {
    //     log_fatal("Could not init thermometer");
    //     HALT;
    // }
    
    init_fans();
}


void main_loop()
{
    char line_buffer[LINE_BUFFER_SIZE];
    
    // update_led_strip();
    
    // float temp_f = check_thermometer();
    
    // control_fans(temp_f);
    
    // if (thermo_throtle_motors(temp_f)) {
    //     return;
    // }
    
    // Read next instruction if necesssary
    if (at_target()) {
        // set next target
        get_test_line(line_buffer, sizeof(line_buffer));
        set_target_position_gcode(line_buffer);
    }
    
    // guard again incase the next instruction was a noop (e.g. a gcode comment)
    if (at_target()) {
        return;
    }
    
    move_toward_target();
}

