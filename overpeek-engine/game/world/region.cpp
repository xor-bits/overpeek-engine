#include "region.h"

#include "../../engine.h"
#include "../logic/game.h"

Region::Region(int x, int y) {
	m_x = x; m_y = y;
	createTiles();
}

void Region::createTiles() {
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++)
		{
			long int tilex = x + ((m_x - RENDER_DST / 2.0) * REGION_SIZE);
			long int tiley = y + ((m_y - RENDER_DST / 2.0) * REGION_SIZE);
			float height = logic::Noise::octaveNoise((tilex + INT_MAX / 2) * NOISE_SCALE, (tiley + INT_MAX / 2) * NOISE_SCALE, 3);
			int id = 3;
			int object_id = 0;

			if (height <= LEVEL_WATER) id = 1;
			else if (height <= LEVEL_SAND) id = 2;
			else if (height <= LEVEL_GRASS) id = 3;
			else {
				if (tools::Random::random(-0.5, 1.5) > 0.5) object_id = 5;
			}

			m_tiles[x][y] = new Tile(tilex, tiley, id, object_id);
		}
	}
}

void Region::render(float offx, float offy) {
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++)
		{
			m_tiles[x][y]->render(offx, offy);
		}
	}
}

void Region::update() {
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++)
		{
			m_tiles[x][y]->update();
		}
	}
}