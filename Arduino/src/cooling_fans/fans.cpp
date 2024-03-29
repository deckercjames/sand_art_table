
#include "fans.h"

#include <Arduino.h>

#include "../config/config.h"

void init_fans()
{
    pinMode(FANS_PIN, OUTPUT);
}

void control_fans(float temp_f)
{
    if (temp_f < FANS_OFF_THRESHOLD_F) {
        digitalWrite(FANS_PIN, LOW);
        return;
    }
    
    if (temp_f > FANS_ON_THRESHOLD_F) {
        digitalWrite(FANS_PIN, HIGH);
        return;
    }
}