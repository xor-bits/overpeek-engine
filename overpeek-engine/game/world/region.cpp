#include "region.h"

#include "../../engine.h"

Region::Region(int x, int y) {
	m_x = x; m_y = y;
	createTiles();
}

void Region::createTiles() {
	for (int x = 0; x < SIZE; x++)
	{
		for (int y = 0; y < SIZE; y++)
		{
			int id = ((x + y) % 2) + 1;
			m_tiles[x][y] = new Tile(x + (m_x * SIZE), y + (m_y * SIZE), id);
		}
	}
}

void Region::render() {
	for (int x = 0; x < SIZE; x++)
	{
		for (int y = 0; y < SIZE; y++)
		{
			m_tiles[x][y]->render();
		}
	}
}

void Region::update() {
	for (int x = 0; x < SIZE; x++)
	{
		for (int y = 0; y < SIZE; y++)
		{
			m_tiles[x][y]->update();
		}
	}
}