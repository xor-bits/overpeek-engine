#include "player.h"
#include "../../engine.h"
#include "../logic/game.h"
#include "../world/tile.h"

Player::Player(float x, float y, Inventory *inv) : Creature(x, y, 0) {
	m_inv = inv;
}

void Player::submitToRenderer(graphics::Renderer *renderer, float renderOffsetX, float renderOffsetY) {
	Creature::submitToRenderer(renderer, renderOffsetX, renderOffsetY);
}

void Player::update() {
	Creature::update();
	Creature::collide();
}

void Player::hit() {
	Creature::hit();
}

void Player::place() {
	Tile* tmp;
	switch (heading)
	{
	case 0:
		tmp = Game::getTile(x, y - 1);
		m_swingDir = 1;
		break;
	case 1:
		tmp = Game::getTile(x + 1, y);
		m_swingDir = 2;
		break;
	case 2:
		tmp = Game::getTile(x, y + 1);
		m_swingDir = 3;
		break;
	default:
		tmp = Game::getTile(x - 1, y);
		m_swingDir = 4;
		break;
	}

	if (tmp && m_inv->selectedId != 0) {
		tmp->hitObject(2.2, m_inv);
	}
	audio::AudioManager::play(1);
}