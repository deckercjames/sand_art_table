
#include "sd_card_io.h"

#include <SPI.h>
#include <SD.h>

#include "logging.h"
#include "config.h"

File current_file;

bool init_sd_card()
{
    return SD.begin(CARD_SELECT_PIN);
}

bool open_next_file()
{
    if (!SD.exists("test.gcd")) {
        log_error("File does not exist");
        return false;
    }
    log_debug("File exists");
    log_debug("opening next file");
    current_file = SD.open("test.gcd");
    if (!current_file) {
        log_error("File opened Failed");
        return false;
    }
    
    log_info("File successfully opened");
    return true;
}

void close_current_file()
{
    log_debug("Closing current file");
    current_file.close();
}

void get_next_line(char *buf, unsigned int buf_size)
{
    unsigned int i = 0;
    int val;
    
    if (!current_file) {
        log_debug("no current file");
        buf[0] = 0;
        return;
    }
    
    while (1)
    {
        val = current_file.read();
        
        log_debug_value("reading value", val);
        
        if (val == -1) {
            break;
        }
        
        char c = (char) val;
        
        if (c == '\n') {
            break;
        }
        
        buf[i] = c;
        
        i++;
    }
    
    log_debug("buffer read complete");
    buf[i] = 0;
}


bool file_completed()
{
    return !current_file.available();
}
