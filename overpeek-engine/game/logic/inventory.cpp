#include "inventory.h"

Inventory::Inventory(graphics::Shader *shader, graphics::Window *window) {
	m_shader = shader; m_window = window;

	for (int x = 0; x < INVENTORY_WIDTH; x++)
	{
		for (int y = 0; y < INVENTORY_HEIGHT + 1; y++)
		{
			m_itemIds[x][y] = 0;
		}
	}
}

void Inventory::render(graphics::Renderer *m_renderer) {
	if (selectedSlot <= -1) selectedSlot = 4;
	if (selectedSlot >= 5) selectedSlot = 0;

	selectedId = m_itemIds[int(selectedSlot)][0];

	for (int x = 0; x < INVENTORY_WIDTH; x++)
	{
		if (x == int(selectedSlot)) {
			m_renderer->renderBox((x - ((INVENTORY_WIDTH+0.2) / 2.0)) * INVENTORY_SCALE, -1.0, INVENTORY_SCALE*1.2, INVENTORY_SCALE*1.2, 38);
			if (m_itemIds[x][0] != 0) m_renderer->renderBox((x - ((INVENTORY_WIDTH + 0.2) / 2.0)) * INVENTORY_SCALE, -1.0, INVENTORY_SCALE*1.2, INVENTORY_SCALE*1.2, 19);
		} else if (x > int(selectedSlot)) {
			m_renderer->renderBox((x - ((INVENTORY_WIDTH + 0.2) / 2.0) + 0.2) * INVENTORY_SCALE, -1.0, INVENTORY_SCALE, INVENTORY_SCALE, 38);
			if (m_itemIds[x][0] != 0) m_renderer->renderBox((x - ((INVENTORY_WIDTH + 0.2) / 2.0) + 0.2) * INVENTORY_SCALE, -1.0, INVENTORY_SCALE, INVENTORY_SCALE, 19);
		}
		else {
			m_renderer->renderBox((x - ((INVENTORY_WIDTH + 0.2) / 2.0)) * INVENTORY_SCALE, -1.0, INVENTORY_SCALE, INVENTORY_SCALE, 38);
			if (m_itemIds[x][0] != 0) m_renderer->renderBox((x - ((INVENTORY_WIDTH + 0.2) / 2.0)) * INVENTORY_SCALE, -1.0, INVENTORY_SCALE, INVENTORY_SCALE, 19);
		}
	}

	if (visible) {
		for (int y = 0; y < INVENTORY_HEIGHT; y++)
		{
			for (int x = 0; x < INVENTORY_WIDTH; x++)
			{
				m_renderer->renderBox((x - (INVENTORY_WIDTH / 2.0)) * INVENTORY_SCALE, (y - (INVENTORY_HEIGHT / 2.0)) * INVENTORY_SCALE, INVENTORY_SCALE, INVENTORY_SCALE, 38);
				if (m_itemIds[x][y+1] != 0) m_renderer->renderBox((x - ((INVENTORY_WIDTH + 0.2) / 2.0)) * INVENTORY_SCALE, (y - (INVENTORY_HEIGHT / 2.0)) * INVENTORY_SCALE, INVENTORY_SCALE, INVENTORY_SCALE, 19);
			}
		}
	}
}

void Inventory::update() {

}