#pragma once

#include "../../engine.h"
#include "database.h"

#define DEBUG_ZOOM 1.0
#define ENABLE_AUDIO false

#define RENDER_DST 2 * 2

#define REGION_SIZE 8
#define TILE_SIZE 0.1

#define USER_PROFILE std::string(getenv("USERPROFILE"))
#define SAVE_PATH std::string(USER_PROFILE + "\\AppData\\Roaming\\_overpeek-game\\")
#define WORLD_NAME std::string("test")

class Tile;
class Region;
class Player;
class Creature;
class Game {
private:
	static graphics::Window *m_window;
	static graphics::Shader *m_shader;
	static graphics::Shader *m_guishader;
	static graphics::Renderer *m_worldrenderer;
	static graphics::Renderer *m_creaturerenderer;
	static graphics::Renderer *m_guirenderer;
	static logic::GameLoop *m_loop;
	static glm::ivec2 m_hover_tile;

	static Region m_region[RENDER_DST][RENDER_DST];
	static Player *m_player;

	static float lastRegionX;
	static float lastRegionY;

	static int hitCooldown;
	
	static void renderInfoScreen();
	static void shiftRegions(int deltaX, int deltaY, int playerRegionX, int playerRegionY);
	static void shiftRegionLoop(int x, int y, int deltaX, int deltaY, int playerRegionX, int playerRegionY);

public:
	static bool tilesChanged;
	static bool debugMode;

	static void init(graphics::Shader *shader, graphics::Window * window, logic::GameLoop *loop);
	static void render();
	static void update();
	static void close();
	static void rapidUpdate();
	static void processNewArea();

	void keyPress(int key);
	void buttonPress(int button);
	void scroll(double y);
	
	inline static graphics::Shader *getShader() { return m_shader; }
	inline static graphics::Window *getWindow() { return m_window; }

	static void getInfoFromNoise(int &tileId, int &objId, double x, double y);

	static int screenToWorldX(float x);
	static int screenToWorldY(float y);
	static float posToRegionPos(float xOrY);
	
	static Region *getRegion(float x, float y);
	static Tile *getTile(float x, float y, std::string debugText);
	static void findAllCreatures(Region* region, int x, int y, Creature** array, unsigned int& amount);
	static Player *getPlayer();

	static bool trySetTileObject(float x, float y, int id);
	static bool trySetTileObject(Tile *tile, int id);
};