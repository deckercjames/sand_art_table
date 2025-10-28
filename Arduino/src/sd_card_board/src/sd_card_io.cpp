
#include "sd_card_io.h"

#include <SPI.h>
#include <SD.h>

#include "logging.h"
#include "config.h"

File current_file;

int current_file_idx[4] = { 0 };

bool init_sd_card()
{
    return SD.begin(CARD_SELECT_PIN);
}

bool open_file_idx(int button_idx)
{
    char file_name[20] = "but_0/pat_0.txt";
    file_name[4] += button_idx;
    file_name[10] += current_file_idx[button_idx];
    log_debug_value("Attempting to open file", file_name);
    current_file = SD.open(file_name);
    if (!current_file) {
        if (current_file_idx[button_idx] > 0) {
            log_info("Failed to open file. Resetting index");
            current_file_idx[button_idx] = 0;
            return open_file_idx(button_idx);
        }
        log_error("File opened Failed");
        return false;
    }
    current_file_idx[button_idx]++;
    current_file_idx[button_idx] %= 10;
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

        // EOF
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
