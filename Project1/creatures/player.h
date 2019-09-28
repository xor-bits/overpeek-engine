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

	bool m_no_clip = false;
	bool m_god = false;

public:
	Inventory *inventory;
	
public:
	Player(float x, float y, Inventory *inv);
	void die();
	void setSpawnPoint(unsigned int x, unsigned int y);
	void getSpawnPoint(int &x, int &y);

	void submitToRenderer(oe::Renderer *renderer, float renderOffsetX, float renderOffsetY, float corrector, float renderScale);
	void update(float divider);
	void collide(float divider);
	void ai(float divider) {}
	void hit();
	void mouseHit(int button, int action);
	void place();
	void save();
	bool load();

	inline void setGodmode(bool mode) { m_god = mode; }
	inline void setClipmode(bool mode) { m_no_clip = mode; }
	inline bool getGodmode() { return m_god; }
	inline bool getClipmode() { return m_no_clip; }
};