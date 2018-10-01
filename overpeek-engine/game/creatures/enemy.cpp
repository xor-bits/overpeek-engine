#include "enemy.h"
#include "../../engine.h"
#include "../logic/game.h"

Enemy::Enemy(float x, float y, graphics::Shader *shader) : Creature(x, y, shader) {
	m_texture = 4;
	m_untilnexttarget = 0;
	m_wait = 0;
}

void Enemy::render(float renderOffsetX, float renderOffsetY) {
	Creature::render(0.0, 0.0);
}

void Enemy::update() {
	m_untilnexttarget--; 
	m_wait--;

	if (m_untilnexttarget < 0) {
		m_wait = 100; 
		m_untilnexttarget = 100000000000000;
		m_curtarget_x = tools::Random::random(-0.01, 0.01);
		m_curtarget_y = tools::Random::random(-0.01, 0.01);
	}
	if (m_wait < 0) {
		m_untilnexttarget = tools::Random::random(10, 500);
		m_wait = 10000000000000;
		m_curtarget_x = 0;
		m_curtarget_y = 0;
	}

	//m_vel_x = m_curtarget_x;
	//m_vel_y = m_curtarget_y;
	Creature::update();
	Creature::collide();
}