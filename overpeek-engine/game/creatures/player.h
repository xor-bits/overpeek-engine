#pragma once

#include "../logic/game.h"
#include "creature.h"

class Player : public Creature {
private:

public:
	Player(float x, float y, graphics::Shader *shader);

	void submitToRenderer(graphics::Renderer *renderer, float renderOffsetX, float renderOffsetY);
	void update();
};