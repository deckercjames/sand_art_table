
#include "thermo.h"

#include <Wire.h>
#include "Adafruit_MCP9808.h"

#include "../config/config.h"
#include "../utils/utils.h"
#include "../utils/logging.h"

Adafruit_MCP9808 tempsensor;
unsigned long next_read_time;
float current_temp;

bool init_thermometer()
{
    tempsensor = Adafruit_MCP9808();
    
    if (!tempsensor.begin(THERMO_I2C_ADDR)) {
        return false;
    }
    
    tempsensor.setResolution(THERMO_RESOLUTION);
    
    next_read_time = 0;
    
    return true;
}

bool reached_next_temp_poll_time()
{
    return millis() >= next_read_time;
}

float check_thermometer()
{
    unsigned long current_time = millis();
    
    tempsensor.wake();
    current_temp = tempsensor.readTempF();
    tempsensor.shutdown();
    
    next_read_time = current_time + (THERMO_CHECK_INTERVAL_SECS * SECS_TO_MILLIS);

    return current_temp;
}