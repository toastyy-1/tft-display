#include "meta_pixel.h"
#include <cstdlib>

int MP_BLOCK_SIZE = 0;
int META_WIDTH = 0;
int META_HEIGHT = 0;
uint16_t** MP_frame_buffer = nullptr;

// returns dimensions of MP display
int MP_getDisplayWidth() {
    return 320 / MP_BLOCK_SIZE;
}
int MP_getDisplayHeight() {
    return 240 / MP_BLOCK_SIZE;
}

// frees up the memory for the frame buffer when its done being used
void MP_freeFrameBuffer() {
    if (MP_frame_buffer != nullptr) {
        for (int i = 0; i < META_WIDTH; i++) {
            free(MP_frame_buffer[i]);
        }
        free(MP_frame_buffer);
        MP_frame_buffer = nullptr;
        MP_BLOCK_SIZE = 0;
        META_WIDTH = 0;
        META_HEIGHT = 0;
    }
}

// initializes the size of the meta display based on the input block size
void MP_initialize(int block_size) {
    MP_freeFrameBuffer();

    MP_BLOCK_SIZE = block_size;

    META_WIDTH = MP_getDisplayWidth();
    META_HEIGHT = MP_getDisplayHeight();

    MP_frame_buffer = (uint16_t**)malloc(META_WIDTH * sizeof(uint16_t*));
    for (int i = 0; i < META_WIDTH; i++) {
        MP_frame_buffer[i] = (uint16_t*)malloc(META_HEIGHT * sizeof(uint16_t));
        for (int j = 0; j < META_HEIGHT; j++) {
            MP_frame_buffer[i][j] = BLACK_TFT;
        }
    }
}

// clear screen
void MP_clearScreen() {
	tft.fillScreen(BLACK_TFT);
}

// clear the frame buffer
void MP_clearFrameBuffer() {
    for (int x = 0; x < META_WIDTH; x++) {
        for (int y = 0; y < META_HEIGHT; y++) {
            MP_frame_buffer[x][y] = BLACK_TFT;
        }
    }
}

// displays all pixels stored in a frame buffer for the display
void MP_displayFrame() {
	for (int y = 0; y < META_HEIGHT; y++) {
		for (int x = 0; x < META_WIDTH; x++) {
			tft.fillRect(x * MP_BLOCK_SIZE, y * MP_BLOCK_SIZE, 
                         MP_BLOCK_SIZE, MP_BLOCK_SIZE, MP_frame_buffer[x][y]);
		}
	}
}

// draws a pixel to the frame buffer
void MP_drawPixel(int x, int y, uint16_t color) {
    MP_frame_buffer[x][y] = color;
}

// uses this thing called Bresenham's line algorithm to draw a line
void MP_drawLine(int x0, int y0, int x1, int y1, uint16_t color) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    int step_x;
    if (x0 < x1) { step_x = 1; } else { step_x = -1; }
    int step_y;
    if (y0 < y1) { step_y = 1; } else { step_y = -1; }

    int diff = dx - dy;

    while (true) {
        if (x0 >= 0 && x0 < META_WIDTH && y0 >= 0 && y0 < META_HEIGHT) {
            MP_frame_buffer[x0][y0] = color;
        }

        if (x0 == x1 && y0 == y1) break;

        int diff2 = 2 * diff;
        if (diff2 > -dy) {
            diff -= dy;
            x0 += step_x;
        }
        if (diff2 < dx) {
            diff += dx;
            y0 += step_y;
        }
    }
}

uint16_t randomColor() {
  uint8_t r = random(0, 32);
  uint8_t g = random(0, 64);
  uint8_t b = random(0, 32);

  return (r << 11) | (g << 5) | b;
}