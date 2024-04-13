
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
    unsigned long m = millis();
    int a = (m * LED_COUNT / 1000);
    
    a /= 3;
    
    unsigned int b = (m);
    
    _duel_hue(a, b, 13553);
    
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
