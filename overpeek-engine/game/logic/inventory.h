#pragma once

#include "../../engine.h"

#define INVENTORY_WIDTH 5
#define INVENTORY_HEIGHT 5

#define INVENTORY_SCALE 0.2

class Inventory {
private:
	graphics::Shader *m_shader;
	graphics::Window *m_window;
	int m_itemIds[INVENTORY_WIDTH][INVENTORY_HEIGHT + 1];

public:
	int selectedId;
	float selectedSlot;
	bool visible;

public:
	Inventory(graphics::Shader *shader, graphics::Window *window);

	void render(graphics::Renderer *m_renderer);
	void update();
	void clear();
	void removeSelected();
	bool addItem(int id);

};