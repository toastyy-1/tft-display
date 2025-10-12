#include "meta_pixel.h"
#include <cstdlib>

int MP_BLOCK_SIZE = 0;
int META_WIDTH = 0;
int META_HEIGHT = 0;
int TOTAL_PIXELS = 0;

uint16_t* MP_frame_buffer = nullptr;
uint16_t* MP_scaled_buffer = nullptr;

// returns dimensions of simulated MP display
int MP_getDisplayWidth() {
    return 320 / MP_BLOCK_SIZE;
}
int MP_getDisplayHeight() {
    return 240 / MP_BLOCK_SIZE;
}

// frees up the memory for the frame buffer when its done being used
void MP_freeFrameBuffer() {
    if (MP_frame_buffer != nullptr) {
        free(MP_frame_buffer);
        MP_frame_buffer = nullptr;
    }
    if (MP_scaled_buffer != nullptr) {
        free(MP_scaled_buffer);
        MP_scaled_buffer = nullptr;
    }
    MP_BLOCK_SIZE = 0;
    META_WIDTH = 0;
    META_HEIGHT = 0;
}

// initializes the size of the meta display based on the input block size
void MP_initialize(int block_size) {
    MP_freeFrameBuffer();
    MP_BLOCK_SIZE = block_size;
    META_WIDTH = MP_getDisplayWidth();
    META_HEIGHT = MP_getDisplayHeight();
    TOTAL_PIXELS = META_HEIGHT * META_WIDTH;
    
    MP_frame_buffer = (uint16_t*)malloc(TOTAL_PIXELS * sizeof(uint16_t));

    // only uses MP_scaled_buffer if BLOCK_SIZE > 1
    if (MP_BLOCK_SIZE > 1) {
        MP_scaled_buffer = (uint16_t*)malloc(320 * 240 * sizeof(uint16_t));
    }
    
    for (int i = 0; i < TOTAL_PIXELS; i++) {
        MP_frame_buffer[i] = BLACK_TFT;
    }
}

// clear screen
void MP_clearScreen() {
	tft.fillScreen(BLACK_TFT);
}

// clear the frame buffer
void MP_clearFrameBuffer() {
    for (int i = 0; i < TOTAL_PIXELS; i++) {
        MP_frame_buffer[i] = BLACK_TFT;
    }
}

// displays all pixels stored in a frame buffer for the display
void MP_displayFrame() {
    tft.startWrite();

    if (MP_BLOCK_SIZE == 1) {
        tft.setAddrWindow(0, 0, 320, 240);
        tft.writePixels(MP_frame_buffer, TOTAL_PIXELS);
    } else {
        for (int meta_y = 0; meta_y < META_HEIGHT; meta_y++) {
            for (int meta_x = 0; meta_x < META_WIDTH; meta_x++) {
                uint16_t color = MP_frame_buffer[meta_y * META_WIDTH + meta_x];

                for (int dy = 0; dy < MP_BLOCK_SIZE; dy++) {
                    for (int dx = 0; dx < MP_BLOCK_SIZE; dx++) {
                        int physical_x = meta_x * MP_BLOCK_SIZE + dx;
                        int physical_y = meta_y * MP_BLOCK_SIZE + dy;
                        MP_scaled_buffer[physical_y * 320 + physical_x] = color;
                    }
                }
            }
        }
        tft.setAddrWindow(0, 0, 320, 240);
        tft.writePixels(MP_scaled_buffer, 320 * 240);
    }
    tft.endWrite();
}

// draws a pixel to the frame buffer
void MP_drawPixel(int x, int y, uint16_t color) {
    MP_frame_buffer[y * META_WIDTH + x] = color;
}

// uses this thing called Bresenham's line algorithm to draw a line
void MP_drawLine(int x0, int y0, int x1, int y1, uint16_t color) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int step_x = (x0 < x1) ? 1 : -1;
    int step_y = (y0 < y1) ? 1 : -1;
    int diff = dx - dy;
    
    while (true) {
        if (x0 >= 0 && x0 < META_WIDTH && y0 >= 0 && y0 < META_HEIGHT) {
            MP_frame_buffer[y0 * META_WIDTH + x0] = color;
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

// generates a random color
uint16_t randomColor() {
  uint8_t r = random(0, 32);
  uint8_t g = random(0, 64);
  uint8_t b = random(0, 32);

  return (r << 11) | (g << 5) | b;
}