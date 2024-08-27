
#ifndef LED_PATTERNS__
#define LED_PATTERNS__

#include <stdint.h>

void init_pattern_helpers();

uint32_t duel_hue(int normalized_led_idx, float intra_pattern_val);
uint32_t rainbow(int normalized_led_idx, float intra_pattern_val);
uint32_t white(int normalized_led_idx, float intra_pattern_val);

#endif //LED_PATTERNS__
