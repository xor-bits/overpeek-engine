#pragma once

#define INVENTORY_WIDTH 5
#define INVENTORY_HEIGHT 5

#define INVENTORY_SCALE 0.2f

namespace oe { class Window; class Shader; class Renderer; }
class Inventory {
private:
	oe::Shader *m_shader;
	oe::Window *m_window;
	int m_itemIds[INVENTORY_WIDTH][INVENTORY_HEIGHT + 1];
	int m_itemCounts[INVENTORY_WIDTH][INVENTORY_HEIGHT + 1];

	void renderOneSlot(oe::Renderer *m_renderer, float x, float y, float scale, int itemid, int itemcount);
	
	//Negative n value will drop all items on that slot
	void dropItem(int x, int y, int n);

public:
	int selectedId;
	float selectedSlot;
	bool visible;
	bool load();

	Inventory(oe::Shader *shader, oe::Window *window);

	void init();
	void save();
	void render(oe::Renderer *m_renderer);
	void update();
	void clear();
	void removeSelected(int n);
	bool addItem(int id, int n);

	void dropSelected(int n);
	void dropAll();

};