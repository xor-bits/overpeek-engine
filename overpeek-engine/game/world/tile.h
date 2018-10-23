#pragma once

#include <GL/glew.h>
#include "../../engine.h"
#include "../logic/inventory.h"

class Tile {
public:
	bool m_real = false;

private:
	int m_id;
	int m_object_id;
	long int m_x, m_y;

	float m_object_health;

public:
	Tile(long int x, long int y, int id, int object_id);
	Tile();

	int getTexture();
	int getObjectTexture();
	void update();

	inline int getX() { return m_x; }
	inline int getY() { return m_y; }
	inline int getId() { return m_id; }
	inline int getObjectId() { return m_object_id; }

	inline void setId(int id) { m_id = id; }
	inline void setObjectId(int id) { m_object_id = id; }

	inline void addObjectHealth(float amount) { 
		m_object_health += amount; 
		if (m_object_health <= 0) {
			m_object_id = 0;
			std::cout << "Drop item!" << std::endl;
		}
	}
	inline void hitObject(float damage, Inventory *inv) {
		m_object_health -= damage;
		if (m_object_health <= 0) {
			m_object_id = 0;
			std::cout << "Drop item!" << std::endl;
		}
	}
	inline void healObject() { m_object_health = 1.0; }
};