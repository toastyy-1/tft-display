#ifndef GOL_H
#define GOL_H

#include "meta_pixel.h"
#include <stdlib.h>

// array that will hold the game of life simulation
extern bool** life_grid;
extern bool** next_grid;

extern int life_grid_width;
extern int life_grid_height;

void setLifeGridDimensions(int life_grid_width, int life_grid_height);
void fillRandom();
void calculateLife();

#endif