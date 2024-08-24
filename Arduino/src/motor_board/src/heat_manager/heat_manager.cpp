
#include "heat_manager.h"

#include "config.h"
#include "utils.h"
#include "logging.h"

float temp_f;
bool fans_on;

bool init_heat_manager()
{
    pinMode(FANS_PIN, OUTPUT);
    fans_on = false;
}

void manage_heat()
{
    int sensor_value = analogRead(THERMOMETER_ANALOG_IN_PIN);
    
    temp_f = ((THERMOMETER_POWER_MILLIVOLTS - 500.0f) * 0.18f) + 32.0f;
    
    log_debug_value("Current temerature (deg. F)", temp_f);
    
    if (temp_f < FANS_OFF_THRESHOLD_F && fans_on) {
        log_info("Turning fans ON");
        digitalWrite(FANS_PIN, LOW);
        fans_on = false;
        return;
    }
    
    if (temp_f > FANS_ON_THRESHOLD_F && !fans_on) {
        log_info("Turning fans OFF");
        digitalWrite(FANS_PIN, HIGH);
        fans_on = true;
        return;
    }
}

bool motors_operable()
{
    return temp_f <= MOTOR_OPERATING_TEMP_F;
}
