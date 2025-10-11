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

// display setup
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// array that will hold the game of life simulation
bool** life_grid = nullptr;
bool** next_grid = nullptr;

int life_grid_width;
int life_grid_height;

void setup() {
	// initialize screen
	tft.begin();
	tft.setRotation(MP_SCREEN_ROTATION);
	MP_clearScreen();

	// initialize MP tool
	MP_initialize(8);

	// matches the size of the screen to the size of the simulation
	life_grid_width = MP_getDisplayWidth();
	life_grid_height = MP_getDisplayHeight();

	// initialize game of life grid by allocating a properly sized array sized to fit the screen
	life_grid = (bool**)malloc(life_grid_width * sizeof(bool*));
	next_grid = (bool**)malloc(life_grid_width * sizeof(bool*));
	for (int i = 0; i < life_grid_width; i++) {
		life_grid[i] = (bool*)malloc(life_grid_height * sizeof(bool));
		next_grid[i] = (bool*)malloc(life_grid_height * sizeof(bool));
	}

	// fills the first generation of tiles with a random distribution of cells
	randomSeed(analogRead(0));
	for (int x = 0; x < life_grid_width; x++) {
		for (int y = 0; y < life_grid_height; y++) {
			life_grid[x][y] = (random(100) < 15);
			next_grid[x][y] = false;
		}
	}

	MP_clearFrameBuffer();
	for (int x = 0; x < life_grid_width; x++) {
		for (int y = 0; y < life_grid_width; y++) {
			if (life_grid[x][y]) MP_drawPixel(x, y, WHITE_TFT);
		}
	}
	MP_displayFrame();
}


void loop() {
	for (int x = 0; x < life_grid_width; x++) {
		for (int y = 0; y < life_grid_height; y++) {
			uint8_t live_neighbor_count = 0;
			for (int offset_x = -1; offset_x <= 1; offset_x++) {
				for (int offset_y = -1; offset_y <= 1; offset_y++) {
					if (offset_x == 0 && offset_y == 0) continue; // skips the cell its currently evaluating
					int neighbor_x = x + offset_x;
					int neighbor_y = y + offset_y;

					if (neighbor_x < 0) neighbor_x = life_grid_width - 1;
					if (neighbor_x >= life_grid_width) neighbor_x = 0;
					if (neighbor_y < 0) neighbor_y = life_grid_height - 1;
					if (neighbor_y >= life_grid_height) neighbor_y = 0;
					live_neighbor_count += life_grid[neighbor_x][neighbor_y] ? 1 : 0; 
				}
			}

			bool is_currently_alive = life_grid[x][y];
			bool will_be_alive = (is_currently_alive && (live_neighbor_count == 2 || live_neighbor_count == 3)) ||
        						  (!is_currently_alive && live_neighbor_count == 3);
			next_grid[x][y] = will_be_alive;
		}
	}

	for (int x = 0; x < life_grid_width; x++) {
		for (int y = 0; y < life_grid_height; y++) {
			bool was_alive = life_grid[x][y];
			bool now_alive = next_grid[x][y];

			if (now_alive != was_alive) {
				if (now_alive) {
					MP_drawPixel(x, y, WHITE_TFT);
				} else {
					MP_drawPixel(x, y, BLACK_TFT);
				}
			}

			life_grid[x][y] = now_alive;
		}
	}

	bool** tmp = life_grid;
	life_grid = next_grid;
	next_grid = tmp;

	MP_displayFrame();

	delay(50);
}