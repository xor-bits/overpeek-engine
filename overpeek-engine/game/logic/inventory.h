#pragma once

#include "../../engine.h"

class Inventory {
private:
	graphics::Shader *m_shader;
	graphics::Window *m_window;
	int m_wood;

public:
	bool visible;

public:
	Inventory(graphics::Shader *shader, graphics::Window *window);

	void render();
	void update();

	inline void addItem(int id) {
		m_wood++;
	}

};