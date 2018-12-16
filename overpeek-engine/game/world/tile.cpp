#include "tile.h"

#include "region.h"
#include "../logic/game.h"

Tile::Tile(long int x, long int y, int id, int object_id, unsigned int localX, unsigned int localY) {
	m_id = id; m_object_id = object_id; m_x = x; m_y = y; m_localX = localX; m_localY = localY;
	m_object_health = 1.0; m_real = true;
}

Tile::Tile() {
	m_id = 0; m_object_id = 0; m_x = 0; m_y = 0;
	m_object_health = 0.0; m_real = false;
}

Tile::~Tile() {

}

int Tile::getTexture() {
	if (!m_real) return 0;

	return Database::tiles[m_id].texture;
}

int Tile::getObjectTexture() {
	if (!m_real) return 0;

	

	if (Database::objects[m_object_id].multitexture) {
		Tile* right = Game::getTile(m_x + 1, m_y, "from tile to get right texture");
		Tile* top = Game::getTile(m_x, m_y - 1, "from tile to get right texture");
		Tile* left = Game::getTile(m_x - 1, m_y, "from tile to get right texture");
		Tile* bottom = Game::getTile(m_x, m_y + 1, "from tile to get right texture");

		Tile* topright = Game::getTile(m_x + 1, m_y - 1, "from tile to get right texture");
		Tile* topleft = Game::getTile(m_x - 1, m_y - 1, "from tile to get right texture");
		Tile* bottomleft = Game::getTile(m_x - 1, m_y + 1, "from tile to get right texture");
		Tile* bottomright = Game::getTile(m_x + 1, m_y + 1, "from tile to get right texture");

		bool rightAir = right && !Database::objects[right->getObjectId()].wall;
		bool topAir = top && !Database::objects[top->getObjectId()].wall;
		bool leftAir = left && !Database::objects[left->getObjectId()].wall;
		bool bottomAir = bottom && !Database::objects[bottom->getObjectId()].wall;

		bool topRightAir = topright && !Database::objects[topright->getObjectId()].wall;
		bool topLeftAir = topleft && !Database::objects[topleft->getObjectId()].wall;
		bool bottomLeftAir = bottomleft && !Database::objects[bottomleft->getObjectId()].wall;
		bool bottomRightAir = bottomright && !Database::objects[bottomright->getObjectId()].wall;

		return Database::objects[m_object_id].getTexture(rightAir, topAir, leftAir, bottomAir, topRightAir, topLeftAir, bottomLeftAir, bottomRightAir);
	}
	return Database::objects[m_object_id].getTexture(0, 0, 0, 0, 0, 0, 0, 0);
}


void Tile::hitObject(float damage) {
	m_object_health -= damage;
	if (m_object_health <= 0) {
		Game::addCreature(m_x, m_y, Database::items[Database::objects[m_object_id].dropsAs].id, true);
		m_object_id = 0;
	}
	Game::tilesChanged = true;
}

void Tile::update() {
}

void Tile::setId(int id) { 
	m_id = id;
	Game::tilesChanged = true;
}

void Tile::setObjectId(int id) { 
	m_object_id = id;
	Game::tilesChanged = true;
}

void Tile::healObject() { 
	m_object_health = 1.0;
	Game::tilesChanged = true;
}