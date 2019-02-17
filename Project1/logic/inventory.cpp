#include "../pch.h"

#include "inventory.h"
#include "database.h"
#include "game.h"

Inventory::Inventory(oe::Shader *shader, oe::Window *window) {
	m_shader = shader; m_window = window;

	init();
}

void Inventory::init() {
	if (!load()) {
		for (int x = 0; x < INVENTORY_WIDTH; x++)
		{
			for (int y = 0; y < INVENTORY_HEIGHT + 1; y++)
			{
				m_itemIds[x][y] = 0;
				m_itemCounts[x][y] = 0;
			}
		}
	}
}

bool Inventory::load() {
#if !DEBUG_DISABLE_SAVING
	unsigned long inventory_data_size;
	short *data = (short*)oe::BinaryIO::read<short>(Game::getSaveLocation() + "inventory_data", inventory_data_size);
	if (!data) return false;
	for (int x = 0; x < INVENTORY_WIDTH; x++)
	{
		for (int y = 0; y < INVENTORY_HEIGHT + 1; y++)
		{
			m_itemIds[x][y] = data[x + INVENTORY_WIDTH * y];
		}
	}
#else
	return false;
#endif
}

void Inventory::save() {
	short data[INVENTORY_WIDTH * (INVENTORY_HEIGHT + 1)];
	for (int x = 0; x < INVENTORY_WIDTH; x++)
	{
		for (int y = 0; y < INVENTORY_HEIGHT + 1; y++)
		{
			data[x + INVENTORY_WIDTH * y] = m_itemIds[x][y];
		}
	}
	oe::BinaryIO::write<short>(Game::getSaveLocation() + "inventory.data", data, INVENTORY_WIDTH * (INVENTORY_HEIGHT + 1));
}

void Inventory::render(oe::Renderer *m_renderer) {
#if 1
	if (selectedSlot <= -1) selectedSlot = 4;
	if (selectedSlot >= 5) selectedSlot = 0;
	
	selectedId = m_itemIds[int(selectedSlot)][0];
	
	for (int x = 0; x < INVENTORY_WIDTH; x++)
	{
		if (x == int(selectedSlot)) {
			//Selected slot
			m_renderer->renderPoint(
				glm::vec2((x - ((INVENTORY_WIDTH+0.2) / 2.0)) * INVENTORY_SCALE, -1.0), 
				glm::vec2(INVENTORY_SCALE*1.2, INVENTORY_SCALE*1.2), 
				80, 
				glm::vec4(1.0, 1.0, 1.0, 1.0)
			);
			
			//Selected item
			if (m_itemIds[x][0] != 0) {
				m_renderer->renderPoint(
					glm::vec2((x - ((INVENTORY_WIDTH + 0.2) / 2.0)) * INVENTORY_SCALE, -1.0),
					glm::vec2(INVENTORY_SCALE*1.2, INVENTORY_SCALE*1.2),
					Database::items[m_itemIds[x][0]].texture,
					glm::vec4(1.0, 1.0, 1.0, 1.0)
				);
			}
		} else if (x > int(selectedSlot)) {
			//Slot
			m_renderer->renderPoint(
				glm::vec2((x - ((INVENTORY_WIDTH + 0.2) / 2.0) + 0.2) * INVENTORY_SCALE, -1.0), 
				glm::vec2(INVENTORY_SCALE, INVENTORY_SCALE), 
				80, 
				glm::vec4(1.0, 1.0, 1.0, 1.0)
			);
			
			//Item
			if (m_itemIds[x][0] != 0) {
				m_renderer->renderPoint(
					glm::vec2((x - ((INVENTORY_WIDTH + 0.2) / 2.0) + 0.2) * INVENTORY_SCALE, -1.0), 
					glm::vec2(INVENTORY_SCALE, INVENTORY_SCALE), 
					Database::items[m_itemIds[x][0]].texture, 
					glm::vec4(1.0, 1.0, 1.0, 1.0)
				);
			}
		}
		else {
			//Slot
			m_renderer->renderPoint(
				glm::vec2((x - ((INVENTORY_WIDTH + 0.2) / 2.0)) * INVENTORY_SCALE, -1.0),
				glm::vec2(INVENTORY_SCALE, INVENTORY_SCALE),
				80,
				glm::vec4(1.0, 1.0, 1.0, 1.0)
			);

			//Item
			if (m_itemIds[x][0] != 0) {
				m_renderer->renderPoint(
					glm::vec2((x - ((INVENTORY_WIDTH + 0.2) / 2.0)) * INVENTORY_SCALE, -1.0), 
					glm::vec2(INVENTORY_SCALE, INVENTORY_SCALE), 
					Database::items[m_itemIds[x][0]].texture, 
					glm::vec4(1.0, 1.0, 1.0, 1.0)
				);
			}
		}
	}
	
	if (visible) {
		for (int y = 0; y < INVENTORY_HEIGHT; y++)
		{
			for (int x = 0; x < INVENTORY_WIDTH; x++)
			{
				m_renderer->renderPoint(glm::vec2((x - (INVENTORY_WIDTH / 2.0)) * INVENTORY_SCALE, (y - (INVENTORY_HEIGHT / 2.0)) * INVENTORY_SCALE), glm::vec2(INVENTORY_SCALE, INVENTORY_SCALE), 80, glm::vec4(1.0, 1.0, 1.0, 1.0));
				if (m_itemIds[x][y+1] != 0) m_renderer->renderPoint(glm::vec2((x - ((INVENTORY_WIDTH + 0.2) / 2.0)) * INVENTORY_SCALE, (y - (INVENTORY_HEIGHT / 2.0)) * INVENTORY_SCALE), glm::vec2(INVENTORY_SCALE, INVENTORY_SCALE), Database::items[m_itemIds[x][y + 1]].texture, glm::vec4(1.0, 1.0, 1.0, 1.0));
			}
		}
	}
#endif
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

void Inventory::removeSelected(int n) {
	m_itemCounts[int(selectedSlot)][0] -= n;

	oe::Logger::info(m_itemCounts[int(selectedSlot)][0]);

	if (m_itemCounts[int(selectedSlot)][0] <= 0) m_itemIds[int(selectedSlot)][0] = 0;
}

bool Inventory::addItem(int id, int n) {
	for (int y = 0; y < INVENTORY_HEIGHT + 1; y++)
	{
		for (int x = 0; x < INVENTORY_WIDTH; x++)
		{
			if (m_itemIds[x][y] == 0 || m_itemIds[x][y] == id) {
				m_itemIds[x][y] = id;
				m_itemCounts[x][y] += n;
				if (m_itemCounts[x][y] + n >= Database::items[id].stack_size) {
					m_itemCounts[x][y] = Database::items[id].stack_size;
					n -= Database::items[id].stack_size - m_itemCounts[x][y];
				}
				return true;
			}
		}
	}
	return false;
}