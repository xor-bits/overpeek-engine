#pragma once

#include "../../engine.h"

#define INVENTORY_WIDTH 5
#define INVENTORY_HEIGHT 5

#define INVENTORY_SCALE 0.2

class Inventory {
private:
	graphics::Shader *m_shader;
	graphics::Window *m_window;
	int itemIds[INVENTORY_WIDTH][INVENTORY_HEIGHT + 1];

public:
	bool visible;

public:
	Inventory(graphics::Shader *shader, graphics::Window *window);

	void render(graphics::Renderer *m_renderer);
	void update();

	inline bool addItem(int id) {
		for (int y = 0; y < INVENTORY_HEIGHT + 1; y++)
		{
			for (int x = 0; x < INVENTORY_WIDTH; x++)
			{
				if (itemIds[x][y] == 0) {
					itemIds[x][y] = id;
					return true;
				}
			}
		}
		return false;
	}

};