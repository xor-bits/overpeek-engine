#include "../pch.h"

#include "map.h"

#include "../logic/game.h"
#include "../creatures/player.h"


Map::Map(std::string name) {
	m_name = name;
	oe::Logger::out(oe::info, "World loading started");
	oe::Debug::startTimer();

	std::string strcommand("mkdir \"" + saveLocation() + "\"" + " >nul 2>&1");
	system(strcommand.c_str());

#if !DEBUG_DISABLE_SAVING
	//Generate map
	if (!loadMap()) {
		createMapFast();
	}
#else
	createMapFast();
#endif
	//save();
	oe::Debug::printTimer("Microseconds passed while creating world: ");
}

bool Map::loadMap() {
	oe::Logger::out(oe::info, "Starting to load the map");
	oe::Debug::startTimer();


	//Load and check tile data
	unsigned long tile_data_size;
	Byte *tile_data = (Byte*)oe::BinaryIO::read<Byte>(saveLocation() + "tile.data", tile_data_size);
	if (!tile_data) {
		free(tile_data);
		return false;
	}

	//Print time took to check the data
	oe::Debug::printTimer("Microseconds took to check data: ");
	oe::Debug::startTimer();

	//Uncompress tiledata
	unsigned long uncompressedSize = MAP_SIZE * MAP_SIZE * 3 * sizeof(short);
	Byte* uncompressedTiles = new Byte[uncompressedSize];
	int state = uncompress(uncompressedTiles, &uncompressedSize, tile_data, tile_data_size);
	if (state != Z_OK) oe::Logger::out(oe::error, "Error uncompressing save file! ", state);
	short int *tileData = (short int*)uncompressedTiles;

	//Print time took to uncompress the data
	oe::Debug::printTimer("Microseconds took to uncompress data: ");
	oe::Debug::startTimer();
	
	//Load tiles
#pragma omp parallel for
	for (int x = 0; x < MAP_SIZE; x++)
	{
		std::vector<std::unique_ptr<MapTile>> tmp;
		for (int y = 0; y < MAP_SIZE; y++)
		{
			tmp.push_back(std::move(std::unique_ptr<MapTile>(new MapTile(
				tileData[(x + y * MAP_SIZE) * 3 + 0], 
				tileData[(x + y * MAP_SIZE) * 3 + 1],
				tileData[(x + y * MAP_SIZE) * 3 + 2]
			))));
		}
		m_tiles.push_back(std::move(tmp));
	}

	//Print time took to load tiles
	oe::Debug::printTimer("Microseconds took to uncompress data: ");
	oe::Debug::startTimer();


	//Load creatures
	unsigned long creature_data_size;
	CreatureSaveData *creature_data = (CreatureSaveData*)oe::BinaryIO::read<CreatureSaveData>(saveLocation() + "creature.data", creature_data_size);
	if (creature_data) {
		creature_data_size /= sizeof(CreatureSaveData);
		for (int i = 0; i < creature_data_size; i++)
		{
			addCreature(
				((CreatureSaveData*)creature_data)[i].x,
				((CreatureSaveData*)creature_data)[i].y,
				((CreatureSaveData*)creature_data)[i].id,
				((CreatureSaveData*)creature_data)[i].item
			);
		}
	}

	//Cleanup
	free(tileData);
	free(creature_data);


	oe::Logger::out(oe::info, "Map loaded successfully!");
}

void Map::save() {
	oe::Logger::out(oe::info, "Starting to save the map");
	oe::Debug::startTimer();


	//Tile collection
	short *tiles = new short[MAP_SIZE * MAP_SIZE * 3];
	for (int x = 0; x < MAP_SIZE; x++)
	{
		for (int y = 0; y < MAP_SIZE; y++)
		{
			tiles[(x + y * MAP_SIZE) * 3 + 0] = m_tiles[x][y]->m_tile;
			tiles[(x + y * MAP_SIZE) * 3 + 1] = m_tiles[x][y]->m_object;
			tiles[(x + y * MAP_SIZE) * 3 + 2] = m_tiles[x][y]->m_objectHealth;
		}
	}
	//Creature collection
	CreatureSaveData *creatures = new CreatureSaveData[m_creatures.size()];
	for (int i = 0; i < m_creatures.size(); i++) {
		creatures[i] = { m_creatures[i]->getX(), m_creatures[i]->getY(), (short)m_creatures[i]->getId(), m_creatures[i]->m_item };
	}


	//Print data collection time
	oe::Debug::printTimer("Microseconds to collect the data: ");
	oe::Debug::startTimer();


	//Compress tile data
	unsigned long sourceLen = (MAP_SIZE * MAP_SIZE * 3 * sizeof(short));
	unsigned long compressedSize = compressBound(sourceLen);
	Byte* compressedTiles = new Byte[compressedSize];
	Byte* tiledata = (Byte*)(tiles);
	int state = compress(compressedTiles, &compressedSize, tiledata, sourceLen);
	if (state != Z_OK) oe::Logger::out(oe::error, "Error compressing save file!");


	//Print compression time
	oe::Debug::printTimer("Microseconds to compress the data: ");
	oe::Debug::startTimer();

	oe::BinaryIO::write<Byte>(saveLocation() + "tile.data", compressedTiles, compressedSize);
	oe::BinaryIO::write<CreatureSaveData>(saveLocation() + "creature.data", creatures, m_creatures.size());

	//Print saving time
	oe::Debug::printTimer("Microseconds to save the data: ");
	oe::Logger::out(oe::info, "Map saved successfully!");

	//Cleanup
	delete tiles;
}

void Map::createMapFast() {
	FastNoiseSIMD* noise = FastNoiseSIMD::NewFastNoiseSIMD(oe::Clock::getMicroseconds() + 0);
	noise->SetFractalOctaves(MAP_BIOME_OCTA);
	noise->SetFrequency(MAP_BIOME_FREQ);
	m_biomenoise1 = noise->GetSimplexFractalSet(0, 0, 0, MAP_SIZE, MAP_SIZE, 1, 1.0f);

	noise = FastNoiseSIMD::NewFastNoiseSIMD(oe::Clock::getMicroseconds() + 1);
	noise->SetFractalOctaves(MAP_BIOME_OCTA);
	noise->SetFrequency(MAP_BIOME_FREQ);
	m_biomenoise2 = noise->GetSimplexFractalSet(0, 0, 0, MAP_SIZE, MAP_SIZE, 1, 1.0f);
	
	noise = FastNoiseSIMD::NewFastNoiseSIMD(oe::Clock::getMicroseconds() + 2);
	noise->SetFractalOctaves(MAP_OCTA);
	noise->SetFrequency(MAP_FREQ);
	m_mapnoise =	noise->GetSimplexFractalSet(0, 0, 0, MAP_SIZE, MAP_SIZE, 1, 1.0f);
	
	noise = FastNoiseSIMD::NewFastNoiseSIMD(oe::Clock::getMicroseconds() + 3);
	noise->SetFractalOctaves(MAP_PLANT1_OCTA);
	noise->SetFrequency(MAP_PLANT1_FREQ);
	m_plantnoise1 = noise->GetSimplexFractalSet(0, 0, 0, MAP_SIZE, MAP_SIZE, 1, 1.0f);
	
	noise = FastNoiseSIMD::NewFastNoiseSIMD(oe::Clock::getMicroseconds() + 4);
	noise->SetFractalOctaves(MAP_PLANT2_OCTA);
	noise->SetFrequency(MAP_PLANT2_FREQ);
	m_plantnoise2 = noise->GetSimplexFractalSet(0, 0, 0, MAP_SIZE, MAP_SIZE, 1, 1.0f);

	oe::Logger::out(oe::info, "Noisemaps generated");

#pragma omp parallel for
	for (int x = 0; x < MAP_SIZE; x++)
	{
		std::vector<std::unique_ptr<MapTile>> tmp;
		for (int y = 0; y < MAP_SIZE; y++)
		{
			int tile, object;
			getInfoFromNoise(tile, object, x, y);
			short health = Database::objects[object].health;
			tmp.push_back(std::move(std::unique_ptr<MapTile>(new MapTile(tile, object, health))));
		}
		m_tiles.push_back(std::move(tmp));
	}
	oe::Logger::out(oe::info, "Map generated");

	noise->FreeNoiseSet(m_biomenoise1);
	noise->FreeNoiseSet(m_biomenoise2);
	noise->FreeNoiseSet(m_mapnoise);
	noise->FreeNoiseSet(m_plantnoise1);
	noise->FreeNoiseSet(m_plantnoise2);
	oe::Logger::out(oe::info, "Noisemap memory freed");
}

Database::Biome *Map::getTileBiome(float x, float y) {
	return Database::getBiome(
		(m_biomenoise1[int(x + y * MAP_SIZE)] + 1.0f) / 2.0f, 
		(m_biomenoise2[int(x + y * MAP_SIZE)] + 1.0f) / 2.0f
	);
}

int Map::getInfoFromNoiseIfLoop(Database::Biome *biome, float x, float y, int index) {
	if (biome->heightMap[index].grassId != 0) {
		if ((m_plantnoise1[int(x + y * MAP_SIZE)] + 1.0f) / 2.0f > biome->heightMap[index].grassRarity) return biome->heightMap[index].grassId;
	}
	if (biome->heightMap[index].plantId != 0) {
		if ((m_plantnoise2[int(x + y * MAP_SIZE)] + 1.0f) / 2.0f > biome->heightMap[index].plantRarity) return biome->heightMap[index].plantId;
	}
	return 0;
}

void Map::getInfoFromNoise(int &tileId, int &objId, float x, float y) {
	Database::Biome *biome = getTileBiome(x, y);
	if (!biome) oe::Logger::out(oe::error, "Biome was nullptr");

	if (biome->heightMap.size() == 1) {
		tileId = biome->heightMap[0].id;
		objId = getInfoFromNoiseIfLoop(biome, x, y, 0);
	}
	else {
		float height1 = (m_mapnoise[int(x + y * MAP_SIZE)] + 1.0f) / 2.0f;
		for (int i = 0; i < biome->heightMap.size(); i++)
		{
			if (height1 <= biome->heightMap[i].height) {
				tileId = biome->heightMap[i].id;
				objId = getInfoFromNoiseIfLoop(biome, x, y, i);
				break;
			}
		}
	}
}

void Map::hit(unsigned int x, unsigned int y, short dmg) {
	MapTile *tile = getTile(x, y);
	tile->m_objectHealth -= dmg;
	if (tile->m_objectHealth <= 0) {
		addCreature(x + 0.5f, y + 0.5f, Database::objects[tile->m_object].dropsAs, true);
		tile->m_object = 0;
	}
	Game::tilesChanged = true;
}

std::string Map::saveLocation() {
	return Game::getSaveLocation() + m_name + "\\";
}

int Map::getObjectTexture(unsigned int x, unsigned int y) {
	MapTile *thistile = getTile(x, y);
	if (!thistile) return 0;

	if (Database::objects[thistile->m_object].multitexture) {
		MapTile* right = getTile(x + 1, y);
		MapTile* top = getTile(x, y - 1);
		MapTile* left = getTile(x - 1, y);
		MapTile* bottom = getTile(x, y + 1);

		MapTile* topright =	getTile(x + 1, y - 1);
		MapTile* topleft = getTile(x - 1, y - 1);
		MapTile* bottomleft = getTile(x - 1, y + 1);
		MapTile* bottomright = getTile(x + 1, y + 1);

		bool rightAir = right && !Database::objects[right->m_object].wall;
		bool topAir = top && !Database::objects[top->m_object].wall;
		bool leftAir = left && !Database::objects[left->m_object].wall;
		bool bottomAir = bottom && !Database::objects[bottom->m_object].wall;

		bool topRightAir = topright && !Database::objects[topright->m_object].wall;
		bool topLeftAir = topleft && !Database::objects[topleft->m_object].wall;
		bool bottomLeftAir = bottomleft && !Database::objects[bottomleft->m_object].wall;
		bool bottomRightAir = bottomright && !Database::objects[bottomright->m_object].wall;

		return Database::objects[thistile->m_object].getTexture(rightAir, topAir, leftAir, bottomAir, topRightAir, topLeftAir, bottomLeftAir, bottomRightAir);
	}
	return Database::objects[thistile->m_object].texture;
}

void Map::submitToRenderer(oe::Renderer *renderer, float offX, float offY) {
	//Map rendering
	for (int x = -RENDER_HORIZONTAL; x < RENDER_HORIZONTAL; x++)
	{
		for (int y = -RENDER_VERTICAL; y < RENDER_VERTICAL; y++)
		{
			MapTile *tile = getTile(x + Game::getPlayer()->getX(), y + Game::getPlayer()->getY());
			Database::Tile db_tile = Database::tiles[tile->m_tile];
			Database::Object db_object = Database::objects[tile->m_object];
			float rx = (float(x) + floor(Game::getPlayer()->getX()) + offX) * TILE_SIZE * Game::renderScale();
			float ry = (float(y) + floor(Game::getPlayer()->getY()) + offY) * TILE_SIZE * Game::renderScale();
			renderer->renderPoint(glm::vec2(rx, ry), glm::vec2(TILE_SIZE * Game::renderScale(), TILE_SIZE * Game::renderScale()), db_tile.texture, glm::vec4(1.0));

			if (db_object.id != 0)
			renderer->renderPoint(glm::vec2(rx, ry), glm::vec2(TILE_SIZE * Game::renderScale(), TILE_SIZE * Game::renderScale()), getObjectTexture(x + Game::getPlayer()->getX(), y + Game::getPlayer()->getY()), glm::vec4(db_object.color, 1.0));
		
		}
	}

	//Creature rendering
	for (int i = 0; i < m_creatures.size(); i++)
	{
		m_creatures[i]->submitToRenderer(renderer, -Game::getPlayer()->getX(), -Game::getPlayer()->getY());
	}
}

void Map::debugCeilCreatures() {
	for (int i = 0; i < m_creatures.size(); i++)
	{
		if (m_creatures[i]) {
			m_creatures[i]->setX(round(m_creatures[i]->getX()));
			m_creatures[i]->setY(round(m_creatures[i]->getY()));

			MapTile* tmp;
			tmp = getTile(m_creatures[i]->getX() + 1, m_creatures[i]->getY());
			if (tmp) tmp->m_object = 4;
			tmp = getTile(m_creatures[i]->getX() - 1, m_creatures[i]->getY());
			if (tmp) tmp->m_object = 4;
			tmp = getTile(m_creatures[i]->getX(), m_creatures[i]->getY() + 1);
			if (tmp) tmp->m_object = 4;
			tmp = getTile(m_creatures[i]->getX(), m_creatures[i]->getY() - 1);
			if (tmp) tmp->m_object = 4;

			return;
		}
	}
}

Creature *Map::addCreature(float x, float y, int id, bool item) {
	std::unique_ptr<Creature> tmp(new Creature(x, y, id, item));
	m_creatures.push_back(std::move(tmp));
	return m_creatures[m_creatures.size() - 1].get();
}

Creature *Map::itemDrop(float x, float y, int id) {
	Creature *newItem = addCreature(x, y, id, true);
	newItem->vel_x = oe::Random::random(-0.2, 0.2);
	newItem->vel_y = oe::Random::random(-0.2, 0.2);
	return newItem;
}

void Map::removeCreature(int i) {
	if (!m_creatures[i]) return;
	m_creatures[i].reset();
	m_creatures.erase(m_creatures.begin() + i);
}

void Map::removeCreature(Creature *creature) {
	for (int i = 0; i < m_creatures.size(); i++)
	{
		if (m_creatures[i].get() == creature) {
			removeCreature(i);
			return;
		}
	}
	char buff[100];
	snprintf(buff, sizeof(buff), "%p", (void*)creature);
	std::string buffAsStdStr = buff;
	oe::Logger::out(oe::critical, "Couldn't find creature: ", buffAsStdStr.c_str(), "!");
}

void Map::update() {
	for (int i = 0; i < m_creatures.size(); i++)
	{
		m_creatures[i]->update(i);
	}
}

Map::MapTile *Map::getTile(unsigned int x, unsigned int y) {
	x = oe::clamp(x, unsigned int(0), unsigned int(MAP_SIZE - 1));
	y = oe::clamp(y, unsigned int(0), unsigned int(MAP_SIZE - 1));

	return m_tiles[x][y].get();
}

int Map::trySetObject(unsigned int x, unsigned int y, short id) {
	MapTile* tile = getTile(x, y);
	if (tile->m_object != 0) return -1;
	if (tile->m_tile == 1) return -2;

	tile->m_object = id;
	return 0;
}

int Map::trySetObject(MapTile *tile, short id) {
	if (tile->m_object != 0) return -1;
	if (tile->m_tile == 1) return -2;

	tile->m_object = id;
	return 0;
}

void Map::findAllCreatures(float _x, float _y, std::vector<Creature*> &_array, float _radius) {
	_array.push_back(Game::getPlayer());
	for (int i = 0; i < m_creatures.size(); i++)
	{
		if (!oe::isInRange(m_creatures[i]->getX(), _x - _radius, _x + _radius)) continue;
		if (!oe::isInRange(m_creatures[i]->getY(), _y - _radius, _y + _radius)) continue;
		_array.push_back(m_creatures[i].get());
	}
}