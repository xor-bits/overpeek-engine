#pragma once

#define SIZE 16
#define TILE_SIZE 0.1

#include "tile.h"

class Region {
private:
	int m_x, m_y;
	Tile *m_tiles[SIZE][SIZE];

	void createTiles();

public:
	Region(int x, int y);

	void render();
	void update();
};