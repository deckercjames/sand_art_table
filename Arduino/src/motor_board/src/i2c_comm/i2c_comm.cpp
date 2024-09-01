
#include "i2c_comm.h"

#include <Wire.h>
#include <stdint.h>

#include "logging.h"
#include "config.h"

void init_i2c_communcation()
{
    Wire.begin();
}

void get_next_position_from_wire(location_msg_t *location)
{
    uint8_t *received_buffer = (uint8_t *) location;

    int index = 0;

    log_debug("Requesting next position from wire");

    location->x_location_steps = 0;
    location->y_location_steps = 0;

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

    log_debug_value("Received Position [x steps]", location->x_location_steps);
    log_debug_value("Received Position [y steps]", location->y_location_steps);
}
