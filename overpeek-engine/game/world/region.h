#pragma once

#define NOISE_SCALE 0.02

#define LEVEL_WATER 0.5
#define LEVEL_SAND 0.52
#define LEVEL_SOIL 0.7
#define LEVEL_STONE 0.72

#define MAX_CREATURES 128

#include "tile.h"
#include "../logic/game.h"
#include "../../engine.h"
#include "../creatures/creature.h"

class Region {
private:
	int m_x, m_y;
	Tile *m_tiles[REGION_SIZE][REGION_SIZE];
	Creature *m_creatures[MAX_CREATURES];
	glm::fvec4 m_texture_off_array[REGION_SIZE * REGION_SIZE * 6 * 2];

	void createTiles();
	void loadTiles();

	inline std::string getSaveLocation() {
		std::string regionname = std::to_string(m_x) + ", " + std::to_string(m_y);
		return (SAVE_PATH + WORLD_NAME + "\\regions\\" + regionname).c_str();
	}

public:

	Region(int x, int y);
	~Region();

	void saveTiles();
	void update();
	void submitToRenderer(graphics::Renderer *renderer, float offx, float offy);
	void submitCreaturesToRenderer(graphics::Renderer *renderer, float offx, float offy);

	inline Tile* getTile(unsigned int x, unsigned int y) { return m_tiles[x][y]; }

	void addCreature(float x, float y, int id, bool item) {
		for (int i = 0; i < MAX_CREATURES; i++)
		{
			if (!m_creatures[i]) {
				m_creatures[i] = new Creature(x, y, id, item, this);
				return;
			}
		}
	}

	void addCreature(Creature *creature) {
		for (int i = 0; i < MAX_CREATURES; i++)
		{
			if (!m_creatures[i]) {
				m_creatures[i] = creature;
				return;
			}
		}
	}

	void removeCreature(int i) { 
		m_creatures[i] = nullptr;
	}

	void removeCreature(Creature *creature) {
		for (int i = 0; i < MAX_CREATURES; i++)
		{
			if (m_creatures[i] == creature) {
				m_creatures[i] = nullptr;
				std::cout << "Removed!" << std::endl;
				return;
			}
		}
	}

	inline int getX() { return (m_x - floor(RENDER_DST/2.0)) * REGION_SIZE; }
	inline int getY() { return (m_y - floor(RENDER_DST/2.0)) * REGION_SIZE; }

};