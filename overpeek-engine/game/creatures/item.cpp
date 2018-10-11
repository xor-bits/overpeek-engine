#include "item.h"
#include "../../engine.h"
#include "../logic/game.h"

Item::Item(float x, float y) : Creature(x, y, 3) {
	m_id = 1;
}

void Item::submitToRenderer(graphics::Renderer *renderer, float renderOffsetX, float renderOffsetY) {
	Creature::submitToRenderer(renderer, renderOffsetX, renderOffsetY);
}

void Item::update() {
	Creature::update();
	Creature::collide();
}