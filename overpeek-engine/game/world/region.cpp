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
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++)
		{
			long tilex = x + ((m_x - RENDER_DST / 2.0) * REGION_SIZE);
			long tiley = y + ((m_y - RENDER_DST / 2.0) * REGION_SIZE);
			double noisetilex = (tilex + INT_MAX / 2.0);
			double noisetiley = (tiley + INT_MAX / 2.0);

			int id = 0;
			int object_id = 0;
			Game::getInfoFromNoise(id, object_id, tilex, tiley);
			m_tiles[x][y] = new Tile(tilex, tiley, id, object_id, this);
		}
	}
}

void Region::loadTiles() {
	int tmp = 0;
	unsigned char *readData = (unsigned char*)tools::BinaryIO::read(getSaveLocation());
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

	unsigned char *readCreatures = (unsigned char*)tools::BinaryIO::read(getSaveLocation() + " c");
	for (int i = 0; i < readCreatures[0] * 4; i += 4)
	{
		if (readCreatures[i + 1] == 1) {
			addCreature(readCreatures[i + 1 + 1] - ceil(REGION_SIZE / 2.0) + getX(), readCreatures[i + 2 + 1] - ceil(REGION_SIZE / 2.0) + getY(), readCreatures[i + 0 + 1], (bool)readCreatures[i + 3 + 1]);
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
			dataCreature[i + 1 + 1] = floor(m_creatures[i / 4]->getX()) + ceil(REGION_SIZE / 2.0) - getX();
			dataCreature[i + 2 + 1] = floor(m_creatures[i / 4]->getY()) + ceil(REGION_SIZE / 2.0) - getY();
			dataCreature[i + 3 + 1] = (char)m_creatures[i / 4]->m_item;
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
			m_creatures[i]->collide();
			if (Game::posToRegionPos(m_creatures[i]->getX()) * REGION_SIZE != getX() || Game::posToRegionPos(m_creatures[i]->getY()) * REGION_SIZE != getY()) {
				Region *tmp = Game::getRegion(Game::posToRegionPos(m_creatures[i]->getX()) * REGION_SIZE, Game::posToRegionPos(m_creatures[i]->getY()) * REGION_SIZE);
				if (tmp) {
					tmp->addCreature(m_creatures[i]);
					removeCreature(i);
				}
				else {
					tmp = new Region(Game::posToRegionPos(m_creatures[i]->getX()) + floor(RENDER_DST / 2.0), Game::posToRegionPos(m_creatures[i]->getY()) + floor(RENDER_DST / 2.0));
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
			
			renderer->renderBox(renderx, rendery, TILE_SIZE, TILE_SIZE, 0, id);
			if (objid != 0) renderer->renderBox(renderx, rendery, TILE_SIZE, TILE_SIZE, 0, objid);
		}
	}

	if (!Game::debugMode) return;
	renderer->renderBox(rx, ry, 0.02, REGION_SIZE * TILE_SIZE, 0, 20);
	renderer->renderBox(rx + REGION_SIZE * TILE_SIZE - 0.02, ry, 0.02, REGION_SIZE * TILE_SIZE, 0, 21);
	renderer->renderBox(rx, ry, REGION_SIZE * TILE_SIZE, 0.02, 0, 22);
	renderer->renderBox(rx, ry + REGION_SIZE * TILE_SIZE - 0.02, REGION_SIZE * TILE_SIZE, 0.02, 0, 23);
}

void Region::submitCreaturesToRenderer(graphics::Renderer *renderer, float offx, float offy) {
	for (int i = 0; i < MAX_CREATURES; i++)
	{
		if (m_creatures[i]) m_creatures[i]->submitToRenderer(renderer, offx, offy);
	}
}