#pragma once

#include "../../engine.h"

#define RENDER_DST 3

#define REGION_SIZE 8
#define TILE_SIZE 0.2

class Tile;
class Region;
class Player;
class Game {
private:
	static graphics::Window *m_window;
	static graphics::Shader *m_shader;
	static graphics::SimpleRenderer *m_renderer;
	static logic::GameLoop *m_loop;
	static Tile *m_hover_tile;

	static Region *m_region[RENDER_DST * 2][RENDER_DST * 2];
	static Player *m_player;

	static float lastRegionX;
	static float lastRegionY;

	static int hitCooldown;

public:
	static void init(graphics::Shader *shader, graphics::Window * window, logic::GameLoop *loop);
	static void render();
	static void renderDebugScreen();
	static void update();
	static void rapidUpdate();
	
	inline static graphics::Shader *getShader() { return m_shader; }
	inline static graphics::Window *getWindow() { return m_window; }

	static int screenToWorldX(float x);
	static int screenToWorldY(float y);
	static Region *getRegion(int x, int y);
	static Tile *getTile(int x, int y);
};