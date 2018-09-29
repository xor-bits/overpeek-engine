#include "tile.h"

#include "region.h"
#include "../../engine.h"
#include "../logic/game.h"

Tile::Tile(long int x, long int y, int id, int object_id) {
	m_id = id; m_object_id = object_id; m_x = x; m_y = y;
	m_object_health = 1.0;
}

bool Tile::decreaceObjectHealth(float amount) {
	m_object_health -= amount;
	if (m_object_health < 0) {
		m_object_id = 5;
		return true;
	}
	return false;
}