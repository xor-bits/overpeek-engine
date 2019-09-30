#include "item.h"

//#include "../pch.h"

#include "player.h"
#include "creature.h"

#include "../logic/game.h"
#include "../logic/inventory.h"
#include "../world/map.h"
#include "../logic/game.h"


Item::Item(float x, float y, int id) : Creature::Creature(x, y, id, true) {
	nothing = false;
}

void Item::ai(float divider) {
	//No ai
}

void Item::update(int index, float divider) {
	Creature::update(index, divider);

	//float distanceToPlayer = abs(getX() - Game::getPlayer()->getX()) + abs(getY() - Game::getPlayer()->getY());
	//if (distanceToPlayer < 0.8) {
	//	if (Game::getPlayer()->inventory->addItem(m_id, 1)) {
	//		Game::getMap()->removeCreature(index);
	//		oe::AudioManager::play(2);
	//		return;
	//	}
	//}
}

void Item::collide(float divider) {
	Creature::collide(divider);
}