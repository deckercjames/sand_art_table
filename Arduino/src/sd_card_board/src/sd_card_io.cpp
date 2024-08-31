
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
    current_file = SD.open("test.gcd");
    if (!current_file) {
        log_error("File opened Failed");
        return false;
    }
    log_info("File successfully opened");
    return true;
}

bool open_file_idx(int file_idx)
{
    char file_name[20] = "patterns/pat_0.gcd";
    file_name[13] += file_idx;
    current_file = SD.open(file_name);
    if (!current_file) {
        log_error("File opened Failed");
        return false;
    }
    log_debug_value("Opened file", file_name);
    return true;
}

void close_current_file()
{
    if (!current_file) {
        log_error("Can not close file. No file open");
        return;
    }
    log_debug("Closing current file");
    current_file.close();
}

void get_next_line(char *buf, unsigned int buf_size)
{
    unsigned int i = 0;

    if (!current_file) {
        log_error("No current file");
        buf[0] = 0;
        return;
    }
    
    while (1)
    {
        // Save room for a null terminator
        if (i == (buf_size - 1)) {
            break;
        }

        int val = current_file.read();
        
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
    return !current_file || !current_file.available();
}
