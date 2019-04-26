#pragma once

#include "creature.h"

class Item : public Creature {
private:
	bool nothing;
	
public:
	Item(float x, float y, int id);
	~Item();

	void ai(float divider);
	void update(int index, float divider);
	void collide(float divider);
};