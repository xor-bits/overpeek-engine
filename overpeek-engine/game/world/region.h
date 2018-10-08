#pragma once

#define NOISE_SCALE 0.02

#define LEVEL_WATER 0.5
#define LEVEL_SAND 0.52
#define LEVEL_GRASS 0.7
#define LEVEL_STONE 0.72

#define MAX_CREATURES 128

#include "tile.h"
#include "../logic/game.h"
#include "../../engine.h"
#include "../creatures/creature.h"

class Region {
private:
	int m_x, m_y;
	Tile m_tiles[REGION_SIZE][REGION_SIZE];
	Creature *m_creatures[MAX_CREATURES];
	unsigned int m_creatureAmount;
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

	inline Tile getTile(unsigned int x, unsigned int y) { return m_tiles[x][y]; }
	inline int getTileId(unsigned int x, unsigned int y) { return m_tiles[x][y].getId(); }
	inline int getTileObjectId(unsigned int x, unsigned int y) { return m_tiles[x][y].getObjectId(); }

	inline void setTileId(unsigned int x, unsigned int y, int id) { m_tiles[x][y].setId(id); }
	inline void setTileObjectId(unsigned int x, unsigned int y, int id) { m_tiles[x][y].setObjectId(id); }
	inline void addObjectHealth(unsigned int x, unsigned int y, float amount) { m_tiles[x][y].addObjectHealth(amount); }
	inline void healObject(unsigned int x, unsigned int y) { m_tiles[x][y].healObject(); }

	inline int getTileX(unsigned int x, unsigned int y) { return m_tiles[x][y].getX(); }
	inline int getTileY(unsigned int x, unsigned int y) { return m_tiles[x][y].getY(); }

	inline void addCreature(Creature *creature) { m_creatures[m_creatureAmount++] = creature; }
	inline void removeCreature(Creature *creature) { m_creatures[m_creatureAmount++] = creature; }

	inline int getX() { return (m_x - floor(RENDER_DST/2.0)) * REGION_SIZE; }
	inline int getY() { return (m_y - floor(RENDER_DST/2.0)) * REGION_SIZE; }

};