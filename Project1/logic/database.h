#pragma once

#include <string>
#include <glm/glm.hpp>
#include <vector>

#define COUNT_ITEMS		4
#define COUNT_TILES		5
#define COUNT_OBJECTS	6
#define COUNT_CREATURES 3
#define COUNT_BIOMES	6

#define MAP_FREQ		0.05f
#define MAP_BIOME_FREQ	MAP_FREQ / 4.0f
#define MAP_PLANT1_FREQ	0.1f
#define MAP_PLANT2_FREQ	0.1f

#define MAP_OCTA		3
#define MAP_BIOME_OCTA	4
#define MAP_PLANT1_OCTA	2
#define MAP_PLANT2_OCTA	2

class Database {
public:

	static struct Item
	{
		std::string name = "null";
		unsigned int id = 0;
		unsigned int texture = 0;

		unsigned int stack_size = 0;

		unsigned int placedAs;

		Item() {}
		Item(std::string _name, unsigned int _id, unsigned int _texture, unsigned int _stack, unsigned int _placedAs) {
			name = _name;
			id = _id;
			texture = _texture;
			stack_size = _stack;
			placedAs = _placedAs;
		}
	} items[COUNT_ITEMS];

	static struct Tile
	{
		std::string name = "null";
		unsigned int id = 0;
		unsigned int texture = 0;

		Tile() {}
		Tile(std::string _name, unsigned int _id, unsigned int _texture) {
			name = _name;
			id = _id;
			texture = _texture;
		}
	} tiles[COUNT_TILES];

	static struct Object
	{
		std::string name = "null";
		unsigned int id = 0;
		unsigned int texture = 0;
		bool wall = 0;
		bool multitexture = 0;
		bool smooth = 0;
		bool destructable = 0;
		glm::vec3 color = glm::vec3(0.0f);

		short health = 0;
		unsigned int dropsAs = 0;

		Object() {}
		Object(std::string _name, unsigned int _id, unsigned int _texture, bool _wall, bool _multitexture, bool _smooth, bool _destructable, glm::vec3 _color, short _health, unsigned int _dropsAs) {

			name = _name;
			id = _id;
			texture = _texture;
			wall = _wall;
			multitexture = _multitexture;
			smooth = _smooth;
			destructable = _destructable;
			color = _color;

			health = _health;
			dropsAs = _dropsAs;
		}

		unsigned int getTexture(bool rightAir, bool topAir, bool leftAir, bool bottomAir, bool topRightAir, bool topLeftAir, bool bottomLeftAir, bool bottomRightAir) {
			if (!multitexture) return texture;
			
			unsigned int index = 160;
			if (!smooth) index += 48;

			bool allSidesAir = rightAir && topAir && leftAir && bottomAir;
			bool allCornersAir = topRightAir && topLeftAir && bottomLeftAir && bottomRightAir;
			bool noSideAir = !rightAir && !topAir && !leftAir && !bottomAir;
			bool noCornerAir = !topRightAir && !topLeftAir && !bottomLeftAir && !bottomRightAir;

			if (!rightAir && topAir && leftAir && !bottomAir && !bottomRightAir) {
				index += 0 + 0 * 16;
			}
			else if (!rightAir && topAir && !leftAir && !bottomAir && !bottomLeftAir && !bottomRightAir) {
				index += 1 + 0 * 16;
			}
			else if (rightAir && topAir && !leftAir && !bottomAir && !bottomLeftAir) {
				index += 2 + 0 * 16;
			}
			else if (!rightAir && !topAir && leftAir && !bottomAir && !topRightAir && !bottomRightAir) {
				index += 0 + 1 * 16;
			}
			else if (!rightAir && !topAir && !leftAir && !bottomAir && !topRightAir && !topLeftAir && !bottomLeftAir && !bottomRightAir) {
				index += 1 + 1 * 16;
			}
			else if (rightAir && !topAir && !leftAir && !bottomAir && !topLeftAir && !bottomLeftAir) {
				index += 2 + 1 * 16;
			}
			else if (!rightAir && !topAir && leftAir && bottomAir && !topRightAir) {
				index += 0 + 2 * 16;
			}
			else if (!rightAir && !topAir && !leftAir && bottomAir && !topRightAir && !topLeftAir) {
				index += 1 + 2 * 16;
			}
			else if (rightAir && !topAir && !leftAir && bottomAir && !topLeftAir) {
				index += 2 + 2 * 16;
			}

			else if (noSideAir && !topRightAir && !topLeftAir && !bottomLeftAir && bottomRightAir) {
				index += 3 + 0 * 16;
			}
			else if (noSideAir && !topRightAir && !topLeftAir && bottomLeftAir && bottomRightAir) {
				index += 4 + 0 * 16;
			}
			else if (noSideAir && !topRightAir && !topLeftAir && bottomLeftAir && !bottomRightAir) {
				index += 5 + 0 * 16;
			}
			else if (noSideAir && topRightAir && !topLeftAir && !bottomLeftAir && bottomRightAir) {
				index += 3 + 1 * 16;
			}
			else if (noSideAir && topRightAir && topLeftAir && bottomLeftAir && bottomRightAir) {
				index += 4 + 1 * 16;
			}
			else if (noSideAir && !topRightAir && topLeftAir && bottomLeftAir && !bottomRightAir) {
				index += 5 + 1 * 16;
			}
			else if (noSideAir && topRightAir && !topLeftAir && !bottomLeftAir && !bottomRightAir) {
				index += 3 + 2 * 16;
			}
			else if (noSideAir && topRightAir && topLeftAir && !bottomLeftAir && !bottomRightAir) {
				index += 4 + 2 * 16;
			}
			else if (noSideAir && !topRightAir && topLeftAir && !bottomLeftAir && !bottomRightAir) {
				index += 5 + 2 * 16;
			}

			else if (!rightAir && topAir && !leftAir && bottomAir) {
				index += 6 + 0 * 16;
			}
			else if (rightAir && topAir && leftAir && !bottomAir) {
				index += 7 + 0 * 16;
			}
			else if (rightAir && !topAir && leftAir && !bottomAir) {
				index += 6 + 1 * 16;
			}
			else if (rightAir && !topAir && leftAir && bottomAir) {
				index += 7 + 1 * 16;
			}
			else if (!rightAir && topAir && leftAir && bottomAir) {
				index += 6 + 2 * 16;
			}
			else if (rightAir && topAir && !leftAir && bottomAir) {
				index += 7 + 2 * 16;
			}

			else if (!rightAir && topAir && leftAir && !bottomAir && bottomRightAir) {
				index += 8 + 0 * 16;
			}
			else if (!rightAir && topAir && !leftAir && !bottomAir && bottomRightAir && bottomLeftAir) {
				index += 9 + 0 * 16;
			}
			else if (rightAir && topAir && !leftAir && !bottomAir && bottomLeftAir) {
				index += 10 + 0 * 16;
			}
			else if (!rightAir && !topAir && leftAir && !bottomAir && topRightAir && bottomRightAir) {
				index += 8 + 1 * 16;
			}
			else if (!rightAir && !topAir && !leftAir && !bottomAir && topRightAir && topLeftAir && bottomRightAir && bottomLeftAir) {
				index += 9 + 1 * 16;
			}
			else if (rightAir && !topAir && !leftAir && !bottomAir && topLeftAir && bottomLeftAir) {
				index += 10 + 1 * 16;
			}
			else if (!rightAir && !topAir && leftAir && bottomAir && topRightAir) {
				index += 8 + 2 * 16;
			}
			else if (!rightAir && !topAir && !leftAir && bottomAir && topRightAir && topLeftAir) {
				index += 9 + 2 * 16;
			}
			else if (rightAir && !topAir && !leftAir && bottomAir && topLeftAir) {
				index += 10 + 2 * 16;
			}

			else if (noSideAir && topRightAir && topLeftAir && bottomLeftAir && !bottomRightAir) {
				index += 11 + 0 * 16;
			}
			else if (noSideAir && topRightAir && topLeftAir && !bottomLeftAir && bottomRightAir) {
				index += 12 + 0 * 16;
			}
			else if (noSideAir && !topRightAir && topLeftAir && bottomLeftAir && bottomRightAir) {
				index += 13 + 0 * 16;
			}
			else if (noSideAir && topRightAir && !topLeftAir && bottomLeftAir && bottomRightAir) {
				index += 14 + 0 * 16;
			}

			else if (!rightAir && !topAir && leftAir && !bottomAir && !topRightAir && bottomRightAir) {
				index += 11 + 1 * 16;
			}
			else if (rightAir && !topAir && !leftAir && !bottomAir && !topLeftAir && bottomLeftAir) {
				index += 12 + 1 * 16;
			}
			else if (!rightAir && !topAir && leftAir && !bottomAir && topRightAir && !bottomRightAir) {
				index += 11 + 2 * 16;
			}
			else if (rightAir && !topAir && !leftAir && !bottomAir && topLeftAir && !bottomLeftAir) {
				index += 12 + 2 * 16;
			}

			else if (!rightAir && topAir && !leftAir && !bottomAir && !bottomLeftAir && bottomRightAir) {
				index += 13 + 1 * 16;
			}
			else if (!rightAir && topAir && !leftAir && !bottomAir && bottomLeftAir && !bottomRightAir) {
				index += 14 + 1 * 16;
			}
			else if (!rightAir && !topAir && !leftAir && bottomAir && !topLeftAir && topRightAir) {
				index += 13 + 2 * 16;
			}
			else if (!rightAir && !topAir && !leftAir && bottomAir && topLeftAir && !topRightAir) {
				index += 14 + 2 * 16;
			}

			else if (noSideAir && bottomLeftAir && topRightAir) {
				index += 15 + 0 * 16;
			}
			else if (noSideAir && bottomRightAir && topLeftAir) {
				index += 15 + 1 * 16;
			}

			else {
			index += 15 + 2 * 16;
			}

			return index;
		}
	} objects[COUNT_OBJECTS];

	static struct Creature
	{
		std::string name = "null";
		unsigned int id = 0;
		unsigned int texture_heading_up = 0;
		unsigned int texture_heading_down = 0;
		unsigned int texture_heading_left = 0;
		unsigned int texture_heading_right = 0;
		bool friendly = 0;
		bool ghost = false;
		glm::vec3 color = glm::vec3(0.0f);
		float knockback = 0.0f;
		float meleeDamage = 0.0f;

		float health = 0.0f;
		float healthgain = 0.0f;
		float stamina = 0.0f;
		float staminagain = 0.0f;
		unsigned int dropsAs;
	} creatures[COUNT_CREATURES];

	static struct BiomeTileHeight 
	{
		float height;
		int id;
		
		unsigned int grassId = 0;
		float grassRarity = 0.0f;

		unsigned int plantId = 0;
		float plantRarity = 0.0f;
	};

	static struct Biome 
	{
		std::string name = "null";
		unsigned int id = 0;
		float temperature = 0;	//0 - 1.
		float humidity = 0;		//0 - 1.
		float noiseScale = 0;
		std::vector<BiomeTileHeight> heightMap;
	} biomes[COUNT_BIOMES];



	static void init() {

		//Items			//Name			//Id	//Texture	//Stack size	//Placed as
		items[0] = Item("Empty",		0,		9,			99,				0);
		items[1] = Item("Stone",		1,		17,			99,				5);
		items[2] = Item("Plant fiber",	2,		18,			99,				0);
		items[3] = Item("Log",			3,		16,			99,				4);

		//Tiles			//Name			//Id	//Texture
		tiles[0] = Tile("Soil",			0,		0);
		tiles[1] = Tile("Water",		1,		1);
		tiles[2] = Tile("Sand",			2,		2);
		tiles[3] = Tile("Grassy soil",	3,		4);
		tiles[4] = Tile("Gravel",		4,		8);

		//Objects			//Name			//id	//Texture	//Wall	//Multitexture	//Smooth	//Destroyable	//Color							//Health	//Drop as		
		objects[0] = Object("Air",			0,		0,			false,	false,			false,		false,			glm::vec3(1.0f),				0,			0);
		objects[1] = Object("Grass",		1,		5,			false,	false,			false,		true,			glm::vec3(1.0f),				1,			2);
		objects[2] = Object("Spruce",		2,		6,			false,	false,			false,		true,			glm::vec3(1.0f),				5,			3);
		objects[3] = Object("Birch",		3,		7,			false,	false,			false,		true,			glm::vec3(1.0f),				5,			3);
		objects[4] = Object("Wood wall",	4,		0,			true,	true,			true,		true,			glm::vec3(0.58f, 0.43f, 0.2f),	10,			3);
		objects[5] = Object("Stone wall",	5,		0,			true,	true,			false,		true,			glm::vec3(0.5f, 0.5f, 0.5f),	15,			1);
		
		//Creatures
		creatures[0] = { "Player", 0, 3, 19, 35, 51, true, false, glm::vec3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f, 5.0f, 0.005f, 5.0f, 0.005f, 0 };
		creatures[1] = { "Enemy", 1, 3, 19, 35, 51, false, false, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f, 5.0f, 0.0f, 5.0f, 5.0f, 2 };

		//Biomes

		BiomeTileHeight height0;
		BiomeTileHeight height1;
		BiomeTileHeight height2;
		BiomeTileHeight height3;

		//Ocean
		height0 = { 0.8f, 1};
		height1 = { 0.95f, 2 };
		height2 = { 1.0f, 0 };
		biomes[0] = { "Ocean", 0, 0.5f, 0.5f, 0.02f, std::vector<BiomeTileHeight> { height0, height1, height2 } };

		//Beach
		height0 = { 1.0, 2 };
		biomes[1] = { "Beach", 0, 0.5f, 0.5f, 0.02f, std::vector<BiomeTileHeight> { height0 } };

		//Default
		height0 = { 0.05f, 1 };
		height1 = { 0.10f, 2 };
		height2 = { 0.80f, 0, 1, 0.5f };
		height3 = { 1.00f, 4 };
		biomes[2] = { "Default", 0, 0.5f, 0.5f, 0.02f, std::vector<BiomeTileHeight> { height0, height1, height2, height3 } };

		//Forest
		height0 = { 0.05f, 1 };
		height1 = { 0.10f, 2 };
		height2 = { 0.80f, 0, 1, 0.5f, 2, 0.5f };
		height3 = { 1.00f, 4 };
		biomes[3] = { "Forest", 0, 0.5f, 0.5f, 0.02f, std::vector<BiomeTileHeight> { height0, height1, height2, height3 } };

		//Forest 2
		height0 = { 0.05f, 1 };
		height1 = { 0.10f, 2 };
		height2 = { 0.80f, 0, 1, 0.5f, 3, 0.5f };
		height3 = { 1.09f, 4 };
		biomes[4] = { "Forest 2", 0, 0.5f, 0.5f, 0.02f, std::vector<BiomeTileHeight> { height0, height1, height2, height3 } };

		//Mountains
		height0 = { 1.0f, 4, 5, 0.1f };
		biomes[5] = { "Mountains", 0, 0.5f, 0.5f, 0.02f, std::vector<BiomeTileHeight> { height0 } };
	}

	static Biome *getBiome(float height, float height2) {
		if (height < 0.5f) return &biomes[0];
		if (height < 0.51f) return &biomes[1];
		else if (height < 0.75f) {
			if (height2 < 0.4f) return &biomes[2];
			else if (height2 < 0.7f) return &biomes[3];
			else if (height2 < 1.0f) return &biomes[4];
		}
		else if (height < 1.0f) return &biomes[5];
		
		return nullptr;
	}

};