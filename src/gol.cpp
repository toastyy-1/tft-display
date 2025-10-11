#include "gol.h"

// contains the currently living cells
bool** life_grid = nullptr;
// the construction layer for the next batch of cells
bool** next_grid = nullptr;

// dimensions of the playing field
int life_grid_width;
int life_grid_height;

// initialize game of life grid by allocating a properly sized array sized to fit the screen
void setLifeGridDimensions(int grid_width, int grid_height) {
    life_grid_width = grid_width;
    life_grid_height = grid_height;

    life_grid = (bool**)malloc(life_grid_width * sizeof(bool*));
	next_grid = (bool**)malloc(life_grid_width * sizeof(bool*));
    
	for (int i = 0; i < grid_width; i++) {
		life_grid[i] = (bool*)malloc(life_grid_height * sizeof(bool));
		next_grid[i] = (bool*)malloc(life_grid_height * sizeof(bool));
	}
}

// fill the life grid with a random distribution of alive/not alive cells
void fillRandom() {
    randomSeed(analogRead(0));
	for (int x = 0; x < life_grid_width; x++) {
		for (int y = 0; y < life_grid_height; y++) {
			life_grid[x][y] = (random(100) < 15);
			next_grid[x][y] = false;
		}
	}
}

// function that should be run each time in the loop: updates the game of life playing board
void calculateLife() {
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

	bool** tmp = life_grid;
	life_grid = next_grid;
	next_grid = tmp;
}