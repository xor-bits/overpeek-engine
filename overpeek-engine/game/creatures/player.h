#pragma once

#include "../logic/game.h"
#include "creature.h"

class Player : public Creature {
public:
	Player(float x, float y, graphics::Shader *shader, Inventory *inv);

	void submitToRenderer(graphics::Renderer *renderer, float renderOffsetX, float renderOffsetY);
	void update();
	void hit();
};