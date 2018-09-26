#pragma once

#include <GL/glew.h>

class Tile {
private:
	int m_id;
	int m_object_id;
	long int m_x, m_y;
	GLuint m_texture;
	GLuint m_object_texture;

	float m_object_health;

public:
	Tile(long int x, long int y, int id, int object_id);
	~Tile();

	void render(float offx, float offy);
	void update();

	inline int getX() { return m_x; }
	inline int getY() { return m_y; }
	inline int getId() { return m_id; }
	inline int getObjectId() { return m_object_id; }
	inline bool decreaceObjectHealth(float amount) { 
		m_object_health -= amount;
		if (m_object_health < 0) {
			m_object_id = 0;
			return true;
		}
		return false;
	}
};