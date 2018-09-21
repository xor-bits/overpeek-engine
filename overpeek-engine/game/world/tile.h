#pragma once

class Tile {
private:
	int m_id;
	long int m_x, m_y;

public:
	Tile(long int x, long int y, int id);

	void render();
	void update();
};