#include "region.h"

Region::Region(int x, int y) {
	m_x = x; m_y = y;
	m_creatureAmount = 0;
	if (tools::BinaryIO::read(getSaveLocation()) != nullptr) loadTiles();
	else createTiles();
}

Region::~Region() {
	saveTiles();
}

void Region::createTiles() {
	int tmp = 0;
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++)
		{
			long int tilex = x + ((m_x - RENDER_DST / 2.0) * REGION_SIZE);
			long int tiley = y + ((m_y - RENDER_DST / 2.0) * REGION_SIZE);
			float height = (logic::Noise::octaveNoise((tilex + INT_MAX / 2) * NOISE_SCALE, (tiley + INT_MAX / 2) * NOISE_SCALE, 3) + 1.0) / 2.0;
			int id = 1;
			int object_id = 5;

			//height is from 0 to 1
			if (height <= LEVEL_WATER) id = 2;
			else if (height <= LEVEL_SAND) id = 3;
			else if (height <= LEVEL_GRASS) {
				id = 1;
				float d = tools::Random::random(-0.5, 1.5);
				if (d > 1.0) object_id = 6;
				else if (d > 0.5) object_id = 7;
				else if (d > 0.0) object_id = 8;
			}
			else if (height <= LEVEL_STONE) {
				id = 4;
			}
			else {
				id = 4;
				object_id = 9;
			}

			m_tiles[x][y] = Tile(tilex, tiley, id, object_id);
		}
	}
}

void Region::loadTiles() {
	int tmp = 0;
	unsigned char *readData = tools::BinaryIO::read(getSaveLocation());
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++)
		{
			long int tilex = x + ((m_x - RENDER_DST / 2.0) * REGION_SIZE);
			long int tiley = y + ((m_y - RENDER_DST / 2.0) * REGION_SIZE);
			int id = readData[x + (y * REGION_SIZE)];
			int objid = readData[x + (y * REGION_SIZE) + REGION_SIZE * REGION_SIZE];
			m_tiles[x][y] = Tile(tilex, tiley, id, objid);
		}
	}
}

void Region::saveTiles() {
	unsigned char data[REGION_SIZE*REGION_SIZE*2];
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++)
		{
			data[x + (y * REGION_SIZE)] = m_tiles[x][y].getId();
			data[x + (y * REGION_SIZE) + REGION_SIZE * REGION_SIZE] = m_tiles[x][y].getObjectId();
		}
	}
	tools::BinaryIO::write(getSaveLocation(), data, sizeof(data) / sizeof(unsigned char));
}

void Region::update() {
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++)
		{
			m_tiles[x][y].update();
		}
	}
}

void Region::submitToRenderer(graphics::Renderer *renderer, float offx, float offy) {
	float rx = (m_x - RENDER_DST / 2.0) * (float)TILE_SIZE * REGION_SIZE + offx * TILE_SIZE;
	float ry = (m_y - RENDER_DST / 2.0) * (float)TILE_SIZE * REGION_SIZE + offy * TILE_SIZE;
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++)
		{
			int id = m_tiles[x][y].getFloorTexture();
			int objid = m_tiles[x][y].getObjectTexture();

			glm::vec2 arrayid = glm::vec2(id % 16, (id - (id % 16)) / 16);
			glm::vec2 arrayobjectid = glm::vec2(objid % 16, (objid - (objid % 16)) / 16);

			float renderx = x * TILE_SIZE + rx, rendery = y * TILE_SIZE + ry;
			renderer->renderBox(renderx, rendery, TILE_SIZE, TILE_SIZE, arrayid.x / 16.0, arrayid.y / 16.0, 1 / 16.0, 1 / 16.0);
			renderer->renderBox(renderx, rendery, TILE_SIZE, TILE_SIZE, arrayobjectid.x / 16.0, arrayobjectid.y / 16.0, 1 / 16.0, 1 / 16.0);
		}
	}
}