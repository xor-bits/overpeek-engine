#pragma once

#define INVENTORY_WIDTH 5
#define INVENTORY_HEIGHT 5

#define INVENTORY_SCALE 0.2

namespace oe { class Window; class Shader; class Renderer; }
class Inventory {
private:
	oe::Shader *m_shader;
	oe::Window *m_window;
	int m_itemIds[INVENTORY_WIDTH][INVENTORY_HEIGHT + 1];

public:
	int selectedId;
	float selectedSlot;
	bool visible;
	bool load();

public:
	Inventory(oe::Shader *shader, oe::Window *window);

	void init();
	void save();
	void render(oe::Renderer *m_renderer);
	void update();
	void clear();
	void removeSelected();
	bool addItem(int id);

};