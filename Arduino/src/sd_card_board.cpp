
#include "sd_card_board.h"

#include <Wire.h>

#include "utils/logging.h"
#include "config/config.h"
#include "sd_card/sd_card.h"

#define HALT while(1)

void wire_request_provide_next_pos();

void sd_card_board_setup()
{
    Wire.begin(SD_CARD_BOARD_I2C_ADDR);
    Wire.onRequest(wire_request_provide_next_pos); 

    if (!init_sd_card()) {
        log_fatal("Could not init sd card");
        HALT;
    }
    log_info("SD Card Initilized");

}

void sd_card_board_loop()
{
    
}

void wire_request_provide_next_pos()
{
    
}