#pragma once

#include <GL/glew.h>
#include "../../engine.h"
#include "../logic/inventory.h"

class Region;
class Tile {
public:
	bool m_real = false;

private:
	int m_id;
	int m_object_id;
	long int m_x, m_y;
	Region *m_parent;

	float m_object_health;

public:
	Tile(long int x, long int y, int id, int object_id, Region *parent);
	Tile();

	int getTexture();
	int getObjectTexture();
	void update();

	void hitObject(float amount);

	inline int getX() { return m_x; }
	inline int getY() { return m_y; }
	inline int getId() { return m_id; }
	inline int getObjectId() { return m_object_id; }

	inline void setId(int id) { m_id = id; }
	inline void setObjectId(int id) { m_object_id = id; }

	inline void healObject() { m_object_health = 1.0; }
};