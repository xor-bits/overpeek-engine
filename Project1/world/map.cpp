#include "../pch.h"

#include "map.h"

#include "../logic/game.h"
#include "../creatures/player.h"


Map::Map(std::string name) {
	m_name = name; 
	reserveSpace();
	system(("mkdir \"" + Game::getSaveLocation() + "\\" + name + "\\").c_str());
	for (int i = 0; i < M_MAP_MAX_CREATURES; i++) { m_creatures[i] = nullptr; }

#if !DEBUG_DISABLE_SAVING
	short int *tileData = (short int*)tools::BinaryIO::read<short>(Game::getSaveLocation() + "\\" + name + "\\" + "tile_data");
	if (tileData) {
		short int *objectData = (short int*)tools::BinaryIO::read<short>(Game::getSaveLocation() + "\\" + name + "\\" + "object_data");
	
		//char row[M_MAP_SIZE];
		for (int x = 0; x < M_MAP_SIZE; x++)
		{
			for (int y = 0; y < M_MAP_SIZE; y++)
			{
				m_tiles[x][y] = MapTile(tileData[x + y * M_MAP_SIZE], objectData[x + y * M_MAP_SIZE], 10);
				//row[y] = tileData[x + y * M_MAP_SIZE];
			}
			//tools::Logger::info(row);
		}
	}
	else {
#endif
		#pragma omp parallel for
		for (int x = 0; x < M_MAP_SIZE; x++)
		{
			for (int y = 0; y < M_MAP_SIZE; y++)
			{
				int tile, object;
				//tile = tools::Random::random(0, 5);
				//object = 0;
				Game::getInfoFromNoise(tile, object, x, y);
				m_tiles[x][y] = MapTile(tile, object, 1.0);
			}
		}
#if !DEBUG_DISABLE_SAVING
	}
#endif
}

void Map::reserveSpace() {
	for (int x = 0; x < M_MAP_SIZE; x++)
	{
		std::vector<MapTile> tmp1;
		for (int y = 0; y < M_MAP_SIZE; y++)
		{
			tmp1.push_back(MapTile(0, 0, 0));
		}
		m_tiles.push_back(tmp1);
	}
}

void Map::save() {
	std::vector<short> tiles;
	std::vector<short> objects;

	for (int x = 0; x < M_MAP_SIZE; x++)
	{
		for (int y = 0; y < M_MAP_SIZE; y++)
		{
			tiles.push_back(m_tiles[x][y].m_tile);
			objects.push_back(m_tiles[x][y].m_object);
		}
	}

	tools::BinaryIO::write<short int>(Game::getSaveLocation() + "\\" + m_name + "\\" + "tile_data", &tiles[0], M_MAP_SIZE * M_MAP_SIZE);
	tools::BinaryIO::write<short int>(Game::getSaveLocation() + "\\" + m_name + "\\" + "object_data", &objects[0], M_MAP_SIZE * M_MAP_SIZE);
}

void Map::hit(unsigned int x, unsigned int y, short dmg) {
	m_tiles[x][y].m_objectHealth -= dmg;
	if (m_tiles[x][y].m_objectHealth <= 0) {
		Game::addCreature(x, y, Database::items[Database::objects[m_tiles[x][y].m_object].dropsAs].id, true);
		m_tiles[x][y].m_object = 0;
	}
	Game::tilesChanged = true;
}

int Map::getObjectTexture(unsigned int x, unsigned int y) {
	MapTile *thistile = getTile(x, y);
	if (!thistile) return 0;

	if (Database::objects[thistile->m_object].multitexture) {
		MapTile* right = getTile(x + 1, y);
		MapTile* top = getTile(x, y - 1);
		MapTile* left = getTile(x - 1, y);
		MapTile* bottom = getTile(x, y + 1);

		MapTile* topright =	getTile(x + 1, y - 1);
		MapTile* topleft = getTile(x - 1, y - 1);
		MapTile* bottomleft = getTile(x - 1, y + 1);
		MapTile* bottomright = getTile(x + 1, y + 1);

		bool rightAir = right && !Database::objects[right->m_object].wall;
		bool topAir = top && !Database::objects[top->m_object].wall;
		bool leftAir = left && !Database::objects[left->m_object].wall;
		bool bottomAir = bottom && !Database::objects[bottom->m_object].wall;

		bool topRightAir = topright && !Database::objects[topright->m_object].wall;
		bool topLeftAir = topleft && !Database::objects[topleft->m_object].wall;
		bool bottomLeftAir = bottomleft && !Database::objects[bottomleft->m_object].wall;
		bool bottomRightAir = bottomright && !Database::objects[bottomright->m_object].wall;

		return Database::objects[thistile->m_object].getTexture(rightAir, topAir, leftAir, bottomAir, topRightAir, topLeftAir, bottomLeftAir, bottomRightAir);
	}
	return Database::objects[thistile->m_object].texture;
}

void Map::submitToRenderer(graphics::Renderer *renderer) {
	
	for (int x = -RENDER_HORIZONTAL; x < RENDER_HORIZONTAL; x++)
	{
		for (int y = -RENDER_VERTICAL; y < RENDER_VERTICAL; y++)
		{
			Database::Tile tile = Database::tiles[getTile(x + Game::getPlayer()->getX(), y + Game::getPlayer()->getY())->m_tile];
			Database::Object object = Database::objects[getTile(x + Game::getPlayer()->getX(), y + Game::getPlayer()->getY())->m_object];
			float rx = (x + (floor( Game::getPlayer()->getX()) -  Game::getPlayer()->getX())) * TILE_SIZE;
			float ry = (y + (floor( Game::getPlayer()->getY()) -  Game::getPlayer()->getY())) * TILE_SIZE;
			renderer->renderBox(rx, ry, TILE_SIZE, TILE_SIZE, 0, tile.texture, glm::vec4(1.0));
			renderer->renderBox(rx, ry, TILE_SIZE, TILE_SIZE, 0, getObjectTexture(x + Game::getPlayer()->getX(), y + Game::getPlayer()->getY()), glm::vec4(object.color, 1.0));
		}
	}

	for (int i = 0; i < M_MAP_MAX_CREATURES; i++)
	{
		if (m_creatures[i]) m_creatures[i]->submitToRenderer(renderer, -Game::getPlayer()->getX(), -Game::getPlayer()->getY());
	}
}

void Map::debugCeilCreatures() {
	for (int i = 0; i < M_MAP_MAX_CREATURES; i++)
	{
		if (m_creatures[i]) {
			m_creatures[i]->setX(round(m_creatures[i]->getX()));
			m_creatures[i]->setY(round(m_creatures[i]->getY()));

			MapTile* tmp;
			tmp = getTile(m_creatures[i]->getX() + 1, m_creatures[i]->getY());
			if (tmp) tmp->m_object = 4;
			tmp = getTile(m_creatures[i]->getX() - 1, m_creatures[i]->getY());
			if (tmp) tmp->m_object = 4;
			tmp = getTile(m_creatures[i]->getX(), m_creatures[i]->getY() + 1);
			if (tmp) tmp->m_object = 4;
			tmp = getTile(m_creatures[i]->getX(), m_creatures[i]->getY() - 1);
			if (tmp) tmp->m_object = 4;

			return;
		}
	}
}

void Map::addCreature(float x, float y, int id, bool item) {
	for (int i = 0; i < M_MAP_MAX_CREATURES; i++)
	{
		if (!m_creatures[i]) {
			m_creatures[i] = new Creature(x, y, id, item);
			m_creatures[i]->setRegionIndex(i);
			return;
		}
	}
}

void Map::addCreature(Creature *creature) {
	for (int i = 0; i < M_MAP_MAX_CREATURES; i++)
	{
		if (!m_creatures[i]) {
			m_creatures[i] = creature;
			m_creatures[i]->setRegionIndex(i);
			return;
		}
	}
}

void Map::removeCreature(int i, bool _delete) {
	if (!m_creatures[i]) return;

	if (_delete) delete m_creatures[i];
	m_creatures[i] = nullptr;
}

void Map::removeCreature(Creature *creature, bool _delete) {
	for (int i = 0; i < M_MAP_MAX_CREATURES; i++)
	{
		if (m_creatures[i] == creature) {
			m_creatures[i] = nullptr;
			return;
		}
	}

	char buff[100];
	snprintf(buff, sizeof(buff), "%p", (void*)creature);
	std::string buffAsStdStr = buff;
	tools::Logger::critical("Couldn't find creature: " + buffAsStdStr + "!");
}

void Map::update() {
	int n = 0;
	for (int i = 0; i < M_MAP_MAX_CREATURES; i++)
	{
		if (m_creatures[i]) {
			n++;
			m_creatures[i]->update();
		}
	}
	tools::Logger::info(n);
}

Map::MapTile* Map::getTile(unsigned int x, unsigned int y) {
	if (x >= M_MAP_SIZE) return nullptr;
	else if (x < 0) return nullptr;
	if (y >= M_MAP_SIZE) return nullptr;
	else if (y < 0) return nullptr;

	return &m_tiles.at(x).at(y);
}

void Map::findAllCreatures(float _x, float _y, Creature** _array, unsigned int& _amount, float _radius) {
	_amount = 0;
	for (int i = 0; i < M_MAP_MAX_CREATURES; i++)
	{
		if (m_creatures[i]) {
			if (!(m_creatures[i]->getX() >= _x - _radius)) continue;
			if (!(m_creatures[i]->getX() <= _x + _radius)) continue;
			if (!(m_creatures[i]->getY() >= _y - _radius)) continue;
			if (!(m_creatures[i]->getY() <= _y + _radius)) continue;
			
			_array[_amount] = m_creatures[i];
			_amount++;
		}
	}
}