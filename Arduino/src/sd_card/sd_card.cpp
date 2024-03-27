

#include <SPI.h>
#include <SD.h>

#include "../config/config.h"
#include "sd_card.h"

File current_file;

bool init_sd_card()
{
    return SD.begin(CARD_SELECT_PIN);
}

void open_next_file()
{
    current_file = SD.open("test.txt");
}

void get_next_line(char *buf, unsigned int buf_size)
{
    unsigned int i = 0;
    int c;
    
    if (!current_file) {
        buf[0] = 0;
        return;
    }
    
    while (1)
    {
        c = current_file.read();
        
        if (c == -1) {
            break;
        }
        
        buf[i] = (char) c;
        
        i++;
    }
    
    buf[i] = 0;
}
