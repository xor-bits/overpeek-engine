#include "creature.h"
#include "../world/tile.h"
#include "../../logic/aabb.h"

Creature::Creature(float x, float y, graphics::Shader *shader, Inventory *inv) {
	m_inv = inv;
	x = x; y = y;
	vel_x = 0;
	vel_y = 0;
	acc_x = 0;
	acc_y = 0;
	m_swingDir = 0;
}

void Creature::submitToRenderer(graphics::Renderer *renderer, float renderOffsetX, float renderOffsetY) {
	renderer->renderBox((x + renderOffsetX - 0.5) * TILE_SIZE, (y + renderOffsetY - 0.5) * TILE_SIZE, TILE_SIZE, TILE_SIZE, m_texture);

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

void Creature::update() {
	vel_x += acc_x;
	vel_y += acc_y;
	x += vel_x * 4;
	y += vel_y * 4;

	if (vel_y < 0) heading = 0;
	else if (vel_y > 0) heading = 2;
	else if (vel_x > 0) heading = 1;
	else if (vel_x < 0) heading = 3;

	vel_x = 0.0;
	vel_y = 0.0;
	acc_x = 0;
	acc_y = 0;


	if (m_swingDir != 0) {
		m_counterToRemoveSwingAnimation++;
	}
	if (m_counterToRemoveSwingAnimation > 10) {
		m_counterToRemoveSwingAnimation = 0;
		m_swingDir = 0;
	}
}

void Creature::hit() {
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
		tmp->hitObject(2.2, m_inv);
		//if (m_inv->selectedId == 0) tmp->hitObject(2.2, m_inv);
		//if (m_inv->selectedId != 0 && Game::trySetTileObject(tmp, m_inv->selectedId)) m_inv->removeSelected();
	}
	audio::AudioManager::play(1);
}

void Creature::place() {
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

void Creature::collide() {
	for (int _x = -2; _x < 3; _x++)
	{
		for (int _y = -2; _y < 3; _y++)
		{
			Tile* tile = Game::getTile(floor(x) + _x, floor(y) + _y);
			if (tile == nullptr) continue;

			if (tile->getObjectId() == 9) {
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