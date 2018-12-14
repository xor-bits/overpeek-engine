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
	unsigned int m_localX, m_localY;

	float m_object_health;

public:
	Tile(long int x, long int y, int id, int object_id, Region *parent, unsigned int localX, unsigned int localY);
	Tile();
	~Tile();

	int getTexture();
	int getObjectTexture();
	void update();


	void setId(int id);
	void setObjectId(int id);

	void healObject();
	void hitObject(float amount);

	inline int getX() { if (!this) return 0; else return m_x; }
	inline int getY() { if (!this) return 0; else return m_y; }
	inline int getLocalX() { if (!this) return 0; else return m_localX; }
	inline int getLocalY() { if (!this) return 0; else return m_localY; }
	inline int getId() { if (!this) return 0; else return m_id; }
	inline int getObjectId() { if (!this) return 0; else return m_object_id; }

	Region* getRegion();
};