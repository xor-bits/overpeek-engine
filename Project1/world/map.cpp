#include "../pch.h"

#include "map.h"

#include "../logic/game.h"
#include "../creatures/player.h"


Map::Map() { 
	m_tiles = std::vector<std::vector<MapTile*>>(MAP_SIZE, std::vector<MapTile*>(MAP_SIZE)); 
}

std::string Map::saveLocation(std::string path) {
	return path + m_name + "\\";
}

bool Map::load(std::string name) {
	oe::Logger::out(oe::info, "Loading world...  ");
	m_name = name;

	//Load and check tile data
	unsigned long tile_data_size;
	Byte *tile_data = (Byte*)oe::BinaryIO::read<Byte>(saveLocation(Game::getSaveLocation()) + "tile.data", tile_data_size);
	if (!tile_data) {
		free(tile_data);
		return false;
	}


	//Uncompress tiledata
	unsigned long uncompressedSize = MAP_SIZE * MAP_SIZE * 3 * sizeof(short);
	Byte* uncompressedTiles = new Byte[uncompressedSize];
	int state = uncompress(uncompressedTiles, &uncompressedSize, tile_data, tile_data_size);
	if (state != Z_OK) {
		oe::Logger::out(oe::error, "Error uncompressing save file! ", state);
	}
	short int *tileData = (short int*)uncompressedTiles;

	//Load tiles
	unload();
#pragma omp parallel for
	for (int x = 0; x < MAP_SIZE; x++)
	{
		for (int y = 0; y < MAP_SIZE; y++)
		{
			m_tiles[x][y] = new MapTile(
				tileData[(x + y * MAP_SIZE) * 3 + 0],
				tileData[(x + y * MAP_SIZE) * 3 + 1],
				tileData[(x + y * MAP_SIZE) * 3 + 2]
			);
		}
	}


	//Load creatures
	unsigned long creature_data_size;
	CreatureSaveData *creature_data = (CreatureSaveData*)oe::BinaryIO::read<CreatureSaveData>(saveLocation(Game::getSaveLocation()) + "creature.data", creature_data_size);
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

	return true;
}

bool Map::unload() {
	try
	{
		for (int x = 0; x < MAP_SIZE; x++)
		{
			for (int y = 0; y < MAP_SIZE; y++)
			{
				delete m_tiles[x][y];
			}
		}
		for (int i = 0; i < m_creatures.size(); i++) {
			m_creatures[i].release();
		}
		m_creatures.erase(m_creatures.begin(), m_creatures.end());
	}
	catch (const std::exception&)
	{
		oe::Logger::out(oe::warning, "Couln't unload map");
	}
}

bool Map::save() {
	oe::Logger::out(oe::info, "Saving map...  ");

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

	//Compress tile data
	unsigned long sourceLen = (MAP_SIZE * MAP_SIZE * 3 * sizeof(short));
	unsigned long compressedSize = compressBound(sourceLen);
	Byte* compressedTiles = new Byte[compressedSize];
	Byte* tiledata = (Byte*)(tiles);
	int state = compress(compressedTiles, &compressedSize, tiledata, sourceLen);
	if (state != Z_OK) oe::Logger::out(oe::error, "Error compressing save file!");

	//Writing
	oe::BinaryIO::write<Byte>(saveLocation(Game::getSaveLocation()) + "tile.data", compressedTiles, compressedSize);
	oe::BinaryIO::write<CreatureSaveData>(saveLocation(Game::getSaveLocation()) + "creature.data", creatures, m_creatures.size());

	//Cleanup
	delete tiles;
}

bool Map::create(std::string name) {
	oe::Logger::out(oe::info, "Generating map...  ");
	m_name = name;

	//Get noisemaps
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

	
	//Create world based on noisemaps
#pragma omp parallel for
	for (int x = 0; x < MAP_SIZE; x++)
	{
		for (int y = 0; y < MAP_SIZE; y++)
		{
			int tile, object;
			getInfoFromNoise(tile, object, x, y);
			short health = Database::objects[object].health;
			m_tiles[x][y] = new MapTile(tile, object, health);
		}
	}

	//Cleanup
	noise->FreeNoiseSet(m_biomenoise1);
	noise->FreeNoiseSet(m_biomenoise2);
	noise->FreeNoiseSet(m_mapnoise);
	noise->FreeNoiseSet(m_plantnoise1);
	noise->FreeNoiseSet(m_plantnoise2);

	//Print status
	std::cout << "SUCCESS" << std::endl;
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
	if (!biome) oe::Logger::out(oe::error, "Biome was null pointer");

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

void Map::submitToRenderer(oe::Renderer *renderer, float offX, float offY, float corrector) {
	//Map rendering
	glm::fvec2 player_prediction = glm::fvec2(Game::getPlayer()->getX() + Game::getPlayer()->getVelX() * corrector / UPDATES_PER_SECOND, Game::getPlayer()->getY() + Game::getPlayer()->getVelY() * corrector / UPDATES_PER_SECOND);
	for (int x = -RENDER_HORIZONTAL; x < RENDER_HORIZONTAL; x++)
	{
		for (int y = -RENDER_VERTICAL; y < RENDER_VERTICAL; y++)
		{
			int tile_x = x + player_prediction.x;
			int tile_y = y + player_prediction.y;

			MapTile *tile = getTile(tile_x, tile_y);
			Database::Tile db_tile = Database::tiles[tile->m_tile];
			Database::Object db_object = Database::objects[tile->m_object];
			float rx = (tile_x + offX) * TILE_SIZE * Game::renderScale();
			float ry = (tile_y + offY) * TILE_SIZE * Game::renderScale();
			//oe::Logger::out(oe::info, "Tile at: ", offX, offY);
			//oe::Logger::out(oe::info, "Tile: ", tile_x, tile_y);
			renderer->renderPoint(glm::vec2(rx, ry), glm::vec2(TILE_SIZE, TILE_SIZE) * Game::renderScale(), db_tile.texture, glm::vec4(1.0));

			if (db_object.id != 0) {
				int objTexture = getObjectTexture(tile_x, tile_y);
				renderer->renderPoint(glm::vec2(rx, ry), glm::vec2(TILE_SIZE, TILE_SIZE) * Game::renderScale(), objTexture, glm::vec4(db_object.color, 1.0));
			}
		
		}
	}

	//Creature rendering
	for (int i = 0; i < m_creatures.size(); i++)
	{
		m_creatures[i]->submitToRenderer(renderer, -Game::getPlayer()->getX() - Game::getPlayer()->getVelX() * corrector / UPDATES_PER_SECOND, -Game::getPlayer()->getY() - Game::getPlayer()->getVelY() * corrector / UPDATES_PER_SECOND, corrector);
	}
}

void Map::renderGhostObject(oe::Renderer *renderer, float x, float y, int id, float offX, float offY, float corrector) {
	x = floor(x);
	y = floor(y);
	//x += 0.5;
	//y += 0.5;


	Database::Object db_object = Database::objects[id];
	float rx = (x + offX) * TILE_SIZE * Game::renderScale();
	float ry = (y + offY) * TILE_SIZE * Game::renderScale();

	//if (rx > 0) rx += 1 * TILE_SIZE * Game::renderScale();
	//if (ry > 0) ry += 1 * TILE_SIZE * Game::renderScale();
	if (db_object.id != 0)
		renderer->renderPoint(glm::vec2(rx, ry), glm::vec2(TILE_SIZE, TILE_SIZE) * Game::renderScale(), db_object.texture, glm::vec4(db_object.color, 0.5));
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

void Map::update(float divider) {
	for (int i = 0; i < m_creatures.size(); i++)
	{
		m_creatures[i]->update(i, divider);
	}

	for (int x = -MAP_WORK_DST; x < MAP_WORK_DST; x++)
	{
		for (int y = -MAP_WORK_DST; y < MAP_WORK_DST; y++)
		{
			if (!oe::isInRange(Game::getPlayer()->getX() + x, 0, MAP_SIZE - 1)) continue;
			if (!oe::isInRange(Game::getPlayer()->getY() + y, 0, MAP_SIZE - 1)) continue;

			//Spawners
			if (getTile(Game::getPlayer()->getX() + x, Game::getPlayer()->getY() + y)->m_object == 6) {
				float chance = oe::Random::random(0.0f, 100.0f);
				if (Game::advancedDebugMode) chance *= 2;
				if (chance > 99.5f) {
					std::vector<Creature*> arr;
					findAllCreatures(Game::getPlayer()->getX() + x, Game::getPlayer()->getY() + y, arr, MAP_WORK_DST);

					if (arr.size() > 32) continue;

					float _x = Game::getPlayer()->getX() + x + oe::Random::random(-2.0f, 2.0f);
					float _y = Game::getPlayer()->getY() + y + oe::Random::random(-2.0f, 2.0f);
					addCreature(_x, _y, 1, false);
				}
			}
		}
	}
}

Map::MapTile *Map::getTile(unsigned int x, unsigned int y) {
	x = oe::clamp(x, unsigned int(0), unsigned int(MAP_SIZE - 1));
	y = oe::clamp(y, unsigned int(0), unsigned int(MAP_SIZE - 1));

	return m_tiles[x][y];
}

int Map::trySetObject(unsigned int x, unsigned int y, short id) {
	MapTile* tile = getTile(x, y);
	if (tile->m_object != 0) return -1;
	//if (tile->m_tile == 1) return -2;

	tile->m_object = id;
	tile->m_objectHealth = Database::objects[id].health;
	return 0;
}

int Map::trySetObject(MapTile *tile, short id) {
	if (tile->m_object != 0) return -1;
	//if (tile->m_tile == 1) return -2;

	tile->m_object = id;
	tile->m_objectHealth = Database::objects[id].health;
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