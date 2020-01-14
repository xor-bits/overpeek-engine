#include <engine/engine.h>

#include <string>
#include "FastNoise.h"



oe::graphics::MultiTextureShader* shader;
oe::graphics::Renderer* renderer;
oe::graphics::Font* font;

void render(float update_fraction) {
	// clear framebuffer
	oe::graphics::Window::clear();

	// begin submitting
	renderer->begin();
	renderer->clear();

	// submitting
	oe::graphics::Text::submit(*renderer, "#0000ff0@#%()[]{}<>,.;:?!|/\\", { 0.0f, -0.2f }, 0.14f, oe::graphics::alignment::center_center);
	oe::graphics::Text::submit(*renderer, "#ff00001234567890 =+-/*", { 0.0f,  0.0f }, 0.14f, oe::graphics::alignment::center_center);
	oe::graphics::Text::submit(*renderer, "#00ff00The quick brown fox jumps over the lazy dog.", { 0.0f,  0.2f }, 0.14f, oe::graphics::alignment::center_center);

	// bind font texture and shader
	oe::graphics::Text::getFont()->bindTexture();
	shader->bind();

	// stop submitting and render
	renderer->end();
	renderer->render();

	// swap buffers and poll events
	oe::graphics::Window::update();
	oe::graphics::Window::input();

	// check if needs to close
	if (oe::graphics::Window::shouldClose()) oe::utils::GameLoop::stop();
}

void update() {
	spdlog::info("Performance\nFPS: {}\nmspf: {}\n--------\n", oe::utils::GameLoop::getFPS(), oe::utils::GameLoop::getFrameMilliseconds());
}

void resize(int width, int height) {
	if (height == 0) height = 1;
	float aspect_rato = (float)width / (float)height;

	glm::mat4 pr_matrix = glm::ortho(-aspect_rato, aspect_rato, 1.0f, -1.0f);
	shader->projectionMatrix(pr_matrix);
	shader->useTexture(true);
}

int main() {
	// engine
	oe::init();

	// window
	oe::graphics::Window::init(900, 600, "Test 2 - Text", WINDOW_GL_DEBUG);
	oe::graphics::Window::setResizeCallback(resize);
	oe::graphics::GL::setBackFaceCulling(false);
	oe::graphics::GL::enableBlending();
	oe::graphics::GL::setSwapInterval(0);

	// drawing
	font = new oe::graphics::Font();
	oe::graphics::Text::setFont(*font);
	renderer = new oe::graphics::Renderer(oe::graphics::types::dynamicrender, oe::graphics::types::staticrender, 100, nullptr);
	shader = new oe::graphics::MultiTextureShader();

	// matrices
	glm::mat4 pr = glm::ortho(-oe::graphics::Window::getAspect(), oe::graphics::Window::getAspect(), 1.0f, -1.0f);
	shader->projectionMatrix(pr);
	shader->useTexture(true);
	
	// start
	oe::utils::GameLoop::init(render, update, 1);

	// closing
	oe::graphics::Window::close();
	delete shader;
	delete renderer;
	delete font;

	return 0;
}
