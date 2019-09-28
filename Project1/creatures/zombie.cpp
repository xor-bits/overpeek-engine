#include "zombie.h"

#include "player.h"
#include "../logic/game.h"
#include "../logic/aStar.h"
#include "../settings.h"
#include "../logic/database.h"
#include "../logic/game.h"
#include "../settings.h"

//#include "../pch.h"

Zombie::Zombie(float x, float y) : Creature::Creature(x, y, 1, false) {
	m_untilnexttarget = 500;
	m_wait = 0;
	m_hit_cooldown = 0;
	m_chasing = false;
	m_curtarget_x = 0;
	m_curtarget_y = 0;
	m_check_player_cooldown = 0;
}

void Zombie::update(int index, float divider) {
	Creature::update(index, divider);

}

void Zombie::collide(float divider) {
	Creature::collide(divider);

}

void Zombie::ai(float divider) {
	m_untilnexttarget -= 1.0 / divider;
	m_wait -= 1.0 / divider;
	m_check_player_cooldown -= 1.0 / divider;
	
	if (m_check_player_cooldown <= 0) {
		m_check_player_cooldown = oe::Random::random(0, 1);
	
		if (canSee(Game::getPlayer()->getX(), Game::getPlayer()->getY())) {
			m_chasing = true;
			delete m_path;
			m_path = nullptr;
			return;
		}
		else m_chasing = false;
	}
	if (m_chasing) {
		glm::vec2 dstToPlayer = glm::vec2(Game::getPlayer()->getX() - getX(), Game::getPlayer()->getY() - getY());
		glm::vec2 dirToPlayer = glm::normalize(dstToPlayer);
		if (abs(dstToPlayer.x) > 1.2 || abs(dstToPlayer.y) > 1.2) {
			Creature::acc_x += dirToPlayer.x * Database::creatures[m_id].walkSpeed / UPDATES_PER_SECOND;
			Creature::acc_y += dirToPlayer.y * Database::creatures[m_id].walkSpeed / UPDATES_PER_SECOND;
			setHeading(Creature::acc_x, Creature::acc_y);
		}
		else {
			m_hit_cooldown += 1.0 / divider;
			if (m_hit_cooldown > 0.5) {
				m_hit_cooldown = 0;
				hit(0, 0);
			}
		}
		setHeading(dstToPlayer.x, dstToPlayer.y);
	}
	if (m_untilnexttarget < 0) {
		m_wait = 5.0;
		m_untilnexttarget = 20.0;
	
	
		m_curtarget_x = oe::Random::random(-16, 16);
		m_curtarget_y = oe::Random::random(-16, 16);
	
	
		unsigned startnode_x = (unsigned)oe::clamp(getX(), 0.0f, (float)MAP_SIZE);
		unsigned startnode_y = (unsigned)oe::clamp(getY(), 0.0f, (float)MAP_SIZE);
		unsigned endnode_x = (unsigned)oe::clamp(getX() + m_curtarget_x, 0.0f, (float)MAP_SIZE);
		unsigned endnode_y = (unsigned)oe::clamp(getY() + m_curtarget_y, 0.0f, (float)MAP_SIZE);
	
		m_path = new Pathfinder(startnode_x, startnode_y, endnode_x, endnode_y, 10);
		if (m_path->failed) {
			delete m_path;
			m_path = nullptr;
		}
		m_result = 0;
	}
	if (m_wait < 0) {
		m_untilnexttarget = oe::Random::random(0.5, 2);
		m_wait = 40;
	
	
		m_result = 0;
		m_chasing = false;
		delete m_path;
		m_path = nullptr;
	}
	
	if (m_path && !m_chasing) followTarget(divider);
}

void Zombie::followTarget(float divider) {
	if (m_result == 0) {
		m_result = m_path->runNSteps(200.0 / divider);
		m_wait = 20;
		m_untilnexttarget = 100;
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
	
		if (m_bumping) {
			m_stuck_timer += 1.0 / divider;
	
			if (m_stuck_timer > 1) {
				m_stuck_timer = 0;
				m_untilnexttarget = oe::Random::random(0.5, 2);
				m_wait = 40;
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
				m_untilnexttarget = oe::Random::random(0.5, 2);
				m_wait = 4000;
				vel_x = 0;
				vel_y = 0;
				delete m_path;
				m_path = nullptr;
				return;
			}
		}
	
		if (abs(mov_x) > 0.2) acc_x += oe::sign(mov_x) * Database::creatures[m_id].walkSpeed / UPDATES_PER_SECOND;
		if (abs(mov_y) > 0.2) acc_y += oe::sign(mov_y) * Database::creatures[m_id].walkSpeed / UPDATES_PER_SECOND;
		setHeading(acc_x, acc_y);
	}
}

void Zombie::submitToRenderer(oe::Renderer* renderer, float renderOffsetX, float renderOffsetY, float corrector, float renderScale) {
	Creature::submitToRenderer(renderer, renderOffsetX, renderOffsetY, corrector, renderScale);

	if (m_path && m_result != 0 && Game::advancedDebugMode) m_path->debugRender(renderer, renderOffsetX, renderOffsetY);
}