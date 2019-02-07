#include "../pch.h"


#include "player.h"

#include "../logic/game.h"
#include "../logic/inventory.h"
#include "../world/tile.h"
#include "../world/region.h"
#include "../world/map.h"

Player::Player(float x, float y, Inventory *inv) : Creature(x, y, 0, false) {
	inventory = inv;
}

void Player::submitToRenderer(graphics::Renderer *renderer, float renderOffsetX, float renderOffsetY) {
	Creature::submitToRenderer(renderer, renderOffsetX, renderOffsetY);
}

void Player::update() {
	Creature::update(0);
}

void Player::collide() {
	Creature::collide();
}

void Player::hit() {
	Creature::hit();
}

void Player::place() {
#if !STORE_MAP_IN_RAM
	Tile *tmp;
	switch (heading)
	{
	case 0:
		tmp = Game::getTile(getX(), getY() - 1, "player");
		break;
	case 1:
		tmp = Game::getTile(getX() + 1, getY(), "player");
		break;
	case 2:
		tmp = Game::getTile(getX(), getY() + 1, "player");
		break;
	default:
		tmp = Game::getTile(getX() - 1, getY(), "player");
		break;
	}

	if (tmp) {
		Game::trySetTileObject(tmp, 4);
	}
#else
	Map::MapTile* tmp;
	switch (heading)
	{
	case 0:
		tmp = Game::getMap()->getTile(getX(), getY() - 1);
		//m_swingDir = 1;
		break;
	case 1:
		tmp = Game::getMap()->getTile(getX() + 1, getY());
		//m_swingDir = 2;
		break;
	case 2:
		tmp = Game::getMap()->getTile(getX(), getY() + 1);
		//m_swingDir = 3;
		break;
	default:
		tmp = Game::getMap()->getTile(getX() - 1, getY());
		//m_swingDir = 4;
		break;
	}

	if (tmp) {
		///REDO TILE SET ASKING
		tmp->m_object = 4;
		//Game::trySetTileObject(tmp, 4);
	}
#endif
	audio::AudioManager::play(1);
}

void Player::save() {
	inventory->save();
	float playerData[2] = { getX(), getY() };
	tools::BinaryIO::write<float>(Game::getSaveLocation() + "player.data", playerData, 2);
}

bool Player::load() {
	inventory->init();

	unsigned long data_size;
	float *playerData = (float*)tools::BinaryIO::read<float>(Game::getSaveLocation() + "player.data", data_size);
	if (!playerData) return false;

	setX(playerData[0]);
	setY(playerData[1]);
}