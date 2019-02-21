#pragma once

#include <vector>
#include "../settings.h"
#include "../logic/database.h"

class Creature;
class Map {
	public:
		struct MapTile
		{
			short int m_tile;
			short int m_object;
			short int m_objectHealth;
			MapTile(short int tile, short int object, short hp) {
				m_tile = tile;
				m_object = object;
				m_objectHealth = hp;
			}
			MapTile() {
				m_tile = 0;
				m_object = 0;
				m_objectHealth = 0;
			}
		};

	private:
		struct CreatureSaveData
		{
			float x;
			float y;
			short id;
			bool item;
		};
		std::vector<std::unique_ptr<Creature>> m_creatures;
		std::vector<std::vector<std::unique_ptr<MapTile>>>  m_tiles;
		std::string m_name;

		float* m_mapnoise;
		float* m_biomenoise1;
		float* m_biomenoise2;
		float* m_plantnoise1;
		float* m_plantnoise2;

		bool loadMap();
		void createMapFast();

		int getInfoFromNoiseIfLoop(Database::Biome *biome, float x, float y, int index);
		void getInfoFromNoise(int &tileId, int &objId, float x, float y);
		Database::Biome *getTileBiome(float x, float y);
		
		std::string saveLocation();

	public:
		Map(std::string name);

		void save();
		void hit(unsigned int x, unsigned int y, short dmg);
		int getObjectTexture(unsigned int x, unsigned int y);
		void submitToRenderer(oe::Renderer *renderer, float offX, float offY);
		void debugCeilCreatures();
		void update();

		Creature *addCreature(float x, float y, int id, bool item);
		Creature *itemDrop(float x, float y, int id);
		void removeCreature(int index);
		void removeCreature(Creature *creature);

		MapTile *getTile(unsigned int x, unsigned int y);
		int trySetObject(unsigned int x, unsigned int y, short id);
		int trySetObject(MapTile *tile, short id);

		void findAllCreatures(float x, float y, std::vector<Creature*> &_array, float radius);

};