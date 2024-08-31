
#include <Arduino.h>
#include <Wire.h>

#include "config.h"
#include "utils.h"
#include "logging.h"
#include "src/motor_driver/motor_driver.h"
#include "src/motor_driver/motor_utils.h"
#include "src/heat_manager/heat_manager.h"
#include "src/input/toggle_switch.h"

#define HALT do { release_motors(); while(1); } while(0)

void setup()
{
    LOG_INIT(SERIAL_BAUD);

    Wire.begin();
    
    init_heat_manager();
    
    init_toggle_switch();

    if (!init_motors()) {
        log_fatal("Could not initilize motors");
        HALT;
    }
    log_info("Motors Initilized");

    pinMode(INSTRUCTION_READY_PIN_IN, INPUT);
    pinMode(SIG_INT_PIN_IN, INPUT);
}

static void _get_next_position_from_wire()
{
    location_msg_t location;
    uint8_t *received_buffer = (uint8_t *) &location;
    
    int index = 0;

    log_debug("Requesting next position from wire");
    
    location.x_location_steps = 0;
    location.y_location_steps = 0;
    
    Wire.requestFrom(SD_CARD_BOARD_I2C_ADDR, 4);
    
    while (Wire.available()) { // peripheral may send less than requested
        char c = Wire.read();
        // Discard any extra bytes
        if (index >= sizeof(location_msg_t)) {
            continue;
        }
        received_buffer[index] = (uint8_t) c;
        index++;
    }
    
    if (index < sizeof(location_msg_t)) {
        log_error("Received too few bytes");
    }

    log_debug_value("Received Position [x steps]", location.x_location_steps);
    log_debug_value("Received Position [y steps]", location.y_location_steps);

    set_target_pos_steps(&location);
}

void loop()
{
    // manage_heat();
    // if (!motors_operable()) {
    //     log_fatal("Motors have overheated. Halting");
    //     HALT;
    // }
    
    toggle_switch_position_t toggle_pos = check_toggle_switch();
    if (toggle_pos == TOGGLE_POSITION_OFF) {
        return;
    }
    set_drawing_type(toggle_pos == TOGGLE_POSITION_UP ? DRAWING_TYPE_BEZIER : DRAWING_TYPE_IMAGE);

    // Read next instruction if necesssary
    if (is_in_image_mode()) {
        if (digitalRead(SIG_INT_PIN_IN) && digitalRead(INSTRUCTION_READY_PIN_IN) == INSTRUCTION_READY) {
            log_info("Interupt signal received");
            _get_next_position_from_wire();
        } else if (ready_for_next_instr() && digitalRead(INSTRUCTION_READY_PIN_IN) == INSTRUCTION_READY) {
            log_info("Motors are ready for next instruction");
            _get_next_position_from_wire();
        }
    }
    
    service_motors();
}
