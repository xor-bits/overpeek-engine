#pragma once

#define MAX_CREATURES 256

#include "tile.h"
#include "../logic/game.h"
#include "../../engine.h"
#include "../creatures/creature.h"

class Region {
private:
	int m_x, m_y;
	Tile m_tiles[REGION_SIZE][REGION_SIZE];
	Creature *m_creatures[MAX_CREATURES];
	glm::fvec4 m_texture_off_array[REGION_SIZE * REGION_SIZE * 6 * 2];
	int m_biome;

	int m_renderIdArray[REGION_SIZE][REGION_SIZE];
	int m_renderIdObjectArray[REGION_SIZE][REGION_SIZE];

	void createTiles();
	void loadTiles(unsigned char *tileData, float *creatureData);

	std::string getSaveLocation();

public:
	bool null;

	Region(int x, int y);
	Region();
	~Region();

	void saveTiles();
	void update();
	void tilesChanged();
	void deleteRegion();
	void submitToRenderer(graphics::Renderer *renderer, float offx, float offy);
	void submitCreaturesToRenderer(graphics::Renderer *renderer, float offx, float offy);
	void debugCeilCreatures();

	void addCreature(float x, float y, int id, bool item);
	void addCreature(Creature *creature);
	void removeCreature(int i, bool _delete);
	void removeCreature(Creature *creature, bool _delete);

	inline void submitToRenderArray(unsigned int x, unsigned int y, int id, int objid) { m_renderIdArray[x][y] = id; m_renderIdObjectArray[x][y] = objid; }

	Tile* getTile(unsigned int x, unsigned int y);
	inline int getX() { return (m_x - floor(RENDER_DST / 2.0)) * REGION_SIZE; }
	inline int getY() { return (m_y - floor(RENDER_DST / 2.0)) * REGION_SIZE; }
	inline Creature* getCreature(int i) { return m_creatures[i]; }

};