#include "player.h"
#include "../../engine.h"
#include "../logic/game.h"

Player::Player(float x, float y, graphics::Shader *shader) : Creature(x, y, shader) {
	m_texture = 4;
}

void Player::render(float renderOffsetX, float renderOffsetY) {
	Creature::render(-m_x, -m_y);
}

void Player::update() {
	Creature::update();
	Creature::collide();
}