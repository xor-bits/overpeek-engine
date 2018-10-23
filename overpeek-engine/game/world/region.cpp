#include "region.h"

Region::Region(int x, int y) {
	m_x = x; m_y = y;
	for (int i = 0; i < MAX_CREATURES; i++) m_creatures[i] = nullptr;
	if (tools::BinaryIO::read(getSaveLocation()) != nullptr) loadTiles();
	else createTiles();
}

Region::~Region() {
	saveTiles();
}

void Region::createTiles() {
	int tmp = 0;
	addCreature(getX(), getY(), 1, false);
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++)
		{
			long int tilex = x + ((m_x - RENDER_DST / 2.0) * REGION_SIZE);
			long int tiley = y + ((m_y - RENDER_DST / 2.0) * REGION_SIZE);
			float height = (logic::Noise::octaveNoise((tilex + INT_MAX / 2) * NOISE_SCALE, (tiley + INT_MAX / 2) * NOISE_SCALE, 3) + 1.0) / 2.0;
			int id = 0;
			int object_id = 0;

			//height is from 0 to 1
			if (height <= LEVEL_WATER) id = 1;
			else if (height <= LEVEL_SAND) id = 2;
			else if (height <= LEVEL_SOIL) {
				float n = (logic::Noise::octaveNoise((tilex + INT_MAX / 2) * NOISE_SCALE * 3.0, (tiley + INT_MAX / 2) * NOISE_SCALE * 3.0, 3) + 1.0) / 2.0;
				if (n > 0.7 || n < 0.3) id = 3;
				else  id = 0;

				if (n > 0.8) object_id = 2;
				else if (n < 0.2) object_id = 3;
				else if (n > 0.7 || n < 0.3) {
					if (tools::Random::random(0.0, 1.0) > 0.5) object_id = 1;
				}
			}
			else if (height <= LEVEL_STONE) id = 4;
			else { id = 4; object_id = 5; }

			m_tiles[x][y] = new Tile(tilex, tiley, id, object_id, this);
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
			m_tiles[x][y] = new Tile(tilex, tiley, id, objid, this);
		}
	}

	unsigned char *readCreatures = tools::BinaryIO::read(getSaveLocation() + " c");
	for (int i = 0; i < readCreatures[0] * 4; i += 4)
	{
		if (readCreatures[i + 1] == 1) {
			//std::cout << "add loaded creature" << std::endl;
			addCreature(readCreatures[i + 1 + 1] - ceil(REGION_SIZE / 2.0) + getX(), readCreatures[i + 2 + 1] - ceil(REGION_SIZE / 2.0) + getY(), readCreatures[i + 0 + 1], (bool)readCreatures[i + 3 + 1]);
			//std::cout << (int)readCreatures[i + 0] << " ";
			//std::cout << (int)readCreatures[i + 1] - ceil(REGION_SIZE / 2.0) + getX() << " ";
			//std::cout << (int)readCreatures[i + 2] - ceil(REGION_SIZE / 2.0) + getY() << " ";
		}
	}
}

void Region::saveTiles() {
	unsigned char data[REGION_SIZE*REGION_SIZE * 2 + 1];
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++)
		{
			data[x + (y * REGION_SIZE)] = m_tiles[x][y]->getId();
			data[x + (y * REGION_SIZE) + REGION_SIZE * REGION_SIZE] = m_tiles[x][y]->getObjectId();
		}
	}

	unsigned char dataCreature[MAX_CREATURES * 4 + 1];
	int amountOfCreatures = 0;
	for (int i = 0; i < MAX_CREATURES * 4; i += 4)
	{
		if (m_creatures[i / 4]) {
			amountOfCreatures++;
			dataCreature[i + 0 + 1] = m_creatures[i / 4]->getId();
			dataCreature[i + 1 + 1] = floor(m_creatures[i / 4]->x) + ceil(REGION_SIZE / 2.0) - getX();
			dataCreature[i + 2 + 1] = floor(m_creatures[i / 4]->y) + ceil(REGION_SIZE / 2.0) - getY();
			dataCreature[i + 3 + 1] = (char)m_creatures[i / 4]->m_item;
			//std::cout << (int)dataCreature[i + 0] << " ";
			//std::cout << (int)dataCreature[i + 1] << " ";
			//std::cout << (int)dataCreature[i + 2] << " ";
		}
		else {
			dataCreature[i + 0 + 1] = 0;
			dataCreature[i + 1 + 1] = 0;
			dataCreature[i + 2 + 1] = 0;
			dataCreature[i + 3 + 1] = 0;
		}
	}

	dataCreature[0] = amountOfCreatures;
	
	tools::BinaryIO::write(getSaveLocation(), data, sizeof(data) / sizeof(unsigned char));
	tools::BinaryIO::write(getSaveLocation() + " c", dataCreature, amountOfCreatures * 4 + 1);
}

void Region::update() {
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++)
		{
			m_tiles[x][y]->update();
		}
	}

	for (int i = 0; i < MAX_CREATURES; i++)
	{
		if (m_creatures[i]) m_creatures[i]->update();
		if (m_creatures[i]) {
			if (m_creatures[i]->getRegionX() * REGION_SIZE != getX() || m_creatures[i]->getRegionY() * REGION_SIZE != getY()) {
				//std::cout << "Creature region: " << m_creatures[i]->getRegionX() * REGION_SIZE << ", " << m_creatures[i]->getRegionY() * REGION_SIZE << std::endl;
				//std::cout << "This region: " << getX() << ", " << getY() << std::endl;
				Region *tmp;

				tmp = Game::getRegion(m_creatures[i]->getRegionX() * REGION_SIZE, m_creatures[i]->getRegionY() * REGION_SIZE);
				if (tmp) {
					//tmp->addCreature(m_creatures[i]->x, m_creatures[i]->y, m_creatures[i]->getId());
					tmp->addCreature(m_creatures[i]);
					removeCreature(i);
				}
				else {
					//std::cout << "This: " << getX() / REGION_SIZE << ", " << getY() / REGION_SIZE << std::endl;
					//std::cout << "This nonformated: " << m_x << ", " << m_y<< std::endl;
					//std::cout << "New: " << m_creatures[i]->getRegionX() << ", " << m_creatures[i]->getRegionY() << std::endl;
					//std::cout << "New: " << m_creatures[i]->getRegionX() + floor(RENDER_DST / 2.0) << ", " << m_creatures[i]->getRegionY() + floor(RENDER_DST / 2.0) << std::endl;
					tmp = new Region(m_creatures[i]->getRegionX() + floor(RENDER_DST / 2.0), m_creatures[i]->getRegionY() + floor(RENDER_DST / 2.0));
					//tmp->addCreature(m_creatures[i]->x, m_creatures[i]->y, m_creatures[i]->getId());
					tmp->addCreature(m_creatures[i]);
					removeCreature(i);
					tmp->saveTiles();
					delete tmp;
				}
			}
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
			int id = m_tiles[x][y]->getTexture();
			int objid = m_tiles[x][y]->getObjectTexture();

			float renderx = x * TILE_SIZE + rx, rendery = y * TILE_SIZE + ry;
			renderer->renderBox(renderx, rendery, TILE_SIZE, TILE_SIZE, id);
			renderer->renderBox(renderx, rendery, TILE_SIZE, TILE_SIZE, objid);
		}
	}
}

void Region::submitCreaturesToRenderer(graphics::Renderer *renderer, float offx, float offy) {
	for (int i = 0; i < MAX_CREATURES; i++)
	{
		if (m_creatures[i]) m_creatures[i]->submitToRenderer(renderer, offx, offy);
	}
}