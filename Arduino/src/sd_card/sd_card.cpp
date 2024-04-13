

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



// TESTING

char *commands[] = {
    "G01 X0 Y1000",
    "G01 X1000 Y1000",
    "G01 X1000 Y0"
};

int on_idx = 0;

void get_test_line(char *buf, unsigned int buf_size)
{
    int i = 0;
    while (commands[on_idx][i] && i<buf_size) {
        buf[i] = commands[on_idx][i];
        i++;
    }
    on_idx++;
    if (on_idx == 4) while(1);
    // on_idx %= 3;
}
