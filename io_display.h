#ifndef IO_DISPLAY_H_
#define IO_DISPLAY_H_

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>

#define SCREEN_W 128 // OLED display width, in pixels
#define SCREEN_H 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)

class IO_Display
{
protected:
    Adafruit_SSD1306 d(SCREEN_W, SCREEN_H, &Wire, OLED_RESET);

    unsigned long lastDisplayUpdate = millis();
    bool needDisplayUpdate = false;
    unsigned int forceRefreshIn = 0;

    byte displayVal = 0;
    const char *displayValName = NULL;

    unsigned int msVal = 0;
    const char *msValName = NULL;

    void render(unsigned int *forceRefreshIn)
    {
        d.clearDisplay();
        d.setCursor(0, 0);

        if (displayValName) // should move this in io_audio_base
        {
            d.println(displayValName);
            d.println("");
            d.setTextSize(6);
            d.println(displayVal);
            d.setTextSize(1);
            displayValName = NULL;
            *forceRefreshIn = 2000;
        }
        else if (msValName) // should move this in io_audio_base
        {
            d.println(msValName);
            d.setTextSize(3);
            d.printf("\n%dms\n", msVal);
            d.setTextSize(1);
            msValName = NULL;
            *forceRefreshIn = 2000;
        }
        else
        {
            d.println("Just play...");
        }
    }

    void update()
    {
        if (millis() - lastDisplayUpdate >= 50)
        {
            needDisplayUpdate = false;
            lastDisplayUpdate = millis();
            render(&forceRefreshIn);
            d.display();
        }
        else
        {
            needDisplayUpdate = true;
        }
    }

public:
    void init()
    {
        // should here just have available ...??
        if (!d.begin(SSD1306_SWITCHCAPVCC,
                     0x3C))
        { // Address 0x3D for 128x64
            Serial.println(F("SSD1306 allocation failed"));
            for (;;)
                ; // Don't proceed, loop forever
        }

        d.clearDisplay();
        d.setTextColor(WHITE);
        d.setTextSize(1);
        d.setCursor(0, 0);

        d.println("Init synth...");
        d.display();
    }

    void loop()
    {
        if (needDisplayUpdate)
        {
            update();
        }
        else if (forceRefreshIn && millis() - lastDisplayUpdate > forceRefreshIn)
        {
            forceRefreshIn = 0;
            update();
        }
    }

    void displayValue(const char *name, byte value)
    {
        displayVal = value;
        displayValName = name;
    }

    void displayMs(const char *name, unsigned int value)
    {
        msVal = value;
        msValName = name;
    }
};

#endif
