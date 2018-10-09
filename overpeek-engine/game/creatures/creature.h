#pragma once

#include "../logic/game.h"
#include "../logic/inventory.h"

#define CREAURE_WIDTH 0.8
#define CREAURE_HEIGHT 0.8

class Creature {
protected:
	Inventory *m_inv;
	float m_x, m_y;
	float m_vel_x, m_vel_y;
	float m_acc_x, m_acc_y;
	int m_texture;
	int m_swingDir;

public:
	Creature(float x, float y, graphics::Shader *shader, Inventory *inv);

	virtual void submitToRenderer(graphics::Renderer *renderer, float renderOffsetX, float renderOffsetY);
	virtual void update();

	void collide();

	inline void setX(float value) { m_x = value; }
	inline void setVelX(float value) { m_vel_x = value; }
	inline void setAccX(float value) { m_acc_x = value; }
	inline void setY(float value) { m_y = value; }
	inline void setVelY(float value) { m_vel_y = value; }
	inline void setAccY(float value) { m_acc_y = value; }

	inline float getX() { return m_x; }
	inline float getVelX() { return m_vel_x; }
	inline float getAccX() { return m_acc_x; }
	inline float getY() { return m_y; }
	inline float getVelY() { return m_vel_y; }
	inline float getAccY() { return m_acc_y; }
	inline int getRegionX() {
		if (m_x >= 0) return m_x / REGION_SIZE + 0.5;
		else return m_x / REGION_SIZE - 0.5;
	}
	inline int getRegionY() {
		if (m_y >= 0) return m_y / REGION_SIZE + 0.5;
		else return m_y / REGION_SIZE - 0.5;
	}
};