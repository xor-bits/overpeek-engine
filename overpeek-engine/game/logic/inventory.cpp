#include "inventory.h"

Inventory::Inventory(graphics::Shader *shader, graphics::Window *window) {
	m_shader = shader; m_window = window;

	for (int x = 0; x < INVENTORY_WIDTH; x++)
	{
		for (int y = 0; y < INVENTORY_HEIGHT + 1; y++)
		{
			itemIds[x][y] = 0;
		}
	}
}

void Inventory::render(graphics::Renderer *m_renderer) {
	for (int x = 0; x < INVENTORY_WIDTH; x++)
	{
		m_renderer->renderBox((x - (INVENTORY_WIDTH / 2.0)) * INVENTORY_SCALE, -1.0, INVENTORY_SCALE, INVENTORY_SCALE, 38);
		if (itemIds[x][0] != 0) m_renderer->renderBox((x - (INVENTORY_WIDTH / 2.0)) * INVENTORY_SCALE, -1.0, INVENTORY_SCALE, INVENTORY_SCALE, 19);
	}

	if (visible) {
		for (int y = 0; y < INVENTORY_HEIGHT; y++)
		{
			for (int x = 0; x < INVENTORY_WIDTH; x++)
			{
				m_renderer->renderBox((x - (INVENTORY_WIDTH / 2.0)) * INVENTORY_SCALE, (y - (INVENTORY_HEIGHT / 2.0)) * INVENTORY_SCALE, INVENTORY_SCALE, INVENTORY_SCALE, 38);
				if (itemIds[x][y+1] != 0) m_renderer->renderBox((x - (INVENTORY_WIDTH / 2.0)) * INVENTORY_SCALE, (y - (INVENTORY_HEIGHT / 2.0)) * INVENTORY_SCALE, INVENTORY_SCALE, INVENTORY_SCALE, 19);
			}
		}
	}
}

void Inventory::update() {

}