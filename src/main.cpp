#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "meta_pixel.h"

// pin connections!
#define TFT_CS	5
#define TFT_DC	18
#define TFT_RST	17

// display setup
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void setup() {
	// initialize screen
	tft.begin();
	tft.setRotation(MP_SCREEN_ROTATION);
	MP_clearScreen();

	// initialize MP tool
	MP_initialize(8);

	MP_drawLine(0, 0, 30, 5, BLUE_TFT);
	MP_drawPixel(3,3,RED_TFT);

	MP_displayFrame();
}

void loop() {

}