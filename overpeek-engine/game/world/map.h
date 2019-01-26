#pragma once

#include "../../engine.h"

#define M_MAP_SIZE 2000
#define M_MAP_MAX_CREATURES 2048

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
		};
		Creature *m_creatures[M_MAP_MAX_CREATURES];

	private:
		std::vector<std::vector<MapTile>> m_tiles;
		std::string m_name;

		void reserveSpace();

	public:
		Map(std::string name);

		void save();
		void hit(unsigned int x, unsigned int y, short dmg);
		int getObjectTexture(unsigned int x, unsigned int y);
		void submitToRenderer(graphics::Renderer *renderer);
		void debugCeilCreatures();
		void update();

		void addCreature(float x, float y, int id, bool item);
		void addCreature(Creature *creature);
		void removeCreature(int i, bool _delete);
		void removeCreature(Creature *creature, bool _delete);

		MapTile* getTile(unsigned int x, unsigned int y);

		void findAllCreatures(float x, float y, Creature** array, unsigned int& amount, float radius);

};