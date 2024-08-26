
#include "led_patterns.h"

#include "utils.h"
#include "config.h"

#define DUEL_HUE_DELTA 13553

float cos_table[LED_COUNT];

const Adafruit_NeoPixel *pat_strip;

void init_pattern_helpers(const Adafruit_NeoPixel *strip_ptr)
{
    for (int i = 0; i < LED_COUNT; i++) {
        cos_table[i] = 0.5f - (cos(((float) i / (LED_COUNT - 1)) * PI * 2) * 0.5f);
    }

    pat_strip = strip_ptr;
}

uint32_t duel_hue(int normalized_led_idx, float intra_pattern_val)
{
    uint16_t start_hue = (uint16_t) (0xFFFF * intra_pattern_val);
    uint16_t hue_delta = (uint16_t) (DUEL_HUE_DELTA * cos_table[normalized_led_idx]);
    return pat_strip->ColorHSV(start_hue + hue_delta);
}

uint32_t rainbow(int normalized_led_idx, float intra_pattern_val)
{
    float hue = ((float) normalized_led_idx / LED_COUNT) * 0xFFFF;
    return pat_strip->ColorHSV(hue);
}

uint32_t white(int normalized_led_idx, float intra_pattern_val)
{
    UNUSED(normalized_led_idx);
    UNUSED(intra_pattern_val);
    return 0xFFFFFFFF;
}