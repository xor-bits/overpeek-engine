#include "inventory.h"
#include "database.h"

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

	//m_renderer->renderText(-1, 0, 0.2, 0.2, "213 3180 ajwpin", glm::vec3(1.0f, 1.0f, 1.0f), TEXT_ALIGN_BOTTOM, TEXT_ALIGN_RIGHT);
	for (int x = 0; x < INVENTORY_WIDTH; x++)
	{
		if (x == int(selectedSlot)) {
			m_renderer->renderBox((x - ((INVENTORY_WIDTH+0.2) / 2.0)) * INVENTORY_SCALE, -1.0, INVENTORY_SCALE*1.2, INVENTORY_SCALE*1.2, 0, 80);
			if (m_itemIds[x][0] != 0) m_renderer->renderBox((x - ((INVENTORY_WIDTH + 0.2) / 2.0)) * INVENTORY_SCALE, -1.0, INVENTORY_SCALE*1.2, INVENTORY_SCALE*1.2, 0, Database::items[m_itemIds[x][0]].texture);
		} else if (x > int(selectedSlot)) {
			m_renderer->renderBox((x - ((INVENTORY_WIDTH + 0.2) / 2.0) + 0.2) * INVENTORY_SCALE, -1.0, INVENTORY_SCALE, INVENTORY_SCALE, 0, 80);
			if (m_itemIds[x][0] != 0) {
				m_renderer->renderBox((x - ((INVENTORY_WIDTH + 0.2) / 2.0) + 0.2) * INVENTORY_SCALE, -1.0, INVENTORY_SCALE, INVENTORY_SCALE, 0, Database::items[m_itemIds[x][0]].texture);
			}
		}
		else {
			m_renderer->renderBox((x - ((INVENTORY_WIDTH + 0.2) / 2.0)) * INVENTORY_SCALE, -1.0, INVENTORY_SCALE, INVENTORY_SCALE, 0, 80);
			if (m_itemIds[x][0] != 0) m_renderer->renderBox((x - ((INVENTORY_WIDTH + 0.2) / 2.0)) * INVENTORY_SCALE, -1.0, INVENTORY_SCALE, INVENTORY_SCALE, 0, Database::items[m_itemIds[x][0]].texture);
		}
	}

	if (visible) {
		for (int y = 0; y < INVENTORY_HEIGHT; y++)
		{
			for (int x = 0; x < INVENTORY_WIDTH; x++)
			{
				m_renderer->renderBox((x - (INVENTORY_WIDTH / 2.0)) * INVENTORY_SCALE, (y - (INVENTORY_HEIGHT / 2.0)) * INVENTORY_SCALE, INVENTORY_SCALE, INVENTORY_SCALE, 0, 80);
				if (m_itemIds[x][y+1] != 0) m_renderer->renderBox((x - ((INVENTORY_WIDTH + 0.2) / 2.0)) * INVENTORY_SCALE, (y - (INVENTORY_HEIGHT / 2.0)) * INVENTORY_SCALE, INVENTORY_SCALE, INVENTORY_SCALE, 0, Database::items[m_itemIds[x][y + 1]].texture);
			}
		}
	}
}

void Inventory::update() {

}

void Inventory::clear() {
	for (int y = 0; y < INVENTORY_HEIGHT + 1; y++)
	{
		for (int x = 0; x < INVENTORY_WIDTH; x++)
		{
			m_itemIds[x][y] = 0;
		}
	}
}

void Inventory::removeSelected() {
	if (!this) return;

	m_itemIds[int(selectedId)][0] = 0;
}

bool Inventory::addItem(int id) {
	for (int y = 0; y < INVENTORY_HEIGHT + 1; y++)
	{
		for (int x = 0; x < INVENTORY_WIDTH; x++)
		{
			if (m_itemIds[x][y] == 0) {
				m_itemIds[x][y] = id;
				//tools::Logger::info(id);
				return true;
			}
		}
	}
	return false;
}