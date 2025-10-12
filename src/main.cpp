#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <stdlib.h>
#include "meta_pixel.h"
#include "gol.h"

// pin connections!
#define TFT_CS	5
#define TFT_DC	18
#define TFT_RST	17

// draws the game of life pattern to the screen each frame
void drawLifeGrid(int grid_width, int grid_height, bool** current_life_grid) {
	for (int x = 0; x < grid_width; x++) {
		for (int y = 0; y < grid_height; y++) {
			bool cell = current_life_grid[x][y];

			if (cell) {
				MP_drawPixel(x, y, WHITE_TFT);
			} else {
				MP_drawPixel(x, y, BLACK_TFT);
			}
		}
	}
}

// display setup
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void setup() {
	// initialize screen
	tft.begin();
	tft.setRotation(MP_SCREEN_ROTATION);
	MP_clearFrameBuffer();
	MP_clearScreen();

	// initialize MP tool
	MP_initialize(4);

	// initialize game of life grid by allocating a properly sized array sized to fit the screen
	setLifeGridDimensions(MP_getDisplayWidth(), MP_getDisplayHeight());

	// fills the first generation of tiles with a random distribution of cells
	fillRandom();
}


void loop() {
	calculateLife();
	
	drawLifeGrid(life_grid_width, life_grid_height, life_grid);

	MP_displayFrame();

	delay(50);
}