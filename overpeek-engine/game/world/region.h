#pragma once

#define NOISE_SCALE 0.01

#define LEVEL_WATER 0.0
#define LEVEL_SAND 0.05
#define LEVEL_GRASS 0.15

#include "tile.h"
#include "../logic/game.h"

class Region {
private:
	int m_x, m_y;
	Tile *m_tiles[REGION_SIZE][REGION_SIZE];

	void createTiles();
	void loadTiles();

	inline std::string getSaveLocation() {
		std::string regionname = std::to_string(m_x) + ", " + std::to_string(m_y);
		return (SAVE_PATH + WORLD_NAME + "\\regions\\" + regionname).c_str();
	}

public:
	Region(int x, int y);
	~Region();

	void render(float offx, float offy);
	void update();
	void saveTiles();

	inline Tile *getTile(unsigned int x, unsigned int y) { return m_tiles[x][y]; }
	inline int getX() { return (m_x - floor(RENDER_DST/2.0)) * REGION_SIZE; }
	inline int getY() { return (m_y - floor(RENDER_DST/2.0)) * REGION_SIZE; }

};