#include "player.h"
#include "../../engine.h"
#include "../logic/game.h"
#include "../world/tile.h"

Player::Player(float x, float y, graphics::Shader *shader, Inventory *inv) : Creature(x, y, shader, inv) {
	m_swingDir = 0;
	m_texture = 4;
}

void Player::submitToRenderer(graphics::Renderer *renderer, float renderOffsetX, float renderOffsetY) {
	Creature::submitToRenderer(renderer, renderOffsetX, renderOffsetY);
	switch (m_swingDir)
	{
	case 0:
		break;
	case 1:
		renderer->renderBox((m_x + renderOffsetX - 0.5) * TILE_SIZE, (m_y + renderOffsetY - 1.0) * TILE_SIZE, TILE_SIZE, TILE_SIZE, 13);
		break; 
	case 2:
		renderer->renderBox((m_x + renderOffsetX - 0.5) * TILE_SIZE, (m_y + renderOffsetY - 0.0) * TILE_SIZE, TILE_SIZE, TILE_SIZE, 15);
		break;
	case 3:
		renderer->renderBox((m_x + renderOffsetX - 1.0) * TILE_SIZE, (m_y + renderOffsetY - 0.5) * TILE_SIZE, TILE_SIZE, TILE_SIZE, 14);
		break;
	case 4:
		renderer->renderBox((m_x + renderOffsetX - 0.0) * TILE_SIZE, (m_y + renderOffsetY - 0.5) * TILE_SIZE, TILE_SIZE, TILE_SIZE, 12);
		break;
	default:
		break;
	}
}

int counterToRemoveSwingAnimation = 0;
void Player::update() {
	Creature::update();
	Creature::collide();

	if (m_swingDir != 0) {
		counterToRemoveSwingAnimation++;
	}
	if (counterToRemoveSwingAnimation > 10) {
		counterToRemoveSwingAnimation = 0;
		m_swingDir = 0;
	}
}

void Player::hitUp() {
	Tile* tmp = Game::getTile(m_x, m_y - 1);
	if (tmp && m_inv) tmp->hitObject(0.2, m_inv);
	audio::AudioManager::play(1);
	m_swingDir = 1;
}

void Player::hitDown() {
	Tile* tmp = Game::getTile(m_x, m_y + 1);
	if (tmp && m_inv) tmp->hitObject(0.2, m_inv);
	audio::AudioManager::play(1);
	m_swingDir = 2;
}

void Player::hitLeft() {
	Tile* tmp = Game::getTile(m_x - 1, m_y);
	if (tmp && m_inv) tmp->hitObject(0.2, m_inv);
	audio::AudioManager::play(1);
	m_swingDir = 3;
}

void Player::hitRight() {
	Tile* tmp = Game::getTile(m_x + 1, m_y);
	if (tmp && m_inv) tmp->hitObject(0.2, m_inv);
	audio::AudioManager::play(1);
	m_swingDir = 4;
}