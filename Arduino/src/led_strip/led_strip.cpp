
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

static void _duel_hue(int start_led, unsigned int start_hue, unsigned int hue_delta);

void update_led_strip()
{
    _duel_hue(0, 0, 5);
}


static void _duel_hue(int start_led, unsigned int start_hue, unsigned int hue_delta)
{
    for (int i = 0; i < LED_COUNT; i++)
    {
        int hue = (int) (start_hue + ( (float) i / LED_COUNT) * hue_delta);
        int c = strip.gamma32(strip.ColorHSV(hue));
        int l = (i + start_led) % LED_COUNT;
        strip.setPixelColor(l, c);
    }
}
