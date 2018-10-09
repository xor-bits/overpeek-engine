#include "creature.h"
#include "../world/tile.h"
#include "../../logic/aabb.h"

Creature::Creature(float x, float y, graphics::Shader *shader, Inventory *inv) {
	m_inv = inv;
	m_x = x; m_y = y;
	m_vel_x = 0;
	m_vel_y = 0;
	m_acc_x = 0;
	m_acc_y = 0;
}

void Creature::submitToRenderer(graphics::Renderer *renderer, float renderOffsetX, float renderOffsetY) {
	renderer->renderBox((m_x + renderOffsetX - 0.5) * TILE_SIZE, (m_y + renderOffsetY - 0.5) * TILE_SIZE, TILE_SIZE, TILE_SIZE, m_texture);
}

void Creature::update() {
	m_x += m_vel_x * 1;
	m_y += m_vel_y * 1;
	m_vel_x += m_acc_x;
	m_vel_y += m_acc_y;
	m_vel_x *= 0.8;
	m_vel_y *= 0.8;
	m_acc_x = 0;
	m_acc_y = 0;
}

void Creature::collide() {
	for (int x = -2; x < 3; x++)
	{
		for (int y = -2; y < 3; y++)
		{
			Tile* tile = Game::getTile(floor(m_x) + x, floor(m_y) + y);
			if (tile == nullptr) continue;

			if (tile->getObjectId() == 9) {
				int tilex = floor(m_x) + x;
				int tiley = floor(m_y) + y;
				//LEFT COLLIDER
				if (logic::AABB(
					glm::vec2(m_x - CREAURE_WIDTH / 2.0, (m_y - CREAURE_HEIGHT / 2.0) + 0.3),
					glm::vec2(CREAURE_WIDTH / 2.0, CREAURE_HEIGHT - 0.6),
					glm::vec2(tilex, tiley),
					glm::vec2(1, 1)
				)) {
					m_x = tilex + 1 + (CREAURE_WIDTH / 2.0);
				}
				//RIGHT COLLIDER
				if (logic::AABB(
					glm::vec2(m_x, (m_y - CREAURE_HEIGHT / 2.0) + 0.3),
					glm::vec2(CREAURE_WIDTH / 2.0, CREAURE_HEIGHT - 0.6),
					glm::vec2(tilex, tiley),
					glm::vec2(1, 1)
				)) {
					m_x = tilex - (CREAURE_WIDTH / 2.0);
				}
				//TOP COLLIDER
				if (logic::AABB(
					glm::vec2((m_x - CREAURE_WIDTH / 2.0) + 0.3, m_y - CREAURE_HEIGHT / 2.0),
					glm::vec2(CREAURE_WIDTH - 0.6, CREAURE_HEIGHT / 2.0),
					glm::vec2(tilex, tiley),
					glm::vec2(1, 1)
				)) {
					m_y = tiley + 1 + (CREAURE_HEIGHT / 2.0);
				}
				//BOTTOM COLLIDER
				if (logic::AABB(
					glm::vec2((m_x - CREAURE_WIDTH / 2.0) + 0.3, m_y),
					glm::vec2(CREAURE_WIDTH - 0.6, CREAURE_HEIGHT / 2.0),
					glm::vec2(tilex, tiley),
					glm::vec2(1, 1)
				)) {
					m_y = tiley - (CREAURE_HEIGHT / 2.0);
				}
			}
		}
	}
}