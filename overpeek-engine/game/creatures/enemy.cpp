#include "enemy.h"
#include "../../engine.h"
#include "../logic/game.h"

Enemy::Enemy(float x, float y, graphics::Shader *shader, Inventory *inv) : Creature(x, y, shader, inv) {
	m_texture = 4;
	m_untilnexttarget = 0;
	m_wait = 0;
}

void Enemy::submitToRenderer(graphics::Renderer *renderer, float renderOffsetX, float renderOffsetY) {
	Creature::submitToRenderer(renderer, renderOffsetX, renderOffsetY);
}

void Enemy::update() {
	m_untilnexttarget--; 
	m_wait--;

	if (m_untilnexttarget < 0) {
		m_wait = 100; 
		m_untilnexttarget = 100000000000000;
		float direction = tools::Random::random(0, glm::two_pi<float>());
		m_curtarget_x = cos(direction) / 100;
		m_curtarget_y = sin(direction) / 100;
	}
	if (m_wait < 0) {
		m_untilnexttarget = tools::Random::random(10, 500);
		m_wait = 10000000000000;
		m_curtarget_x = 0;
		m_curtarget_y = 0;
	}

	m_acc_x = m_curtarget_x;
	m_acc_y = m_curtarget_y;
	Creature::update();
	Creature::collide();
}