#include "inventory.h"

Inventory::Inventory(graphics::Shader *shader, graphics::SimpleRenderer *renderer, graphics::Window *window) {
	m_shader = shader; m_renderer = renderer; m_window = window;
	m_wood = 5;
}

void Inventory::render() {
	std::string str = "Wood: "; str += std::to_string(m_wood);
	m_renderer->renderText(-m_window->getAspect(), 0.0, 0.05, 0.05, str, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_LEFT, TEXT_ALIGN_CENTER);
}

void Inventory::update() {

}