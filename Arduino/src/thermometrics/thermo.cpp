
#include "thermo.h"

#include <Wire.h>
#include "Adafruit_MCP9808.h"

#include "../config/config.h"
#include "../utils/utils.h"

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

float check_thermometer()
{
    unsigned long current_time = millis();
    
    if (current_time >= next_read_time) {
        tempsensor.wake();
        current_temp = tempsensor.readTempC();
        tempsensor.shutdown();
        next_read_time = THERMO_CHECK_INTERVAL_SECS * SECS_TO_MILLIS;
    }
    
    return current_temp;
}