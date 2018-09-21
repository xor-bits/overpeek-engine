#pragma once

#include "../../engine.h"

#define RENDER_DST 4

class Region;
class Player;
class Game {
private:
	static graphics::Window *m_window;
	static graphics::Shader *m_shader;
	static graphics::SimpleRenderer *m_renderer;

	static Region *m_region[RENDER_DST * 2][RENDER_DST * 2];
	static Player *m_player;

public:
	static void init(graphics::Shader *shader, graphics::Window * window);
	static void render();
	static void update();
	
	inline static graphics::Shader *getShader() { return m_shader; }
};