#pragma once

#include "../../engine.h"
#include "database.h"

#define RENDER_DST 3

#define REGION_SIZE 8
#define TILE_SIZE 0.2

#define USER_PROFILE std::string(getenv("USERPROFILE"))
#define SAVE_PATH std::string(USER_PROFILE + "\\AppData\\Roaming\\_overpeek-game\\")
#define WORLD_NAME std::string("test")

class Tile;
class Region;
class Player;
class Enemy;
class Game {
private:
	static graphics::Window *m_window;
	static graphics::Shader *m_shader;
	static graphics::Renderer *m_renderer;
	static logic::GameLoop *m_loop;
	static glm::ivec2 m_hover_tile;

	static Region *m_region[RENDER_DST * 2][RENDER_DST * 2];
	static Player *m_player;

	static float lastRegionX;
	static float lastRegionY;

	static int hitCooldown;
	
	static void renderInfoScreen();
	static void processNewArea();

public:
	static bool debugMode;

	static void init(graphics::Shader *shader, graphics::Window * window, logic::GameLoop *loop);
	static void render();
	static void update();
	static void close();
	static void rapidUpdate();

	void keyPress(int key);
	void buttonPress(int button);
	void scroll(double y);
	
	inline static graphics::Shader *getShader() { return m_shader; }
	inline static graphics::Window *getWindow() { return m_window; }

	static int screenToWorldX(float x);
	static int screenToWorldY(float y);
	static int posToRegionPos(float xOrY);
	
	static Region *getRegion(float x, float y);
	static Tile *getTile(float x, float y);
	static Player *getPlayer();

	static bool trySetTileObject(float x, float y, int id);
	static bool trySetTileObject(Tile *tile, int id);
};