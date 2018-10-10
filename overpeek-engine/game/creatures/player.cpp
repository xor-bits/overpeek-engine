#include "player.h"
#include "../../engine.h"
#include "../logic/game.h"
#include "../world/tile.h"

Player::Player(float x, float y, graphics::Shader *shader, Inventory *inv) : Creature(x, y, shader, inv) {
	m_texture = 4;
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