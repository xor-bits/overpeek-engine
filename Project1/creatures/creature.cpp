#include "../pch.h"

#include "creature.h"

#include "../world/map.h"
#include "../world/tile.h"
#include "../world/region.h"
#include "../logic/database.h"
#include "../logic/game.h"
#include "../logic/inventory.h"
#include "../logic/aStar.h"
#include "../settings.h"
#include "player.h"

Creature::Creature(float _x, float _y, int _id, bool _item) {
	m_id = _id;
	x = _x + 0.5; y = _y + 0.5;
	m_item = _item;
	vel_x = 0; vel_y = 0;
	acc_x = 0; acc_y = 0;
	m_swingDir = 0;

	m_untilnexttarget = 500;
	m_wait = 0;
	m_chasing = false;

	//HS
	m_maxHealth = Database::creatures[m_id].health;
	m_maxStamina = Database::creatures[m_id].stamina;
	m_healthGainRate = Database::creatures[m_id].healthgain;
	m_staminaGainRate = Database::creatures[m_id].staminagain;

	m_staminaRegenCooldown = 0;
	m_healthRegenCooldown = 0;

	resetHealth();
	resetStamina();
}

Creature::Creature() {
	x = 0; y = 0;
	vel_x = 0; vel_y = 0;
	acc_x = 0; acc_y = 0;
	m_swingDir = 0;

	m_untilnexttarget = 0;
	m_wait = 0;
	m_chasing = false;
}

void Creature::submitToRenderer(oe::Renderer *renderer, float renderOffsetX, float renderOffsetY) {
	if (!m_item) {
		int heading_texture;
		switch (heading)
		{
		case HEADING_UP:
			heading_texture = Database::creatures[m_id].texture_heading_up;
			break;
		case HEADING_DOWN:
			heading_texture = Database::creatures[m_id].texture_heading_down;
			break;
		case HEADING_LEFT:
			heading_texture = Database::creatures[m_id].texture_heading_left;
			break;
		case HEADING_RIGHT:
			heading_texture = Database::creatures[m_id].texture_heading_right;
			break;
		default:
			break;
		}
		glm::vec2 pos = glm::vec2((getX() + renderOffsetX - 0.5f) * TILE_SIZE, (getY() + renderOffsetY - 0.5f) * TILE_SIZE);
		glm::vec2 size = glm::vec2(TILE_SIZE, TILE_SIZE);
		//renderer->renderBox(pos, size, heading_texture, glm::vec4(1.0, 1.0, 1.0, 1.0));
		renderer->renderPoint(pos, size, heading_texture, glm::vec4(1.0));

		float swingX, swingY;
		int swingTexture;
		switch (m_swingDir)
		{
		case 1:
			swingTexture = 13;
			swingX = (getX() + renderOffsetX - 0.5) * TILE_SIZE; swingY = (getY() + renderOffsetY - 1.0) * TILE_SIZE;
			break;
		case 2:
			swingTexture = 12;
			swingX = (getX() + renderOffsetX - 0.0) * TILE_SIZE; swingY = (getY() + renderOffsetY - 0.5) * TILE_SIZE;
			break;
		case 3:
			swingTexture = 15;
			swingX = (getX() + renderOffsetX - 0.5) * TILE_SIZE; swingY = (getY() + renderOffsetY - 0.0) * TILE_SIZE;
			break;
		case 4:
			swingTexture = 14;
			swingX = (getX() + renderOffsetX - 1.0) * TILE_SIZE; swingY = (getY() + renderOffsetY - 0.5) * TILE_SIZE;
			break;
		default:
			break;
		}
		if (m_swingDir != 0) {
			glm::vec2 pos = glm::vec2(swingX, swingY);
			glm::vec2 size = glm::vec2(TILE_SIZE, TILE_SIZE);
			//renderer->renderBox(pos, size, swingTexture, glm::vec4(1.0, 1.0, 1.0, 1.0));
			renderer->renderPoint(pos, size, swingTexture, glm::vec4(1.0));
		}
	}
	else {
		glm::vec2 pos = glm::vec2((getX() + renderOffsetX - 0.5f) * TILE_SIZE, (getY() + renderOffsetY - 0.5f) * TILE_SIZE);
		glm::vec2 size = glm::vec2(TILE_SIZE, TILE_SIZE);
		//renderer->renderBox(pos, size, Database::items[m_id].texture, glm::vec4(1.0, 1.0, 1.0, 1.0));
		renderer->renderPoint(pos, size, Database::items[m_id].texture, glm::vec4(1.0));
	}

	if (m_path && m_result != 0) m_path->debugRender(renderer, renderOffsetX, renderOffsetY);

}

void Creature::clampHPAndSTA() {
	if (m_health > m_maxHealth) m_health = m_maxHealth;
	if (m_health < 0) m_health = 0;
	if (m_stamina > m_maxStamina) m_stamina = m_maxStamina;
	if (m_stamina < 0) m_stamina = 0;
}

void Creature::update(int index) {


	if (!m_item) {
		if (m_staminaRegenCooldown > 200) m_stamina += m_staminaGainRate;
		else m_staminaRegenCooldown++;

		if (m_healthRegenCooldown > 200) m_health += m_healthGainRate;
		else m_healthRegenCooldown++;
		clampHPAndSTA();
		if (m_health <= 0) {
			//Drops
			Game::getMap()->addCreature(getX(), getY(), Database::creatures[m_id].dropsAs, true);

			//Remobe this
			Game::getMap()->removeCreature(this);
			return;
		}

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
		float distanceToPlayer = abs(getX() - Game::getPlayer()->getX()) + abs(getY() - Game::getPlayer()->getY());
		if (distanceToPlayer < 0.8) {
			if (Game::getPlayer()->inventory->addItem(m_id, 1)) {
				///REMOVE THIS CREATURE
				#if !STORE_MAP_IN_RAM
				Game::getRegion(getX(), getY())->removeCreature(m_regionIndex, true);
				#else
				Game::getMap()->removeCreature(index);
				#endif				
				oe::AudioManager::play(2);
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
	collide();
}

void Creature::hit(float damagemult) {
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
	std::vector<Creature*> creatureArray;
	Game::getMap()->findAllCreatures(hitx, hity, creatureArray, 1.0);
#endif

	for (int i = 0; i < creatureArray.size(); i++)
	{
		glm::vec2 directionVector = glm::vec2(creatureArray[i]->getX() - getX(), creatureArray[i]->getY() - getY());
		directionVector = glm::normalize(directionVector);
	
		creatureArray[i]->acc_x = directionVector.x / 10.0 * Database::creatures[m_id].knockback;
		creatureArray[i]->acc_y = directionVector.y / 10.0 * Database::creatures[m_id].knockback;
		creatureArray[i]->setHealth(creatureArray[i]->getHealth() - Database::creatures[m_id].meleeDamage * damagemult);
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
	Map::MapTile* tmp = Game::getMap()->getTile((unsigned int)hitx, (unsigned int)hity);
	if (tmp) {
		if (Database::objects[tmp->m_object].destructable) {
			Game::getMap()->hit((unsigned int)hitx, (unsigned int)hity, Database::creatures[m_id].meleeDamage * damagemult);
		}
	}
#endif

	//Swing noise
	oe::AudioManager::play(1);
}

bool AABB(glm::fvec2 aPos, glm::fvec2 aSize, glm::fvec2 bPos, glm::fvec2 bSize) {
	return	bPos.x < aPos.x + aSize.x && aPos.x < bPos.x + bSize.x
		&&  bPos.y < aPos.y + aSize.y && aPos.y < bPos.y + bSize.y;
}

void Creature::collide() {
	if (!m_item && Database::creatures[m_id].ghost) return;

	for (int _x = -1; _x < 2; _x++)
	{
		for (int _y = -1; _y < 2; _y++)
		{
			int tilex = (int)round(getX()) + _x;
			int tiley = (int)round(getY()) + _y;
			#if !STORE_MAP_IN_RAM
			Tile* tile = Game::getTile(floor(x) + _x, floor(y) + _y, std::string("from creature to collide ") + std::to_string(m_id));
			if (tile == nullptr) continue;
			if (Database::objects[tile->getObjectId()].wall) {
			#else
			Map::MapTile* tile = Game::getMap()->getTile(tilex, tiley);
			if (tile == nullptr) continue;
			if (Database::objects[tile->m_object].wall) {
			#endif
				bool top = false, bottom = false;
				bool left = false, right = false;
				float x_to_move;
				float y_to_move;

				//LEFT COLLIDER
				if (AABB(
					glm::vec2(getX() - PLAYER_WIDTH / 2.0f, getY() - PLAYER_HEIGHT / 2.0f + 0.3f),
					glm::vec2(PLAYER_WIDTH / 2.0f, PLAYER_HEIGHT - 0.6f),
					glm::vec2(tilex, tiley),
					glm::vec2(1.0f, 1.0f)
				)) {
					left = true;
					x_to_move = ((float)tilex + 1.0f + (PLAYER_WIDTH / 2.0f)) - getX();
				
					//Process new area if player
					#if !STORE_MAP_IN_RAM
					if (m_id == 0) Game::processNewArea();
					#endif
				}
				
				//RIGHT COLLIDER
				if (AABB(
					glm::vec2(getX(), getY() - PLAYER_HEIGHT / 2.0f + 0.3f),
					glm::vec2(PLAYER_WIDTH / 2.0f, PLAYER_HEIGHT - 0.6f),
					glm::vec2(tilex, tiley),
					glm::vec2(1.0f, 1.0f)
				)) {
					right = true;
					x_to_move = ((float)tilex - (PLAYER_WIDTH / 2.0f)) - getX();
				
					//Process new area if player
					#if !STORE_MAP_IN_RAM
					if (m_id == 0) Game::processNewArea();
					#endif
				}
				//TOP COLLIDER
				if (AABB(
					glm::vec2(getX() - PLAYER_WIDTH / 2.0f + 0.3f, getY() - PLAYER_HEIGHT / 2.0f),
					glm::vec2(PLAYER_WIDTH - 0.6f, PLAYER_HEIGHT / 2.0f),
					glm::vec2(tilex, tiley),
					glm::vec2(1.0f, 1.0f)
				)) {
					top = true;
					y_to_move = ((float)tiley + 1.0f + (PLAYER_HEIGHT / 2.0f)) - getY();
				
					//Process new area if player
					#if !STORE_MAP_IN_RAM
					if (m_id == 0) Game::processNewArea();
					#endif
				}
				//BOTTOM COLLIDER
				if (AABB(
					glm::vec2(getX() - PLAYER_WIDTH / 2.0f + 0.3f, getY()),
					glm::vec2(PLAYER_WIDTH - 0.6f, PLAYER_HEIGHT / 2.0f),
					glm::vec2(tilex, tiley),
					glm::vec2(1.0f, 1.0f)
				)) {
					bottom = true;
					y_to_move = ((float)tiley - PLAYER_HEIGHT / 2.0f) - getY();
				
					//Process new area if player
					#if !STORE_MAP_IN_RAM
					if (m_id == 0) Game::processNewArea();
					#endif
				}

				if (top != bottom) { addY(y_to_move); vel_y = 0; }
				if (left != right) { addX(x_to_move); vel_x = 0;
				}
			}
		}
	}
}

bool lineLine(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d)
{
	float denominator = ((b.x - a.x) * (d.y - c.y)) - ((b.y - a.y) * (d.x - c.x));
	float numerator1 = ((a.y - c.y) * (d.x - c.x)) - ((a.x - c.x) * (d.y - c.y));
	float numerator2 = ((a.y - c.y) * (b.x - a.x)) - ((a.x - c.x) * (b.y - a.y));

	// Detect coincident lines (has a problem, read below)
	if (denominator == 0) return numerator1 == 0 && numerator2 == 0;

	float r = numerator1 / denominator;
	float s = numerator2 / denominator;

	return (r >= 0 && r <= 1) && (s >= 0 && s <= 1);
}

bool Creature::canSee(float _x, float _y) {
	if (Game::advancedDebugMode) return false;
	//LineABXY from this enemy to x,y

	//for all tiles
	//	if this tile has inpassable object
	//	for all object sides
	//	if LineABXY and this object side intersects
	//		return false
	for (int x = -MAP_WORK_DST; x < MAP_WORK_DST; x++)
	{
		for (int y = -MAP_WORK_DST; y < MAP_WORK_DST; y++)
		{
			Map::MapTile *tile = Game::getMap()->getTile(x + floor(getX()), y + floor(getY()));

			if (Database::objects[tile->m_object].wall) {
				if (lineLine(
					glm::vec2(getX(), getY()), 
					glm::vec2(_x, _y),
					glm::vec2(x + floor(getX()), y + floor(getY())), 
					glm::vec2(x + floor(getX()), y + floor(getY()) + 1.0)
				)) {
					//oe::Logger::info("0");
					return false;
				}
				if (lineLine(
					glm::vec2(getX(), getY()),
					glm::vec2(_x, _y),
					glm::vec2(x + floor(getX()), y + floor(getY())),
					glm::vec2(x + floor(getX()) + 1.0, y + floor(getY()))
				)) {
					//oe::Logger::info("1");
					return false;
				}
				if (lineLine(
					glm::vec2(getX(), getY()),
					glm::vec2(_x, _y),
					glm::vec2(x + floor(getX()) + 1.0, y + floor(getY()) + 1.0),
					glm::vec2(x + floor(getX()), y + floor(getY()) + 1.0)
				)) {
					//oe::Logger::info("2");
					return false;
				}
				if (lineLine(
					glm::vec2(getX(), getY()),
					glm::vec2(_x, _y),
					glm::vec2(x + floor(getX()) + 1.0, y + floor(getY()) + 1.0),
					glm::vec2(x + floor(getX()) + 1.0, y + floor(getY()))
				)) {
					//oe::Logger::info("3");
					return false;
				}
			}
		}
	}

	return true;
}

int sign(float n) {
	if (n > 0) return 1;
	return -1;
}

void Creature::enemyAi() {
	m_untilnexttarget--;
	m_wait--;

	//oe::Logger::info(m_untilnexttarget, m_wait);
	if (m_chasing) {
		glm::vec2 dirToPlayer = glm::normalize(glm::vec2(Game::getPlayer()->getX() - getX(), Game::getPlayer()->getY() - getY()));
		vel_x = dirToPlayer.x / 50.0;
		vel_y = dirToPlayer.y / 50.0;
	}
	if (m_untilnexttarget < 0) {
		m_wait = 500;
		m_untilnexttarget = 2000;


		if (canSee(Game::getPlayer()->getX(), Game::getPlayer()->getY())) {
			m_chasing = true;
			return;
		}
		else m_chasing = false;


		m_curtarget_x = oe::Random::random(-16, 16);
		m_curtarget_y = oe::Random::random(-16, 16);

		m_path = new Pathfinder(getX(), getY(), getX() + m_curtarget_x, getY() + m_curtarget_y, 10);
		m_result = 0;
	}
	if (m_wait < 0) {
		m_untilnexttarget = oe::Random::random(50, 200);
		m_wait = 4000;


		if (canSee(Game::getPlayer()->getX(), Game::getPlayer()->getY())) {
			m_chasing = true;
			return;
		}
		else m_chasing = false;


		m_result = 0;
		m_chasing = false;
		delete m_path;
		m_path = nullptr;
	}

	if (m_path && !m_chasing) followTarget();
}

void Creature::followTarget() {
	if (m_result == 0) {
		m_result = m_path->runNSteps(1);
		//oe::Logger::critical(m_result);
		m_wait = 2000;
		m_untilnexttarget = 10000;
		if (m_result != 0) {
			m_retrace = m_path->retrace();
			m_retrace_checkpoint = 0;
		}
	}

	if (m_result != 0) {

		if (m_retrace->size() <= 0) {
			delete m_path;
			m_path = nullptr;
			return;
		}

		//oe::Logger::info((abs(vel_x) + abs(vel_y)));
		if (abs(vel_x) + abs(vel_y) < 0.03) {
			m_stuck_timer++;

			if (m_stuck_timer > 100) {
				m_stuck_timer = 0;
				m_untilnexttarget = oe::Random::random(50, 200);
				m_wait = 4000;
				delete m_path;
				m_path = nullptr;
				return;
			}
		}

		int arr = m_retrace->size() - 1 - m_retrace_checkpoint;
		float mov_x = (m_retrace->at(arr).x - getX() + 0.5);
		float mov_y = (m_retrace->at(arr).y - getY() + 0.5);

		if (mov_x < 0.2 && mov_x > -0.2 && mov_y < 0.2 && mov_y > -0.2) {
			m_retrace_checkpoint++;
			if (m_retrace_checkpoint >= m_retrace->size()) {
				m_untilnexttarget = oe::Random::random(50, 200);
				m_wait = 4000;
				vel_x = 0;
				vel_y = 0;
				delete m_path;
				m_path = nullptr;
				return;
			}
		}

		vel_x = sign(mov_x) / 35.0f;
		vel_y = sign(mov_y) / 35.0f;
	}
}