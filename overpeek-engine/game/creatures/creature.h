#pragma once

#include "../logic/game.h"
#include "../logic/inventory.h"

#define CREAURE_WIDTH 0.8
#define CREAURE_HEIGHT 0.8

class Creature {
protected:
	int m_counterToRemoveSwingAnimation = 0;
	Inventory *m_inv;
	int m_texture;
	int m_swingDir;

public:
	float x, y;
	float vel_x, vel_y;
	float acc_x, acc_y;
	uint8_t heading : 2;


	Creature(float x, float y, graphics::Shader *shader, Inventory *inv);

	virtual void submitToRenderer(graphics::Renderer *renderer, float renderOffsetX, float renderOffsetY);
	virtual void update();
	virtual void hit();
	virtual void place();

	void collide();

	inline int getRegionX() {
		if (x >= 0) return x / REGION_SIZE + 0.5;
		else return x / REGION_SIZE - 0.5;
	}
	inline int getRegionY() {
		if (y >= 0) return y / REGION_SIZE + 0.5;
		else return y / REGION_SIZE - 0.5;
	}
};