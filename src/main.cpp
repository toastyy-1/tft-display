#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <stdlib.h>
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
	MP_displayFrame();

	// initialize other stuff
	randomSeed(3984756);
}

void loop() {
	MP_clearFrameBuffer();

	int randx = random(40);
	int randy = random(30);

	uint16_t rand_color = randomColor();

	MP_drawPixel(randx, randy, rand_color);

	MP_displayFrame();

	delay(100);
}