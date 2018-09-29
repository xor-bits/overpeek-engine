#pragma once

#include <GL/glew.h>

class Tile {
private:
	int m_id;
	int m_object_id;
	long int m_x, m_y;

	float m_object_health;

public:
	Tile(long int x, long int y, int id, int object_id);

	inline int getX() { return m_x; }
	inline int getY() { return m_y; }
	inline int getId() { return m_id; }
	inline int getObjectId() { return m_object_id; }

	bool decreaceObjectHealth(float amount);
};