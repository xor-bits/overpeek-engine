#include "inventory.h"

#define WIDTH 1.4
#define HEIGHT 1.0
#define X WIDTH / -2.0
#define Y HEIGHT / -2.0

Inventory::Inventory(graphics::Shader *shader, graphics::Window *window) {
	m_shader = shader; m_window = window;
	m_wood = 5;
}

void Inventory::render() {
	if (visible) {
		//glBindTexture(GL_TEXTURE_2D, graphics::TextureLoader::getTexture(3));
		//graphics::Renderer::renderBox(X, Y, WIDTH, HEIGHT);
		//graphics::Renderer::renderText(X, Y, 0.1, 0.1, "Inventory", glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_LEFT, TEXT_ALIGN_BOTTOM);
	}
}

void Inventory::update() {

}