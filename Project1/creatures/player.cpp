#include "../pch.h"


#include "player.h"

#include "../logic/game.h"
#include "../logic/inventory.h"
#include "../world/tile.h"
#include "../world/region.h"
#include "../world/map.h"
#include "../logic/gui.h"

Player::Player(float x, float y, Inventory *inv) : Creature(x, y, 0, false) {
	inventory = inv;

	m_death_x = -1;
	m_death_y = -1;
}

void Player::submitToRenderer(oe::Renderer *renderer, float renderOffsetX, float renderOffsetY, float corrector) {
	Creature::submitToRenderer(renderer, renderOffsetX, renderOffsetY, corrector);

	if (m_death_x != -1) {

		float renderDeathX = oe::clamp((m_death_x - (getX() + vel_x * corrector/ UPDATES_PER_SECOND)) * TILE_SIZE, -Game::getWindow()->getAspect(), Game::getWindow()->getAspect() - 0.1f);
		float renderDeathY = oe::clamp((m_death_y - (getY() + vel_y * corrector/ UPDATES_PER_SECOND)) * TILE_SIZE, -1.0f, 1.0f - 0.1f);

		glm::vec3 pos = glm::vec3(renderDeathX, renderDeathY, 0.0f);
		glm::vec2 size = glm::vec2(TILE_SIZE) * Game::renderScale();
		unsigned int deathmarker = 21;

		renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y, pos.z), glm::vec2(0.0f, 0.0f), deathmarker, OE_COLOR_WHITE));
		renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y + size.y, pos.z), glm::vec2(0.0f, 1.0f), deathmarker, OE_COLOR_WHITE));
		renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x + size.x, pos.y + size.y, pos.z), glm::vec2(1.0f, 1.0f), deathmarker, OE_COLOR_WHITE));
		renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x + size.x, pos.y, pos.z), glm::vec2(1.0f, 0.0f), deathmarker, OE_COLOR_WHITE));
	}

	double mx, my;
	Game::getWindow()->getMousePos(mx, my);
	mx /= TILE_SIZE;
	my /= TILE_SIZE;


	if (Database::items[inventory->selectedId].placedAs != 0) {
		int x_dst = abs((int)getX() - floor(getX() + mx));
		int y_dst = abs((int)getY() - floor(getY() + my));
		if (x_dst == 0) y_dst++;
		if (y_dst == 0) x_dst++;
		if (x_dst + y_dst < m_hitdist) {
			Game::getMap()->renderGhostObject(
				renderer, 
				getX() + vel_x * corrector / UPDATES_PER_SECOND + mx, 
				getY() + vel_y * corrector / UPDATES_PER_SECOND + my, 
				Database::items[inventory->selectedId].placedAs, 
				-getX() - vel_x * corrector / UPDATES_PER_SECOND,
				-getY() - vel_y * corrector / UPDATES_PER_SECOND,
				corrector
			);
		}
	}

}

void Player::die() {
	if (m_god) return;

	inventory->dropAll();
	Game::getGui()->addChatLine("Player died at " + std::to_string(getX()) + ", " + std::to_string(getY()));

	m_death_x = getX();
	m_death_y = getY();

	resetHealth();
	resetStamina();
	setX(m_spawn_location_x);
	setY(m_spawn_location_y);
}

void Player::setSpawnPoint(unsigned int x, unsigned int y) {
	m_spawn_location_x = x;
	m_spawn_location_y = y;
	if (Game::getGui()) Game::getGui()->addChatLine("Spawnpoint set");
}

void Player::getSpawnPoint(int &x, int &y) {
	x = m_spawn_location_x;
	y = m_spawn_location_y;
}

void Player::update(float divider) {
#if USE_MOUSE
	double mx, my;
	Game::getWindow()->getMousePos(mx, my);

	//Heading
	float angle = atan2(mx, my) + glm::pi<float>() / 4;
	if (angle > 0 && angle < glm::pi<float>() / 2) {
		heading = HEADING_DOWN;
	} 
	else if (angle > glm::pi<float>() / 2 && angle < glm::pi<float>()) {
		heading = HEADING_RIGHT;
	}
	else if (angle > glm::pi<float>() / -2 && angle < 0) {
		heading = HEADING_LEFT;
	}
	else {
		heading = HEADING_UP;
	}
	if (Game::advancedDebugMode) m_hitdist = 100;
	else m_hitdist = 5.0;
#endif

	if (m_death_x != -1) {

		float dstToDeathX = abs(getX() - m_death_x);
		float dstToDeathY = abs(getY() - m_death_y);

		if (dstToDeathX < 2 && dstToDeathY < 2) {
			m_death_x = -1;
		}
	}
	if (Game::advancedDebugMode || m_god) m_stamina += 1000000;
	if (Game::advancedDebugMode || m_god) m_health += 1000000;

	clampHPAndSTA();
	if (m_health <= 0) {
		die();
		return;
	}

	Creature::update(0, divider);
}

void Player::collide(float divider) {
	if (!m_no_clip) Creature::collide(divider);
}

void Player::hit() {
	if (Database::items[inventory->selectedId].placedAs != 0) {
		place();
	}
	else {
		setStamina(getStamina() - 0.2);
		if (exhausted()) return;

		float dmgadd = Database::items[inventory->selectedId].melee_damage;
		float kbadd = Database::items[inventory->selectedId].melee_kb;
		if (Game::advancedDebugMode) dmgadd *= 100.0;
		Creature::hit(dmgadd, kbadd);
	}
}

void Player::mouseHit(int button, int action) {
#if USE_MOUSE

	if (action != OE_PRESS) return;

	double mx, my;
	Game::getWindow()->getMousePos(mx, my);
	mx /= TILE_SIZE;
	my /= TILE_SIZE;

	int x_dst = abs((int)getX() - floor(getX() + mx));
	int y_dst = abs((int)getY() - floor(getY() + my));
	if (x_dst == 0) y_dst++;
	if (y_dst == 0) x_dst++;

	if (x_dst + y_dst < m_hitdist) {
		Map::MapTile *tile = Game::getMap()->getTile(getX() + mx, getY() + my);
		//PLACE
		if (Database::items[inventory->selectedId].placedAs != 0) {
			if (button == OE_MOUSE_BUTTON_RIGHT) {
				if (!Game::getMap()->trySetObject(tile, Database::items[inventory->selectedId].placedAs)) {
					if (!Game::advancedDebugMode) inventory->removeSelected(1);
					oe::AudioManager::play(1);
				}
			}
		}

		//INTERACT
		if (button == OE_MOUSE_BUTTON_RIGHT) {
			//if (tile->m_object != 0) {
			//	Game::getGui()->addChatLine("Interact " + Database::objects[tile->m_object].name);
			//}W
		}

		//BREAK
		if (button == OE_MOUSE_BUTTON_LEFT) {
			setStamina(getStamina() - 0.2);
			if (exhausted()) return;

			float break_speed = Database::items[inventory->selectedId].break_speed;
			if (Game::advancedDebugMode) break_speed = 100.0;

			if (tile) {
				if (Database::objects[tile->m_object].destructable) {
					Game::getMap()->hit(getX() + mx, getY() + my, 1 + break_speed);
				}
			}
		}
	}
	if (button == OE_MOUSE_BUTTON_LEFT) {
		setStamina(getStamina() - 0.2);
		if (exhausted()) return;
		float dmgadd = Database::items[inventory->selectedId].melee_damage;
		float kbadd = Database::items[inventory->selectedId].melee_kb;
		if (Game::advancedDebugMode) kbadd += 100.0;
		Creature::hit(dmgadd, kbadd);
	}

#endif
}

void Player::place() {
#if !STORE_MAP_IN_RAM
	Tile *tmp;
	switch (heading)
	{
	case 0:
		tmp = Game::getTile(getX(), getY() - 1, "player");
		break;
	case 1:
		tmp = Game::getTile(getX() + 1, getY(), "player");
		break;
	case 2:
		tmp = Game::getTile(getX(), getY() + 1, "player");
		break;
	default:
		tmp = Game::getTile(getX() - 1, getY(), "player");
		break;
	}

	if (tmp) {
		Game::trySetTileObject(tmp, 4);
	}
#else
	Map::MapTile* tmp;
	switch (heading)
	{
	case 0:
		tmp = Game::getMap()->getTile(getX(), getY() - 1);
		//m_swingDir = 1;
		break;
	case 1:
		tmp = Game::getMap()->getTile(getX() + 1, getY());
		//m_swingDir = 2;
		break;
	case 2:
		tmp = Game::getMap()->getTile(getX(), getY() + 1);
		//m_swingDir = 3;
		break;
	default:
		tmp = Game::getMap()->getTile(getX() - 1, getY());
		//m_swingDir = 4;
		break;
	}

	if (tmp) {
		if (!Game::getMap()->trySetObject(tmp, Database::items[inventory->selectedId].placedAs)) {
			inventory->removeSelected(1);
			oe::AudioManager::play(1);
		}
	}
#endif
}

void Player::save() {
	inventory->save();
	float playerData[4] = { getX(), getY(), (float)m_spawn_location_x, (float)m_spawn_location_y };
	oe::BinaryIO::write<float>(Game::getMap()->saveLocation(Game::getSaveLocation()) + "player.data", playerData, 4);
}

bool Player::load() {
	inventory->load();

	unsigned long data_size;
	float *playerData = (float*)oe::BinaryIO::read<float>(Game::getMap()->saveLocation(Game::getSaveLocation()) + "player.data", data_size);
	if (!playerData) {
		setSpawnPoint(getX(), getY());
		return false;
	}
	setX(playerData[0]);
	setY(playerData[1]);
	setSpawnPoint(playerData[2], playerData[3]);
}