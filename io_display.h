#ifndef IO_DISPLAY_H_
#define IO_DISPLAY_H_

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include "io_audio.h"
#include "io_display_util.h"
#include "io_instrument.h"

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_W, SCREEN_H, &Wire, OLED_RESET);

void displayInit()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC,
                       0x3C))
    { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }

    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);

    display.println("Init synth...");
    display.display();
}

unsigned long lastDisplayUpdate = millis();
bool needDisplayUpdate = false;
unsigned int forceRefreshIn = 0;

void displayUpdate()
{
    if (millis() - lastDisplayUpdate >= 50)
    {
        needDisplayUpdate = false;
        lastDisplayUpdate = millis();
        getSynth()->display(&display, &forceRefreshIn);
        display.display();
    }
    else
    {
        needDisplayUpdate = true;
    }
}

void displayLoop()
{
    if (needDisplayUpdate)
    {
        displayUpdate();
    } else if (forceRefreshIn && millis() - lastDisplayUpdate > forceRefreshIn) {
        forceRefreshIn = 0;
        displayUpdate();
    }
}

#endif
