#include "player.h"
#include "../../engine.h"
#include "../logic/game.h"
#include "../logic/textureManager.h"

Player::Player(float x, float y) {
	m_x = x; m_y = y;
}

void Player::render() {
	graphics::SimpleRenderer::renderBox(-PLAYER_WIDTH / 2, -PLAYER_HEIGHT / 2, PLAYER_WIDTH, PLAYER_HEIGHT, TextureManager::getTexture(0));
}

void Player::update() {
	m_x += m_vel_x;
	m_y += m_vel_y;
	m_vel_x += m_acc_x;
	m_vel_y += m_acc_y;
	m_vel_x *= 0.95;
	m_vel_y *= 0.95;
	m_acc_x = 0;
	m_acc_y = 0;
}