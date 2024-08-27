
#include "led_patterns.h"

#include <Arduino.h>

#include "utils.h"
#include "config.h"

#define DUEL_HUE_DELTA 13553

#define NO_SATURATION   (0x00 << 8)
#define FULL_SATURATION (0xFF << 8)
#define FULL_VALUE      (0xFF)

float cos_table[LED_COUNT];

void init_pattern_helpers()
{
    for (int i = 0; i < LED_COUNT; i++) {
        cos_table[i] = 0.5f - (cos(((float) i / (LED_COUNT - 1)) * PI * 2) * 0.5f);
    }
}

uint32_t duel_hue(int normalized_led_idx, float intra_pattern_val)
{
    const uint16_t start_hue = (uint16_t) (0xFFFF * intra_pattern_val);
    const uint16_t hue_delta = (uint16_t) (DUEL_HUE_DELTA * cos_table[normalized_led_idx]);
    const uint16_t hue = start_hue + hue_delta;
    return (hue << 16) | FULL_SATURATION | FULL_VALUE;
}

uint32_t rainbow(int normalized_led_idx, float intra_pattern_val)
{
    UNUSED(intra_pattern_val);
    const uint16_t hue = (uint16_t) (((float) normalized_led_idx / LED_COUNT) * 0xFFFF);
    return (hue << 16) | FULL_SATURATION | FULL_VALUE;
}

uint32_t white(int normalized_led_idx, float intra_pattern_val)
{
    UNUSED(normalized_led_idx);
    UNUSED(intra_pattern_val);
    return NO_SATURATION | FULL_VALUE;
}