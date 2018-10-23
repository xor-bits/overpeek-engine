#include "creature.h"
#include "../world/tile.h"
#include "../../logic/aabb.h"
#include "../logic/database.h"

Creature::Creature(float _x, float _y, int _id, bool _item) {
	m_id = _id;
	x = _x; y = _y;
	m_item = _item;
	vel_x = 0; vel_y = 0;
	acc_x = 0; acc_y = 0;
	m_swingDir = 0;

	m_untilnexttarget = 500;
	m_wait = 0;
}

Creature::Creature() {
	x = 0; y = 0;
	vel_x = 0; vel_y = 0;
	acc_x = 0; acc_y = 0;
	m_swingDir = 0;

	m_untilnexttarget = 0;
	m_wait = 0;
}

void Creature::submitToRenderer(graphics::Renderer *renderer, float renderOffsetX, float renderOffsetY) {
	if (!m_item) {
		renderer->renderBox((x + renderOffsetX - 0.5) * TILE_SIZE, (y + renderOffsetY - 0.5) * TILE_SIZE, TILE_SIZE, TILE_SIZE, Database::creatures[m_id].texture);

		switch (m_swingDir)
		{
		case 0:
			break;
		case 1:
			renderer->renderBox((x + renderOffsetX - 0.5) * TILE_SIZE, (y + renderOffsetY - 1.0) * TILE_SIZE, TILE_SIZE, TILE_SIZE, 13);
			break;
		case 2:
			renderer->renderBox((x + renderOffsetX - 0.0) * TILE_SIZE, (y + renderOffsetY - 0.5) * TILE_SIZE, TILE_SIZE, TILE_SIZE, 12);
			break;
		case 3:
			renderer->renderBox((x + renderOffsetX - 0.5) * TILE_SIZE, (y + renderOffsetY - 0.0) * TILE_SIZE, TILE_SIZE, TILE_SIZE, 15);
			break;
		case 4:
			renderer->renderBox((x + renderOffsetX - 1.0) * TILE_SIZE, (y + renderOffsetY - 0.5) * TILE_SIZE, TILE_SIZE, TILE_SIZE, 14);
			break;
		default:
			break;
		}
	}
	else {
		renderer->renderBox((x + renderOffsetX - 0.5) * TILE_SIZE, (y + renderOffsetY - 0.5) * TILE_SIZE, TILE_SIZE, TILE_SIZE, Database::items[m_id].texture);
	}

}

void Creature::update() {
	if (!m_item) {
		if (m_id == 1) enemyAi();

		if (m_swingDir != 0) {
			m_counterToRemoveSwingAnimation++;
		}
		if (m_counterToRemoveSwingAnimation > 10) {
			m_counterToRemoveSwingAnimation = 0;
			m_swingDir = 0;
		}
	}

	vel_x += acc_x;
	vel_y += acc_y;
	x += vel_x * 4;
	y += vel_y * 4;

	if (!m_item) {
		if (vel_y < 0) heading = 0;
		else if (vel_y > 0) heading = 2;
		else if (vel_x > 0) heading = 1;
		else if (vel_x < 0) heading = 3;
	}

	vel_x = 0.0;
	vel_y = 0.0;
	acc_x = 0;
	acc_y = 0;
}

void Creature::hit() {
	if (m_item) return;
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
	if (tmp) {
		tmp->addObjectHealth(-0.5f);
	}

	audio::AudioManager::play(1);
}

void Creature::collide() {
	if (m_item) return;
	for (int _x = -2; _x < 3; _x++)
	{
		for (int _y = -2; _y < 3; _y++)
		{
			Tile* tile = Game::getTile(floor(x) + _x, floor(y) + _y);
			if (tile == nullptr) continue;

			if (Database::objects[tile->getObjectId()].wall) {
				int tilex = floor(x) + _x;
				int tiley = floor(y) + _y;
				//LEFT COLLIDER
				if (logic::AABB(
					glm::vec2(x - CREAURE_WIDTH / 2.0, (y - CREAURE_HEIGHT / 2.0) + 0.3),
					glm::vec2(CREAURE_WIDTH / 2.0, CREAURE_HEIGHT - 0.6),
					glm::vec2(tilex, tiley),
					glm::vec2(1, 1)
				)) {
					x = tilex + 1 + (CREAURE_WIDTH / 2.0);
				}
				//RIGHT COLLIDER
				if (logic::AABB(
					glm::vec2(x, (y - CREAURE_HEIGHT / 2.0) + 0.3),
					glm::vec2(CREAURE_WIDTH / 2.0, CREAURE_HEIGHT - 0.6),
					glm::vec2(tilex, tiley),
					glm::vec2(1, 1)
				)) {
					x = tilex - (CREAURE_WIDTH / 2.0);
				}
				//TOP COLLIDER
				if (logic::AABB(
					glm::vec2((x - CREAURE_WIDTH / 2.0) + 0.3, y - CREAURE_HEIGHT / 2.0),
					glm::vec2(CREAURE_WIDTH - 0.6, CREAURE_HEIGHT / 2.0),
					glm::vec2(tilex, tiley),
					glm::vec2(1, 1)
				)) {
					y = tiley + 1 + (CREAURE_HEIGHT / 2.0);
				}
				//BOTTOM COLLIDER
				if (logic::AABB(
					glm::vec2((x - CREAURE_WIDTH / 2.0) + 0.3, y),
					glm::vec2(CREAURE_WIDTH - 0.6, CREAURE_HEIGHT / 2.0),
					glm::vec2(tilex, tiley),
					glm::vec2(1, 1)
				)) {
					y = tiley - (CREAURE_HEIGHT / 2.0);
				}
			}
		}
	}
}

void Creature::enemyAi() {
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

	vel_x = m_curtarget_x;
	vel_y = m_curtarget_y;
}