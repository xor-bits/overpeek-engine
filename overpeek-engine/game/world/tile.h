#pragma once

#include <GL/glew.h>
#include "../../engine.h"

/*
	FLOORS
	0	NULL
	1	LAWN
	2	WATER
	3	SAND
	4	STONE

	OBJECTS
	6	GRASS
	7	TREE
	8	TREE2
	9	STONE
*/

class Tile {
private:
	int m_id;
	int m_object_id;
	long int m_x, m_y;

	float m_object_health;

public:
	Tile(long int x, long int y, int id, int object_id);

	int getFloorTexture();
	int getObjectTexture();
	void update();

	inline int getX() { return m_x; }
	inline int getY() { return m_y; }
	inline int getId() { return m_id; }
	inline int getObjectId() { return m_object_id; }

	inline void setId(int id) { m_id = id; }
	inline void setObjectId(int id) { m_object_id = id; }

	static int idToTextureId(int id);

	bool decreaceObjectHealth(float amount);
	inline void healObject() { m_object_health = 1.0; }
};