

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <stdint.h>

#include "src/led_patterns.h"
#include "config.h"
#include "logging.h"
#include "utils.h"

Adafruit_NeoPixel strip(LED_COUNT, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);

int current_patern_idx;

float anchor_led;
unsigned long last_update_time_ms;

void setup()
{
    LOG_INIT(SERIAL_BAUD);
    
    strip.begin();
    strip.show();

    current_patern_idx = 0;
    anchor_led = 0;
    last_update_time_ms = 0;
    
    init_pattern_helpers();

    log_info("Lighting initilized");
}

typedef struct strip_pattern {
    int upper_range;
    uint32_t (*fnc_ptr)(int normalized_led_idx, float intra_pattern_val);
} strip_pattern_t;

const strip_pattern_t pattern_table[] = {
    {512, duel_hue},
    {800, rainbow},
    {POT_MAX_VAL, white},
    {0, NULL} // Terminator
};


void get_pattern_bounds(int pattern_idx, int *lower_bound, int *upper_bound)
{
    *lower_bound = (pattern_idx == 0) ? 0 : pattern_table[pattern_idx-1].upper_range + POT_HYSTERESIS;
    *upper_bound = (pattern_idx == POT_MAX_VAL) ? POT_MAX_VAL : pattern_table[pattern_idx].upper_range - POT_HYSTERESIS;
}


void loop()
{
    // Read Potentiometer Values
    int brightness_val = analogRead(LED_BIGHTNESS_PIN);
    int speed_val      = analogRead(LED_SPEED_PIN);
    int pattern_val    = analogRead(LED_PATTERN_PIN);
    
    if (INVERT_BRIGHTNESS_POT) { brightness_val = POT_MAX_VAL - brightness_val; }
    if (INVERT_SPEED_POT)      { speed_val      = POT_MAX_VAL - speed_val;      }
    if (INVERT_PATTERN_POT)    { pattern_val    = POT_MAX_VAL - pattern_val;    }

    // Set brightness
    strip.setBrightness(brightness_val >> 2);

    // Set anchor LED
    const unsigned long delta_time = millis() - last_update_time_ms;
    const float normalized_speed = (float) speed_val / POT_MAX_VAL;
    anchor_led += ((float) delta_time / SECS_TO_MILLIS) * SPEED_MODIFIER * normalized_speed * normalized_speed;
    while (anchor_led > LED_COUNT) {
        anchor_led -= LED_COUNT;
    }
    last_update_time_ms = millis();
    
    // Select Pattern
    for (int i = 0; pattern_table[i].upper_range; i++) {
        int lower_bound, upper_bound;
        get_pattern_bounds(i, &lower_bound, &upper_bound);
        if (pattern_val >= lower_bound && pattern_val <= upper_bound) {
            current_patern_idx = i;
            break;
        }
    }
    
    // Calculate intra-pattern value
    int lower_bound, upper_bound;
    get_pattern_bounds(current_patern_idx, &lower_bound, &upper_bound);
    float intra_pattern_val = ((float) pattern_val - lower_bound) / (upper_bound - lower_bound);
    intra_pattern_val = min(1.0f, max(0, intra_pattern_val));

    // Set led
    for (int i = 0; i < LED_COUNT; i++) {
        uint32_t hsv_packed = pattern_table[current_patern_idx].fnc_ptr(i, intra_pattern_val);
        uint32_t pixel_color = strip.ColorHSV(
            (uint16_t) (hsv_packed >> 16),
            (uint8_t) ((hsv_packed >> 8) & 0xFF),
            (uint8_t) (hsv_packed & 0xFF));
        pixel_color = strip.gamma32(pixel_color);
        int led_idx = (i + ((int) anchor_led)) % LED_COUNT;
        strip.setPixelColor(led_idx, pixel_color);
    }
    
    strip.show();
}

