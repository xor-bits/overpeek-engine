#pragma once

#include <string>
#include <glm/glm.hpp>
#include <vector>

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

		float melee_damage = 0;
		float melee_kb = 0;
		float break_speed = 0;

		unsigned int placedAs;

		Item() {}
		Item(std::string _name, unsigned int _id, unsigned int _texture, unsigned int _stack, float _melee_damage, float _melee_kb, float _break_speed, unsigned int _placedAs) {
			name = _name;
			id = _id;
			texture = _texture;
			stack_size = _stack;
			melee_damage = _melee_damage;
			melee_kb = _melee_kb;
			break_speed = _break_speed;
			placedAs = _placedAs;
		}
	};

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
	};

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
	};

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

		float walkSpeed = 0.0f;

		Creature() {}
		Creature(std::string _name, unsigned int _id, float _walkSpeed, bool _friendly, bool _ghost, float _kb, float _melee, float _hp, float _hpg, float _st, float _stg, unsigned int _dropsAs, glm::vec3 _color, unsigned int _texture_heading_up, unsigned int _texture_heading_down, unsigned int _texture_heading_left, unsigned int _texture_heading_right) {
			name = _name;
			walkSpeed = _walkSpeed;
			id = _id;
			friendly = _friendly;
			ghost = _ghost;
			knockback = _kb;
			meleeDamage = _melee;
			health = _hp;
			healthgain = _hpg;
			stamina = _st;
			staminagain = _stg;
			dropsAs = _dropsAs;
			color = _color;
			texture_heading_up = _texture_heading_up;
			texture_heading_down = _texture_heading_down;
			texture_heading_left = _texture_heading_left;
			texture_heading_right = _texture_heading_right;
		}
	};

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

		Biome(std::string _name, unsigned int _id, float _temperature, float _humidity, float _noiseScale, std::vector<BiomeTileHeight> _heightMap) {
			name = _name;
			id = _id;
			temperature = _temperature;
			humidity = _humidity;
			noiseScale = _noiseScale;
			heightMap = _heightMap;
		}
	};



	static std::vector<Item> items;
	static std::vector<Biome> biomes;
	static std::vector<Creature> creatures;
	static std::vector<Object> objects;
	static std::vector<Tile> tiles;

	static void init() {

		//Items					//Name			//Id	//Texture	//Stack size	//Melee		//Knock		//Break		//Placed as
		items.push_back(Item(	"Empty",		0,		9,			99,				0.0,		0.0,		0.0,		0));
		items.push_back(Item(	"Stone",		1,		17,			99,				0.0,		0.0,		0.0,		5));
		items.push_back(Item(	"Plant fiber",	2,		18,			99,				0.0,		0.0,		0.0,		0));
		items.push_back(Item(	"Log",			3,		16,			99,				0.0,		0.0,		0.0,		4));
		items.push_back(Item(	"Sword",		4,		22,			1,				4.0,		4.0,		0.0,		0));
		items.push_back(Item(	"Pickaxe",		5,		23,			1,				2.0,		4.0,		4.0,		0));

		//Tiles					//Name			//Id	//Texture
		tiles.push_back(Tile(	"Soil",			0,		0));
		tiles.push_back(Tile(	"Water",		1,		1));
		tiles.push_back(Tile(	"Sand",			2,		2));
		tiles.push_back(Tile(	"Grassy soil",	3,		3));
		tiles.push_back(Tile(	"Gravel",		4,		7));

		//Objects					//Name			//id	//Texture	//Wall		//Multitexture	//Smooth	//Destroyable	//Color							//Health	//Drop as		
		objects.push_back(Object(	"Air",			0,		0,			false,		false,			false,		false,			glm::vec3(1.0f),				0,			0));
		objects.push_back(Object(	"Grass",		1,		4,			false,		false,			false,		true,			glm::vec3(1.0f),				1,			2));
		objects.push_back(Object(	"Spruce",		2,		5,			false,		false,			false,		true,			glm::vec3(1.0f),				5,			3));
		objects.push_back(Object(	"Birch",		3,		6,			false,		false,			false,		true,			glm::vec3(1.0f),				5,			3));
		objects.push_back(Object(	"Wood wall",	4,		207,		true,		true,			true,		true,			glm::vec3(0.58f, 0.43f, 0.2f),	10,			3));
		objects.push_back(Object(	"Stone wall",	5,		255,		true,		true,			false,		true,			glm::vec3(0.5f, 0.5f, 0.5f),	15,			1));
		objects.push_back(Object(	"Spawner",		6,		24,			false,		false,			false,		true,			glm::vec3(1.0f),				5,			0));
		objects.push_back(Object(	"Mover",		7,		26,			false,		false,			false,		true,			glm::vec3(1.0f),				5,			0));
		
		//Creatures						//Name		//Id	//Walkspeed	//Friendly		//Ghost		//Knockback		//Melee		//Hp	//Regen		//Stamina	//Staminaregen	//Drops as	//Color			//Textures >>>>	//Up	//Down	//Left	//Right
		creatures.push_back(Creature(	"Player",	0,		1.0,		true,			false,		0.3f,			1.0f,		20.0f,	0.005f,		5.0f,		0.005f,			0,			glm::vec3(1.0f),				8,		9,		10,		11));
		creatures.push_back(Creature(	"Whatever",	1,		1.0,		false,			false,		0.3f,			4.0f,		20.0f,	0.0f,		5.0f,		5.0f,			2,			glm::vec3(1.0f),				8,		9,		10,		11));
		creatures.push_back(Creature(	"Item",		2,		1.0,		false,			false,		0.0f,			0.0f,		1.0f,	0.0f,		0.0f,		0.0f,			0,			glm::vec3(1.0f),				0,		0,		0,		0));

		//Biomes

		BiomeTileHeight height0;
		BiomeTileHeight height1;
		BiomeTileHeight height2;
		BiomeTileHeight height3;

		//Ocean
		height0 = { 0.8f, 1};
		height1 = { 0.95f, 2 };
		height2 = { 1.0f, 0 };
		std::vector<BiomeTileHeight> heightmap = { height0, height1, height2 };
		biomes.push_back(Biome("Ocean", 0, 0.5f, 0.5f, 0.02f, heightmap));

		//Beach
		height0 = { 1.0, 2 };
		heightmap = { height0 };
		biomes.push_back(Biome("Beach", 0, 0.5f, 0.5f, 0.02f, heightmap));

		//Default
		height0 = { 0.05f, 1 };
		height1 = { 0.10f, 2 };
		height2 = { 0.80f, 0, 1, 0.5f };
		height3 = { 1.00f, 4 };
		heightmap = { height0, height1, height2, height3 };
		biomes.push_back(Biome("Default", 0, 0.5f, 0.5f, 0.02f, heightmap));

		//Forest
		height0 = { 0.05f, 1 };
		height1 = { 0.10f, 2 };
		height2 = { 0.80f, 0, 1, 0.5f, 2, 0.5f };
		height3 = { 1.00f, 4 };
		heightmap = { height0, height1, height2, height3 };
		biomes.push_back(Biome("Forest", 0, 0.5f, 0.5f, 0.02f, heightmap));

		//Forest 2
		height0 = { 0.05f, 1 };
		height1 = { 0.10f, 2 };
		height2 = { 0.80f, 0, 1, 0.5f, 3, 0.5f };
		height3 = { 1.09f, 4 };
		heightmap = { height0, height1, height2, height3 };
		biomes.push_back(Biome("Forest 2", 0, 0.5f, 0.5f, 0.02f, heightmap));

		//Mountains
		height0 = { 1.0f, 4, 5, 0.1f };
		heightmap = { height0 };
		biomes.push_back(Biome("Mountains", 0, 0.5f, 0.5f, 0.02f, heightmap));
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