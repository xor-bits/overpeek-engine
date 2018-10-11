#include "tile.h"

#include "region.h"
#include "../logic/game.h"

Tile::Tile(long int x, long int y, int id, int object_id) {
	m_id = id; m_object_id = object_id; m_x = x; m_y = y;
	m_object_health = 1.0;
}

Tile::Tile() {
	m_id = 0; m_object_id = 0; m_x = 0; m_y = 0;
	m_object_health = 0.0;
}

int Tile::getTexture() {
	return Database::tiles[m_id].texture;
}

int Tile::getObjectTexture() {
	Tile* right = Game::getTile(m_x + 1, m_y);
	Tile* top = Game::getTile(m_x, m_y - 1);
	Tile* left = Game::getTile(m_x - 1, m_y);
	Tile* bottom = Game::getTile(m_x, m_y + 1);

	Tile* topright = Game::getTile(m_x + 1, m_y - 1);
	Tile* topleft = Game::getTile(m_x - 1, m_y - 1);
	Tile* bottomleft = Game::getTile(m_x - 1, m_y + 1);
	Tile* bottomright = Game::getTile(m_x + 1, m_y + 1);

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

void Tile::update() {}