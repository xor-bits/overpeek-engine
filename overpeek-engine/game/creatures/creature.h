#pragma once

#include "../logic/game.h"
#include "../logic/inventory.h"

#define CREAURE_WIDTH 0.8
#define CREAURE_HEIGHT 0.8

class Creature {
protected:
	int m_counterToRemoveSwingAnimation = 0;
	int m_id;
	int m_swingDir;

	int m_untilnexttarget;
	int m_wait;
	float m_curtarget_x;
	float m_curtarget_y;

	void enemyAi();

public:
	float x, y;
	float vel_x, vel_y;
	float acc_x, acc_y;
	uint8_t heading : 2;


	Creature(float x, float y, int id);
	Creature();

	virtual void submitToRenderer(graphics::Renderer *renderer, float renderOffsetX, float renderOffsetY);
	virtual void update();
	virtual void hit();

	void collide();

	inline int getId() { return m_id; }

	inline int getRegionX() {
		if (x >= 0) return x / REGION_SIZE + 0.5;
		else return x / REGION_SIZE - 0.5;
	}
	inline int getRegionY() {
		if (y >= 0) return y / REGION_SIZE + 0.5;
		else return y / REGION_SIZE - 0.5;
	}
};