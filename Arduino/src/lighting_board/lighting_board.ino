

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <stdint.h>

#include "config.h"
#include "logging.h"

Adafruit_NeoPixel strip(LED_COUNT, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);

#define DUEL_HUE_DELTA 13553

float cos_table[LED_COUNT];

int current_patern_idx;

void setup()
{
    LOG_INIT(SERIAL_BAUD);
    
    strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    strip.show();            // Turn OFF all pixels ASAP

    current_patern_idx = 0;

    for (int i = 0; i < LED_COUNT; i++) {
        cos_table[i] = 0.5f - (cos(((float) i / (LED_COUNT - 1)) * PI) * 0.5f);
    }

    log_info("Lighting initilized");
}

static void _duel_hue(float pattern_val);

typedef struct strip_pattern {
    int upper_range;
    uint16_t (*fnc_ptr)(int normalized_led_idx, float intra_pattern_val);
} strip_pattern_t;

const strip_pattern_t pattern_table[] = {
    {1024, _duel_hue},
    {0, NULL} // Terminator
};


void loop()
{
    // Read Pot Values
    int brightness_val = analogRead(LED_BIGHTNESS_PIN);
    int speed_val      = analogRead(LED_SPEED_PIN);
    int pattern_val    = analogRead(LED_PATTERN_PIN);
    
    // Set brightness
    strip.setBrightness(brightness_val >> 2);
    
    // Set anchor LED
    int anchor_led = ((int) ((millis() * SPEED_MODIFIER)/ speed_val)) % LED_COUNT;
    
    float intra_pattern_val = 0.0f;
    
    // Select Pattern
    for (int i = 0; pattern_table[i].upper_range; i++) {
        int lower_bound = (i == 0) ? 0 : pattern_table[i-1].upper_range + POT_HYSTERESIS;
        int upper_bound = (i == 1024) ? 1024 : pattern_table[i].upper_range - POT_HYSTERESIS;
        if (pattern_val >= lower_bound && pattern_val <= upper_bound) {
            current_patern_idx = i;
            intra_pattern_val = ((float) pattern_val - lower_bound) / (upper_bound - lower_bound);
            break;
        }
    }
    
    
    unsigned int start_hue = pattern_val << 6;
    
    for (int i = 0; i <= LED_COUNT; i++) {
        uint16_t pixel_hue = pattern_table[current_patern_idx].fnc_ptr(i, intra_pattern_val);
        uint32_t color = strip.gamma32(strip.ColorHSV(pixel_hue));
        int led_idx = (i + anchor_led) % LED_COUNT;
        strip.setPixelColor(led_idx, color);
    }
    
    strip.show();
}


static uint16_t _duel_hue(int normalized_led_idx, float intra_pattern_val)
{
    uint16_t start_hue = (uint16_t) (0xFFFF * intra_pattern_val);
    uint16_t hue_delta = (uint16_t) (DUEL_HUE_DELTA * cos_table[normalized_led_idx]);
    return start_hue + hue_delta;
}
