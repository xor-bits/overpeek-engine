#include "../pch.h"

#include "inventory.h"
#include "database.h"
#include "game.h"
#include "../world/map.h"
#include "../creatures/player.h"

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
	short *data = (short*)oe::BinaryIO::read<short>(Game::getSaveLocation() + "inventory.data", inventory_data_size);
	if (!data) return false;
	for (int x = 0; x < INVENTORY_WIDTH; x++)
	{
		for (int y = 0; y < INVENTORY_HEIGHT + 1; y++)
		{
			m_itemIds[x][y] = data[(x + INVENTORY_WIDTH * y) * 2 + 0];
			m_itemCounts[x][y] = data[(x + INVENTORY_WIDTH * y) * 2 + 1];
		}
	}
#else
	return false;
#endif
}

void Inventory::save() {
	short data[(INVENTORY_WIDTH * (INVENTORY_HEIGHT + 1)) * 2];
	for (int x = 0; x < INVENTORY_WIDTH; x++)
	{
		for (int y = 0; y < INVENTORY_HEIGHT + 1; y++)
		{
			data[(x + INVENTORY_WIDTH * y) * 2 + 0] = m_itemIds[x][y];
			data[(x + INVENTORY_WIDTH * y) * 2 + 1] = m_itemCounts[x][y];
		}
	}
	oe::BinaryIO::write<short>(Game::getSaveLocation() + "inventory.data", data, (INVENTORY_WIDTH * (INVENTORY_HEIGHT + 1)) * 2);
}

void Inventory::render(oe::Renderer *m_renderer) {
#if 1
	if (selectedSlot <= -1) selectedSlot = 4;
	if (selectedSlot >= 5) selectedSlot = 0;
	
	selectedId = m_itemIds[int(selectedSlot)][0];
	
	for (int x = 0; x < INVENTORY_WIDTH; x++)
	{
		if (x == int(selectedSlot)) {
			renderOneSlot(m_renderer,
				(x - INVENTORY_WIDTH / 2.0) * INVENTORY_SCALE * Game::renderScale(),
				-1.0,
				INVENTORY_SCALE * Game::renderScale() * 1.2,
				m_itemIds[x][0],
				m_itemCounts[x][0]
			);
		} else if (x > int(selectedSlot)) {
			renderOneSlot(m_renderer,
				(x - INVENTORY_WIDTH / 2.0 + 0.2) * INVENTORY_SCALE * Game::renderScale(),
				-1.0,
				INVENTORY_SCALE * Game::renderScale(),
				m_itemIds[x][0],
				m_itemCounts[x][0]
			);
		}
		else {
			renderOneSlot(m_renderer, 
				(x - INVENTORY_WIDTH / 2.0) * INVENTORY_SCALE * Game::renderScale(),
				-1.0, 
				INVENTORY_SCALE * Game::renderScale(), 
				m_itemIds[x][0], 
				m_itemCounts[x][0]
			);
		}
	}
	
	if (visible) {
		for (int y = 0; y < INVENTORY_HEIGHT; y++)
		{
			for (int x = 0; x < INVENTORY_WIDTH; x++)
			{
				
				renderOneSlot(m_renderer, 
					(x - INVENTORY_WIDTH / 2.0) * INVENTORY_SCALE * Game::renderScale(), 
					(y - INVENTORY_HEIGHT / 2.0) * INVENTORY_SCALE * Game::renderScale(),
					INVENTORY_SCALE * Game::renderScale(),
					m_itemIds[x][y + 1], 
					m_itemCounts[x][y + 1]
				);
			}
		}
	}
#endif
}

void Inventory::renderOneSlot(oe::Renderer *m_renderer, float x, float y, float scale, int itemid, int itemcount) {
	//All inventory slots
	m_renderer->renderPoint(
		glm::vec2(x, y),
		glm::vec2(1.0) * scale,
		80,
		glm::vec4(1.0, 1.0, 1.0, 1.0)
	);

	//All inventory items
	if (itemid != 0) {
		m_renderer->renderPoint(
			glm::vec2(x, y),
			glm::vec2(1.0) * scale,
			Database::items[itemid].texture,
			glm::vec4(1.0)
		);
	}
	m_renderer->renderText(
		glm::vec2(x, y),
		glm::vec2(1.0 / 3.0) * scale,
		std::to_string(itemcount).c_str(),
		oe::topLeft
	);
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

	//oe::Logger::out(oe::info, m_itemCounts[int(selectedSlot)][0]);

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
					if (n <= 0) return true;
				}
				else return true;
			}
		}
	}
	return false;
}

void Inventory::dropSelected(int n) {
	
}

void Inventory::dropAll() {
	for (int x = 0; x < INVENTORY_WIDTH; x++)
	{
		for (int y = 0; y < INVENTORY_HEIGHT + 1; y++)
		{
			if (m_itemIds[x][y] != 0) {
				dropItem(x, y, -1);
			}
		}
	}
}

void Inventory::dropItem(int x, int y, int n) {
	if (n == -1) {
		for (int count = 0; count < m_itemCounts[x][y]; count++)
		{
			Game::getMap()->itemDrop(Game::getPlayer()->getX(), Game::getPlayer()->getY(), m_itemIds[x][y]);
		}
		m_itemCounts[x][y] = 0;
		m_itemIds[x][y] = 0;
	}
	for (int count = 0; count < n; count++)
	{
		if (m_itemCounts[x][y] > 0) {
			Game::getMap()->itemDrop(Game::getPlayer()->getX(), Game::getPlayer()->getY(), m_itemIds[x][y]);
			
			m_itemCounts[x][y]--;

			if (m_itemCounts[x][y] <= 0) {
				m_itemIds[x][y] = 0;
				break;
			}
		}
		m_itemIds[x][y] = 0;
		break;
	}
}