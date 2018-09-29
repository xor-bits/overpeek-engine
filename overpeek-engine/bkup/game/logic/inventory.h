#pragma once

#include "../../engine.h"

class Inventory {
private:
	graphics::Shader *m_shader;
	graphics::SimpleRenderer *m_renderer;
	graphics::Window *m_window;
	int m_wood;

public:
	Inventory(graphics::Shader *shader, graphics::SimpleRenderer *renderer, graphics::Window *window);

	void render();
	void update();

	inline void addItem(int id) {
		m_wood++;
	}

};