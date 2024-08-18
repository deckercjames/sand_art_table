
#include "led_strip.h"

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "../config/config.h"

Adafruit_NeoPixel strip(LED_COUNT, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);


void init_led_strip()
{
    strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    strip.show();            // Turn OFF all pixels ASAP
    strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
}

static void _duel_hue(unsigned int start_led, unsigned int start_hue, unsigned int hue_delta);



void update_led_strip()
{
    // Read Pot Values
    int brightness_val = analogRead(LED_BIGHTNESS_PIN);
    int speed_val      = analogRead(LED_SPEED_PIN);
    int pattern_val    = analogRead(LED_PATTERN_PIN);
    
    // Set brightness
    strip.setBrightness(brightness_val >> 2);
    
    unsigned long m = millis();

    int anchor_led_idk = (m * LED_COUNT / speed_val);
    
    anchor_led_idk /= 3;
    
    unsigned int start_hue = pattern_val << 6;
    
    _duel_hue(anchor_led_idk, start_hue, 13553);
    
    strip.show();
}


static void _duel_hue(unsigned int start_led, unsigned int start_hue, unsigned int hue_delta)
{
    for (int i = 0; i <= (LED_COUNT / 2); i++)
    {
        int hue = (int) (start_hue + ((float) i * 2 / LED_COUNT) * hue_delta);
        uint32_t color = strip.gamma32(strip.ColorHSV(hue));
        int led_idx_0 = (i + start_led) % LED_COUNT;
        int led_idx_1 = (LED_COUNT - i + start_led) % LED_COUNT;
        strip.setPixelColor(led_idx_0, color);
        strip.setPixelColor(led_idx_1, color);
    }
}
