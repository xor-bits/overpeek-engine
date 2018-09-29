#include "inventory.h"

#define WIDTH 1.4
#define HEIGHT 1.0
#define TOP_HEIGHT 0.2
#define X WIDTH / -2.0
#define Y HEIGHT / -2.0

Inventory::Inventory(graphics::Shader *shader, graphics::SimpleRenderer *renderer, graphics::Window *window) {
	m_shader = shader; m_renderer = renderer; m_window = window;
	m_wood = 5;
}

void Inventory::render() {
	if (visible) {
		m_renderer->setColor(glm::vec4(1.0, 0.5, 0.0, 1.0));
		m_renderer->renderBox(X, Y, WIDTH, TOP_HEIGHT);
		m_renderer->setColor(glm::vec4(1.0, 0.5, 0.0, 0.5));
		m_renderer->renderBox(X, Y + TOP_HEIGHT, WIDTH, HEIGHT - TOP_HEIGHT);
		m_renderer->renderText(X, Y, 0.1, 0.1, "Inventory", glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_LEFT, TEXT_ALIGN_BOTTOM);
		std::string str = "Wood: "; str += std::to_string(m_wood);
		m_renderer->renderText(X, Y + TOP_HEIGHT, 0.1, 0.1, str, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_LEFT, TEXT_ALIGN_BOTTOM);
	}
}

void Inventory::update() {

}