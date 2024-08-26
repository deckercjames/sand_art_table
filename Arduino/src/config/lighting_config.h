
#ifndef LIGHTING_CONFIG_H__
#define LIGHTING_CONFIG_H__

// Input Pins
#define LED_BIGHTNESS_PIN (A0)
#define LED_SPEED_PIN     (A1)
#define LED_PATTERN_PIN   (A2)

#define INVERT_BRIGHTNESS_POT true
#define INVERT_SPEED_POT      true
#define INVERT_PATTERN_POT    false

// Output Pins
#define LED_STRIP_PIN 2

// Strip Properties
#define LED_COUNT 176

// Max speed in LEDs / Sec.
#define SPEED_MODIFIER 15.0f

#endif // LIGHTING_CONFIG_H__