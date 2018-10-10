#pragma once

#include <string>
#include <vector>

#define ITEM_LOG 0
#define ITEM_STONE 1
#define ITEM_PLANT_FIBER 2

#define TILE_SOIL 0
#define TILE_wATER 1
#define TILE_SAND 2
#define TILE_GRASSY_SOIL 3

#define OBJECT_S


#define COUNT_ITEMS 3
#define COUNT_TILES 4
#define COUNT_OBJECTS 3

class Database {
public:
	static struct Object {};

	static struct Item
	{
		std::string name = "null";
		unsigned int id = 0;
		unsigned int texture = 0;

		Object *placedAs;
	} items[COUNT_ITEMS];

	static struct Tile
	{
		std::string name = "null";
		unsigned int id = 0;
		unsigned int texture = 0;
	} tiles[COUNT_TILES];

	static struct Object
	{
		std::string name = "null";
		unsigned int id = 0;
		unsigned int texture = 0;

		float health = 0;
		Item *dropsAs;
	} objects[COUNT_OBJECTS];

	static void init() {
		
		//Items
		items[0] = { "Log", 0, 19 };
		items[1] = { "Stone", 1, 20 };
		items[2] = { "Plant fiber", 2, 21 };

		//Tiles
		tiles[0] = { "Soil", 0, 0 };
		tiles[1] = { "Water", 1, 1 };
		tiles[2] = { "Sand", 2, 2 };
		tiles[3] = { "Grassy soil", 3, 4 };

		//Objects
		objects[0] = { "Grass", 0, 0 };
		objects[1] = { "Spruce", 1, 1 };
		objects[2] = { "Birch", 2, 2 };
		objects[2] = { "Mountain", 2, 2 };
	}

};