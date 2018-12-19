#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include <map>

#define COUNT_ITEMS		4
#define COUNT_TILES		5
#define COUNT_OBJECTS	6
#define COUNT_CREATURES 3
#define COUNT_BIOMES	6

class Database {
public:

	static struct Item
	{
		std::string name = "null";
		unsigned int id = 0;
		unsigned int texture = 0;

		unsigned int placedAs;
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
		bool wall = 0;
		bool multitexture = 0;
		bool smooth = 0;
		bool destructable = 0;
		glm::vec3 color = glm::vec3(0.0);

		float health = 0;
		unsigned int dropsAs;

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
		glm::vec3 color = glm::vec3(0.0);
		float knockback = 0;
		float meleeDamage = 0;

		float health = 0;
		unsigned int dropsAs;
	} creatures[COUNT_CREATURES];

	static struct BiomeTileHeight 
	{
		float height;
		int id;
		
		unsigned int grassId = 0;
		float grassRarity = 0.0;
		float grassNoiseFrequency = 0.0;

		unsigned int plantId = 0;
		float plantRarity = 0.0;
		float plantNoiseFrequency = 0.0;
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

		//Items
		items[0] = { "Empty", 0, 0, 0 };
		items[1] = { "Stone", 1, 17, 0 };
		items[2] = { "Plant fiber", 2, 18, 0 };
		items[3] = { "Log", 3, 16, 0 };

		//Tiles
		tiles[0] = { "Soil", 0, 0 };
		tiles[1] = { "Water", 1, 1 };
		tiles[2] = { "Sand", 2, 2 };
		tiles[3] = { "Grassy soil", 3, 4 };
		tiles[4] = { "Gravel", 4, 8 };

		//Objects
		objects[0] = { "Air", 0, 9, false, false, false, false, glm::vec3(1), 0, 0 };
		objects[1] = { "Grass", 1, 5, false, false, false, true, glm::vec3(1), 0.3, 2 };
		objects[2] = { "Spruce", 2, 6, false, false, false, true, glm::vec3(1), 1.0, 3 };
		objects[3] = { "Birch", 3, 7, false, false, false, true, glm::vec3(1), 1.0, 3 };
		objects[4] = { "Wood wall", 4, 0, true, true, true, true, glm::vec3(0.588235, 0.435294, 0.2), 2.0, 3 };
		objects[5] = { "Stone wall", 5, 0, true, true, false, true, glm::vec3(0.5, 0.5, 0.5), 3.5, 1 };

		//Creatures
		creatures[0] = { "Player", 0, 3, 19, 35, 51, true, false, glm::vec3(0.0, 0.0, 1.0), 1.0, 1.0, 5.0, 0 };
		creatures[1] = { "Enemy", 1, 3, 19, 35, 51, false, false, glm::vec3(1.0, 0.0, 0.0), 1.0, 1.0, 5.0, 2 };

		//Biomes
#define NOISE_SCALE			0.02
#define NOISE_GRASS			0.4
#define NOISE_FOREST		0.3

		BiomeTileHeight height0;
		BiomeTileHeight height1;
		BiomeTileHeight height2;
		BiomeTileHeight height3;

		//Ocean
		height0 = { 0.8, 1};
		height1 = { 0.95, 2 };
		height2 = { 1.0, 0 };
		biomes[0] = { "Ocean", 0, 0.5, 0.5, 0.02, std::vector<BiomeTileHeight> { height0, height1, height2 } };

		//Beach
		height0 = { 1.0, 2 };
		biomes[1] = { "Beach", 0, 0.5, 0.5, 0.02, std::vector<BiomeTileHeight> { height0 } };

		//Default
		height0 = { 0.05, 1 };
		height1 = { 0.10, 2 };
		height2 = { 0.80, 0, 1, 0.6, 20.0 };
		height3 = { 1.00, 4 };
		biomes[2] = { "Default", 0, 0.5, 0.5, 0.02, std::vector<BiomeTileHeight> { height0, height1, height2, height3 } };

		//Forest
		height0 = { 0.05, 1 };
		height1 = { 0.10, 2 };
		height2 = { 0.80, 0, 1, 0.6, 20.0, 2, 0.6, 20.0 };
		height3 = { 1.00, 4 };
		biomes[3] = { "Forest", 0, 0.5, 0.5, 0.02, std::vector<BiomeTileHeight> { height0, height1, height2, height3 } };

		//Forest 2
		height0 = { 0.05, 1 };
		height1 = { 0.10, 2 };
		height2 = { 0.80, 0, 1, 0.6, 20.0, 3, 0.6, 20.0 };
		height3 = { 1.09, 4 };
		biomes[4] = { "Forest 2", 0, 0.5, 0.5, 0.02, std::vector<BiomeTileHeight> { height0, height1, height2, height3 } };

		//Mountains
		height0 = { 1.0, 4, 5, 0.0, 0.0 };
		biomes[5] = { "Mountains", 0, 0.5, 0.5, 0.02, std::vector<BiomeTileHeight> { height0 } };
	}

	static Biome getBiome(float height, float height2) {
		if (height < 0.5) return biomes[0];
		if (height < 0.51) return biomes[1];
		else if (height < 0.75) {
			if (height2 < 0.4) return biomes[2];
			else if (height2 < 0.7) return biomes[3];
			else if (height2 < 1.0) return biomes[4];
		}
		else if (height < 1.0) return biomes[5];
		
		return Biome();
	}

};