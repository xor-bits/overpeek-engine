#include "player.h"
#include "../../engine.h"
#include "../logic/game.h"
#include "../world/tile.h"
#include "../world/region.h"

Player::Player(float x, float y, Inventory *inv) : Creature(x, y, 0, false, nullptr) {
	inventory = inv;
}

void Player::submitToRenderer(graphics::Renderer *renderer, float renderOffsetX, float renderOffsetY) {
	Creature::submitToRenderer(renderer, renderOffsetX, renderOffsetY);
}

void Player::update() {
	Creature::update();
}

void Player::collide() {
	if (!Game::debugMode) Creature::collide();
}

void Player::hit() {
	Creature::hit();


	float knockBack = 1.0;

	Creature* creatureArray[MAX_CREATURES];
	unsigned int amount;
	Region* thisRegion = Game::getRegion(getX(), getY());
	Game::findAllCreatures(thisRegion, getX(), getY(), creatureArray, amount);

	for (int i = 0; i < amount; i++)
	{
		glm::vec2 directionVector = glm::vec2(creatureArray[i]->getX() - getX(), creatureArray[i]->getY() - getY());
		directionVector = glm::normalize(directionVector);

		creatureArray[i]->acc_x = directionVector.x;
		creatureArray[i]->acc_y = directionVector.y;

		std::cout << directionVector.x << ", " << directionVector.y << std::endl;
	}
}

void Player::place() {
	Tile* tmp;
	switch (heading)
	{
	case 0:
		tmp = Game::getTile(getX(), getY() - 1, "from player place tile");
		//m_swingDir = 1;
		break;
	case 1:
		tmp = Game::getTile(getX() + 1, getY(), "from player place tile");
		//m_swingDir = 2;
		break;
	case 2:
		tmp = Game::getTile(getX(), getY() + 1, "from player place tile");
		//m_swingDir = 3;
		break;
	default:
		tmp = Game::getTile(getX() - 1, getY(), "from player place tile");
		//m_swingDir = 4;
		break;
	}

	if (tmp) {
		Game::trySetTileObject(tmp, 4);
	}
	audio::AudioManager::play(1);
}