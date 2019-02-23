#pragma once

#include "../logic/game.h"
#include "creature.h"

class Inventory;
class Player : public Creature {
private:
	unsigned int m_spawn_location_x;
	unsigned int m_spawn_location_y;

	int m_death_x;
	int m_death_y;

	float m_hitdist = 5.0;

public:
	Inventory *inventory;
	
public:
	Player() {}
	Player(float x, float y, Inventory *inv);
	void die();
	void setSpawnPoint(unsigned int x, unsigned int y);
	void getSpawnPoint(int &x, int &y);

	void submitToRenderer(oe::Renderer *renderer, float renderOffsetX, float renderOffsetY);
	void update();
	void collide();
	void hit();
	void mouseHit(int button, int action);
	void place();
	void save();
	bool load();
};