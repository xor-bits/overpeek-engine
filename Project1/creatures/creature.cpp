#include "../pch.h"

#include "creature.h"

#include "../world/map.h"
#include "../world/tile.h"
#include "../world/region.h"
#include "../logic/database.h"
#include "../logic/game.h"
#include "../logic/inventory.h"
#include "player.h"

Creature::Creature(float _x, float _y, int _id, bool _item) {
	m_id = _id;
	x = _x + 0.5; y = _y + 0.5;
	m_item = _item;
	vel_x = 0; vel_y = 0;
	acc_x = 0; acc_y = 0;
	m_swingDir = 0;
	m_health = Database::creatures[m_id].health;

	m_untilnexttarget = 500;
	m_wait = 0;
}

Creature::Creature() {
	x = 0; y = 0;
	vel_x = 0; vel_y = 0;
	acc_x = 0; acc_y = 0;
	m_swingDir = 0;
	m_health = 0;

	m_untilnexttarget = 0;
	m_wait = 0;
}

void Creature::submitToRenderer(graphics::Renderer *renderer, float renderOffsetX, float renderOffsetY) {
	if (!m_item) {
		switch (heading)
		{
		case HEADING_UP:
			renderer->renderBox((x + renderOffsetX - 0.5) * TILE_SIZE, (y + renderOffsetY - 0.5) * TILE_SIZE, TILE_SIZE, TILE_SIZE, 0, Database::creatures[m_id].texture_heading_up, glm::vec4(1.0, 1.0, 1.0, 1.0));
			break;
		case HEADING_DOWN:
			renderer->renderBox((x + renderOffsetX - 0.5) * TILE_SIZE, (y + renderOffsetY - 0.5) * TILE_SIZE, TILE_SIZE, TILE_SIZE, 0, Database::creatures[m_id].texture_heading_down, glm::vec4(1.0, 1.0, 1.0, 1.0));
			break;
		case HEADING_LEFT:
			renderer->renderBox((x + renderOffsetX - 0.5) * TILE_SIZE, (y + renderOffsetY - 0.5) * TILE_SIZE, TILE_SIZE, TILE_SIZE, 0, Database::creatures[m_id].texture_heading_left, glm::vec4(1.0, 1.0, 1.0, 1.0));
			break;
		case HEADING_RIGHT:
			renderer->renderBox((x + renderOffsetX - 0.5) * TILE_SIZE, (y + renderOffsetY - 0.5) * TILE_SIZE, TILE_SIZE, TILE_SIZE, 0, Database::creatures[m_id].texture_heading_right, glm::vec4(1.0, 1.0, 1.0, 1.0));
			break;
		default:
			break;
		}

		switch (m_swingDir)
		{
		case 0:
			break;
		case 1:
			renderer->renderBox((x + renderOffsetX - 0.5) * TILE_SIZE, (y + renderOffsetY - 1.0) * TILE_SIZE, TILE_SIZE, TILE_SIZE, 0, 13, glm::vec4(1.0, 1.0, 1.0, 1.0));
			break;
		case 2:
			renderer->renderBox((x + renderOffsetX - 0.0) * TILE_SIZE, (y + renderOffsetY - 0.5) * TILE_SIZE, TILE_SIZE, TILE_SIZE, 0, 12, glm::vec4(1.0, 1.0, 1.0, 1.0));
			break;
		case 3:
			renderer->renderBox((x + renderOffsetX - 0.5) * TILE_SIZE, (y + renderOffsetY - 0.0) * TILE_SIZE, TILE_SIZE, TILE_SIZE, 0, 15, glm::vec4(1.0, 1.0, 1.0, 1.0));
			break;
		case 4:
			renderer->renderBox((x + renderOffsetX - 1.0) * TILE_SIZE, (y + renderOffsetY - 0.5) * TILE_SIZE, TILE_SIZE, TILE_SIZE, 0, 14, glm::vec4(1.0, 1.0, 1.0, 1.0));
			break;
		default:
			break;
		}
	}
	else {
		renderer->renderBox((x + renderOffsetX - 0.5) * TILE_SIZE, (y + renderOffsetY - 0.5) * TILE_SIZE, TILE_SIZE, TILE_SIZE, 0, Database::items[m_id].texture, glm::vec4(1.0, 1.0, 1.0, 1.0));
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
	else {
		float distanceToPlayer = abs(x - Game::getPlayer()->x) + abs(y - Game::getPlayer()->y);
		if (distanceToPlayer < 0.8) {
			if (Game::getPlayer()->inventory->addItem(m_id)) {
				///REMOVE THIS CREATURE
				#if !STORE_MAP_IN_RAM
				Game::getRegion(getX(), getY())->removeCreature(m_regionIndex, true);
				#else
				Game::getMap()->removeCreature(m_regionIndex, true);
				#endif				
				audio::AudioManager::play(2);
				return;
			}
		}
	}


	vel_x += acc_x;
	vel_y += acc_y;
	x += vel_x;
	y += vel_y;
	
	vel_x *= 0.90;
	vel_y *= 0.90;
	acc_x = 0;
	acc_y = 0;
}

void Creature::hit() {
	if (m_item) return;
	float hitx = getX(), hity = getY();
	switch (heading)
	{
	case 0:
		hity -= 1;
		m_swingDir = 1;
		break;
	case 1:
		hitx += 1;
		m_swingDir = 2;
		break;
	case 2:
		hity += 1;
		m_swingDir = 3;
		break;
	default:
		hitx -= 1;
		m_swingDir = 4;
		break;
	}

	//Creature hitting
#if !STORE_MAP_IN_RAM
	Creature* creatureArray[MAX_CREATURES];
	unsigned int amount;
	Region* thisRegion = Game::getRegion(hitx, hity);
	Game::findAllCreatures(hitx, hity, creatureArray, amount, 1.0);
#else
	Creature* creatureArray[M_MAP_MAX_CREATURES];
	unsigned int amount;
	Game::getMap()->findAllCreatures(hitx, hity, creatureArray, amount, 1.0);
#endif
	
	for (int i = 0; i < amount; i++)
	{
		glm::vec2 directionVector = glm::vec2(creatureArray[i]->getX() - getX(), creatureArray[i]->getY() - getY());
		directionVector = glm::normalize(directionVector);
	
		creatureArray[i]->acc_x = directionVector.x / 10.0 * Database::creatures[m_id].knockback;
		creatureArray[i]->acc_y = directionVector.y / 10.0 * Database::creatures[m_id].knockback;
		creatureArray[i]->heal(-Database::creatures[m_id].meleeDamage);
	}

	//Tile hitting
#if !STORE_MAP_IN_RAM
	Tile* tmp = Game::getTile(hitx, hity, "creature");
	if (tmp) {
		if (Database::objects[tmp->getObjectId()].destructable) {
			tmp->hitObject(0.5f);
		}
	}
#else
	Map::MapTile* tmp = Game::getMap()->getTile(hitx, hity);
	if (tmp) {
		if (Database::objects[tmp->m_object].destructable) {
			Game::getMap()->hit(hitx, hity, 0.5f);
		}
	}
#endif

	//Swing noise
	audio::AudioManager::play(1);
}

void Creature::collide() {
	if (!m_item && Database::creatures[m_id].ghost) return;

	for (int _x = -1; _x < 2; _x++)
	{
		for (int _y = -1; _y < 2; _y++)
		{
			#if !STORE_MAP_IN_RAM
			Tile* tile = Game::getTile(floor(x) + _x, floor(y) + _y, std::string("from creature to collide ") + std::to_string(m_id));
			if (tile == nullptr) continue;
			if (Database::objects[tile->getObjectId()].wall) {
			#else
			Map::MapTile* tile = Game::getMap()->getTile(floor(x) + _x, floor(y) + _y);
			if (tile == nullptr) continue;
			if (Database::objects[tile->m_object].wall) {
			#endif

				//Start loop
				int tilex = floor(x) + _x;
				int tiley = floor(y) + _y;

				////LEFT COLLIDER
				//if (logic::AABB(
				//	glm::vec2(x - CREAURE_WIDTH / 2.0, (y - CREAURE_HEIGHT / 2.0) + 0.3),
				//	glm::vec2(CREAURE_WIDTH / 2.0, CREAURE_HEIGHT - 0.6),
				//	glm::vec2(tilex, tiley),
				//	glm::vec2(1, 1)
				//)) {
				//	x = tilex + 1 + (CREAURE_WIDTH / 2.0);
				//
				//	//Process new area if player
				//	#if !STORE_MAP_IN_RAM
				//	if (m_id == 0) Game::processNewArea();
				//	#endif
				//}
				////RIGHT COLLIDER
				//if (logic::AABB(
				//	glm::vec2(x, (y - CREAURE_HEIGHT / 2.0) + 0.3),
				//	glm::vec2(CREAURE_WIDTH / 2.0, CREAURE_HEIGHT - 0.6),
				//	glm::vec2(tilex, tiley),
				//	glm::vec2(1, 1)
				//)) {
				//	x = tilex - (CREAURE_WIDTH / 2.0);
				//
				//	//Process new area if player
				//	#if !STORE_MAP_IN_RAM
				//	if (m_id == 0) Game::processNewArea();
				//	#endif
				//}
				////TOP COLLIDER
				//if (logic::AABB(
				//	glm::vec2((x - CREAURE_WIDTH / 2.0) + 0.3, y - CREAURE_HEIGHT / 2.0),
				//	glm::vec2(CREAURE_WIDTH - 0.6, CREAURE_HEIGHT / 2.0),
				//	glm::vec2(tilex, tiley),
				//	glm::vec2(1, 1)
				//)) {
				//	y = tiley + 1 + (CREAURE_HEIGHT / 2.0);
				//
				//	//Process new area if player
				//	#if !STORE_MAP_IN_RAM
				//	if (m_id == 0) Game::processNewArea();
				//	#endif
				//}
				////BOTTOM COLLIDER
				//if (logic::AABB(
				//	glm::vec2((x - CREAURE_WIDTH / 2.0) + 0.3, y),
				//	glm::vec2(CREAURE_WIDTH - 0.6, CREAURE_HEIGHT / 2.0),
				//	glm::vec2(tilex, tiley),
				//	glm::vec2(1, 1)
				//)) {
				//	y = tiley - (CREAURE_HEIGHT / 2.0);
				//
				//	//Process new area if player
				//	#if !STORE_MAP_IN_RAM
				//	if (m_id == 0) Game::processNewArea();
				//	#endif
				//}
			}
		}
	}
}

void Creature::enemyAi() {
	m_untilnexttarget--;
	m_wait--;

	if (m_untilnexttarget < 0) {
		m_wait = 100;
		m_untilnexttarget = INFINITE;
		float direction = tools::Random::random(0, glm::two_pi<float>());
		m_curtarget_x = cos(direction) / 100;
		m_curtarget_y = sin(direction) / 100;
	}
	if (m_wait < 0) {
		m_untilnexttarget = tools::Random::random(10, 500);
		m_wait = INFINITE;
		m_curtarget_x = 0;
		m_curtarget_y = 0;
	}

	x += m_curtarget_x;
	y += m_curtarget_y;
}

void Creature::heal() {
	if (m_item) return;

	m_health = Database::creatures[m_id].health;
}

void Creature::heal(float amount) {
	if (m_item) return;

	m_health += amount;
	if (m_health <= 0) {
		Game::addCreature(getX(), getY(), Database::creatures[m_id].dropsAs, true);
		///REDO THIS
#if !STORE_MAP_IN_RAM
		Game::getRegion(getX(), getY())->removeCreature(m_regionIndex, true);
#endif
	}
}