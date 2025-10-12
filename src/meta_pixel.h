#ifndef META_PIXEL_H
#define META_PIXEL_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

extern Adafruit_ILI9341 tft;

// color definitions
#define BLACK_TFT	0x0000
#define WHITE_TFT	0xFFFF
#define RED_TFT		0xF800
#define GREEN_TFT	0x07E0
#define BLUE_TFT 	0x001F

#define MP_SCREEN_ROTATION 3 // this rotation assumes the origin is in the TOP LEFT corner of the screen

// frame buffer
extern uint16_t* MP_frame_buffer;
extern uint16_t* MP_scaled_buffer;
// "pixel" display
extern int MP_BLOCK_SIZE;

// pin connections
const int TFT_CS = 5;
const int TFT_DC = 18;
const int TFT_RST = 17;

int MP_getDisplayWidth();
int MP_getDisplayHeight();

void MP_freeFrameBuffer();
void MP_initialize(int block_size);

void MP_clearScreen();
void MP_displayFrame();
void MP_clearFrameBuffer();
void MP_drawPixel(int x, int y, uint16_t color);
void MP_drawLine(int x0, int y0, int x1, int y1, uint16_t color);
uint16_t randomColor();

void drawFullScreen(uint16_t** input_frame_buffer);

#endif