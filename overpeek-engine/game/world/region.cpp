#include "region.h"


Region::Region(int x, int y) {
	m_x = x; m_y = y; null = false;
	for (int i = 0; i < MAX_CREATURES; i++) m_creatures[i] = nullptr;

#if !DEBUG_DISABLE_SAVING
	void* tileData = tools::BinaryIO::read<unsigned int>(getSaveLocation());
	if (tileData != nullptr) {
		loadTiles((unsigned char*)tileData, (float*)tools::BinaryIO::read<float>(getSaveLocation() + " c"));
	}
	else {
		createTiles();
	}
#else
	createTiles();
#endif
}

Region::Region() {
	null = true;
	for (int i = 0; i < MAX_CREATURES; i++) m_creatures[i] = nullptr;
}

Region::~Region() {}

std::string Region::getSaveLocation() {
	std::string regionname = std::to_string(m_x) + ", " + std::to_string(m_y);
	return (SAVE_PATH + WORLD_NAME + "\\regions\\" + regionname).c_str();
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
			m_tiles[x][y] = Tile(tilex, tiley, id, object_id, x, y);
			m_renderIdArray[x][y] = Database::tiles[id].texture;
			m_renderIdObjectArray[x][y] = Database::objects[object_id].texture;
		}
	}
}

void Region::tilesChanged() {
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++)
		{
			m_renderIdArray[x][y] = m_tiles[x][y].getTexture();
			m_renderIdObjectArray[x][y] = m_tiles[x][y].getObjectTexture();
		}
	}
}

void Region::deleteRegion() {
	saveTiles();
}

void Region::loadTiles(unsigned char *tileData, float *creatureData) {
	int tmp = 0;
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++)
		{
			long int tilex = x + ((m_x - RENDER_DST / 2.0) * REGION_SIZE);
			long int tiley = y + ((m_y - RENDER_DST / 2.0) * REGION_SIZE);
			int id = tileData[x + (y * REGION_SIZE)];
			int objid = tileData[x + (y * REGION_SIZE) + REGION_SIZE * REGION_SIZE];
			m_tiles[x][y] = Tile(tilex, tiley, id, objid, x, y);
		}
	}

	if (creatureData) {
		for (int i = 0; i < creatureData[0] * 4; i += 4)
		{
			addCreature(
				creatureData[i + 1 + 1] - ceil(REGION_SIZE / 2.0) + getX(),
				creatureData[i + 2 + 1] - ceil(REGION_SIZE / 2.0) + getY(),
				creatureData[i + 0 + 1],
				creatureData[i + 3 + 1]
			);
		}
	}
}

void Region::saveTiles() {
	unsigned char data[REGION_SIZE*REGION_SIZE * 2 + 1];
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++)
		{
			data[x + (y * REGION_SIZE)] = m_tiles[x][y].getId();
			data[x + (y * REGION_SIZE) + REGION_SIZE * REGION_SIZE] = m_tiles[x][y].getObjectId();
		}
	}

	float dataCreature[MAX_CREATURES * 4 + 1];
	int amountOfCreatures = 0;
	for (int i = 0; i < MAX_CREATURES * 4; i += 4)
	{
		if (m_creatures[i / 4]) {
			amountOfCreatures++;
			dataCreature[i + 0 + 1] = m_creatures[i / 4]->getId();
			dataCreature[i + 1 + 1] = m_creatures[i / 4]->getX() + ceil(REGION_SIZE / 2.0) - getX();
			dataCreature[i + 2 + 1] = m_creatures[i / 4]->getY() + ceil(REGION_SIZE / 2.0) - getY();
			dataCreature[i + 3 + 1] = m_creatures[i / 4]->m_item;
		}
		else {
			dataCreature[i + 0 + 1] = 0;
			dataCreature[i + 1 + 1] = 0;
			dataCreature[i + 2 + 1] = 0;
			dataCreature[i + 3 + 1] = 0;
		}
	}


	dataCreature[0] = amountOfCreatures;
	
	tools::BinaryIO::write<unsigned int>(getSaveLocation(), data, sizeof(data) / sizeof(unsigned char));
	tools::BinaryIO::write<float>(getSaveLocation() + " c", dataCreature, amountOfCreatures * 4 + 1);
}

void Region::update() {
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++)
		{
			m_tiles[x][y].update();
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
					removeCreature(i, false);
				}
				else {
					tmp = new Region(Game::posToRegionPos(m_creatures[i]->getX()) + floor(RENDER_DST / 2.0), Game::posToRegionPos(m_creatures[i]->getY()) + floor(RENDER_DST / 2.0));
					tmp->addCreature(m_creatures[i]);
					removeCreature(i, false);
					tmp->saveTiles();
					delete tmp;
				}
			}
		}
	}
}

void Region::submitTilesToRenderer(graphics::Renderer *renderer, float offx, float offy) {
	float rx = (m_x - RENDER_DST / 2.0) * (float)TILE_SIZE * REGION_SIZE + offx * TILE_SIZE;
	float ry = (m_y - RENDER_DST / 2.0) * (float)TILE_SIZE * REGION_SIZE + offy * TILE_SIZE;
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++)
		{
			int id = m_renderIdArray[x][y];
			float renderx = x * TILE_SIZE + rx, rendery = y * TILE_SIZE + ry;
			renderer->renderBox(renderx, rendery, TILE_SIZE, TILE_SIZE, 0, id, glm::vec4(1.0, 1.0, 1.0, 1.0));
		}
	}
}

void Region::submitObjectsToRenderer(graphics::Renderer *renderer, float offx, float offy) {
	float rx = (m_x - RENDER_DST / 2.0) * (float)TILE_SIZE * REGION_SIZE + offx * TILE_SIZE;
	float ry = (m_y - RENDER_DST / 2.0) * (float)TILE_SIZE * REGION_SIZE + offy * TILE_SIZE;
	for (int x = 0; x < REGION_SIZE; x++)
	{
		for (int y = 0; y < REGION_SIZE; y++) {
			int objid = m_renderIdObjectArray[x][y];
			float renderx = x * TILE_SIZE + rx, rendery = y * TILE_SIZE + ry;
			if (objid != 0) renderer->renderBox(renderx, rendery, TILE_SIZE, TILE_SIZE, 0, objid, glm::vec4(1.0, 1.0, 1.0, 1.0));
		}
	}

	if (!Game::debugMode) return;
	renderer->renderBox(rx, ry, TILE_SIZE / 10.0, REGION_SIZE * TILE_SIZE, 0, 20, glm::vec4(1.0, 1.0, 1.0, 1.0));
	renderer->renderBox(rx + REGION_SIZE * TILE_SIZE - TILE_SIZE / 10.0, ry, TILE_SIZE / 10.0, REGION_SIZE * TILE_SIZE, 0, 21, glm::vec4(1.0, 1.0, 1.0, 1.0));
	renderer->renderBox(rx, ry, REGION_SIZE * TILE_SIZE, TILE_SIZE / 10.0, 0, 22, glm::vec4(1.0, 1.0, 1.0, 1.0));
	renderer->renderBox(rx, ry + REGION_SIZE * TILE_SIZE - TILE_SIZE / 10.0, REGION_SIZE * TILE_SIZE, TILE_SIZE / 10.0, 0, 23, glm::vec4(1.0, 1.0, 1.0, 1.0));
}

void Region::submitCreaturesToRenderer(graphics::Renderer *renderer, float offx, float offy) {
	for (int i = 0; i < MAX_CREATURES; i++)
	{
		if (m_creatures[i]) m_creatures[i]->submitToRenderer(renderer, offx, offy);
	}
}

void Region::debugCeilCreatures() {
	for (int i = 0; i < MAX_CREATURES; i++)
	{
		if (m_creatures[i]) {
			m_creatures[i]->setX(round(m_creatures[i]->getX()));
			m_creatures[i]->setY(round(m_creatures[i]->getY()));

			Tile* tmp;
			tmp = Game::getTile(m_creatures[i]->getX() + 1, m_creatures[i]->getY(), "from region to ceil creature");
			if (tmp) tmp->setObjectId(4);
			tmp = Game::getTile(m_creatures[i]->getX() - 1, m_creatures[i]->getY(), "from region to ceil creature");
			if (tmp) tmp->setObjectId(4);
			tmp = Game::getTile(m_creatures[i]->getX(), m_creatures[i]->getY() + 1, "from region to ceil creature");
			if (tmp) tmp->setObjectId(4);
			tmp = Game::getTile(m_creatures[i]->getX(), m_creatures[i]->getY() - 1, "from region to ceil creature");
			if (tmp) tmp->setObjectId(4);

			return;
		}
	}
}

Tile* Region::getTile(unsigned int x, unsigned int y) {
	if (x > REGION_SIZE || x < 0 || y > REGION_SIZE || y < 0) {
		tools::Logger::warning(std::string("Coordinates were out of range when getting tile form region!"));
		return nullptr;
	}
	else {
		return &m_tiles[x][y];
	}
}

void Region::addCreature(float x, float y, int id, bool item) {
	for (int i = 0; i < MAX_CREATURES; i++)
	{
		if (!m_creatures[i]) {
			m_creatures[i] = new Creature(x, y, id, item);
			m_creatures[i]->setRegionIndex(i);
			return;
		}
	}
}

void Region::addCreature(Creature *creature) {
	for (int i = 0; i < MAX_CREATURES; i++)
	{
		if (!m_creatures[i]) {
			m_creatures[i] = creature;
			m_creatures[i]->setRegionIndex(i);
			return;
		}
	}
}

void Region::removeCreature(int i, bool _delete) {
	if (!m_creatures[i]) return;

	if (_delete) delete m_creatures[i];
	m_creatures[i] = nullptr;
}

void Region::removeCreature(Creature *creature, bool _delete) {
	for (int i = 0; i < MAX_CREATURES; i++)
	{
		if (m_creatures[i] == creature) {
			m_creatures[i] = nullptr;
			return;
		}
	}
	char buff[100];
	snprintf(buff, sizeof(buff), "%p", (void*)creature);
	std::string buffAsStdStr = buff;
	tools::Logger::critical("Couldn't find creature: " + buffAsStdStr + "!");
}