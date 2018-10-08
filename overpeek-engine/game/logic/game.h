#pragma once

#include "../../engine.h"

#define RENDER_DST 5

#define REGION_SIZE 8
#define TILE_SIZE 0.2

#define USER_PROFILE std::string(getenv("USERPROFILE"))
#define SAVE_PATH std::string(USER_PROFILE + "\\AppData\\Roaming\\overpeek-game\\")
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
	static Enemy *m_enemy;

	static float lastRegionX;
	static float lastRegionY;

	static int hitCooldown;

public:
	static void init(graphics::Shader *shader, graphics::Window * window, logic::GameLoop *loop);
	static void render();
	static void renderDebugScreen();
	static void update();
	static void close();
	static void rapidUpdate();
	
	inline static graphics::Shader *getShader() { return m_shader; }
	inline static graphics::Window *getWindow() { return m_window; }

	static int screenToWorldX(float x);
	static int screenToWorldY(float y);
	static Region *getRegion(int x, int y);

	static Tile getTile(int x, int y);
	static int getTileId(int x, int y);
	static int getTileObjectId(int x, int y);

	static void setTileId(int x, int y, int id);
	static void setTileObjectId(int x, int y, int id);
	static void addTileObjectHealth(int x, int y, float amount);
	static void healTileObject(int x, int y);
};