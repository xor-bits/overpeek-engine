#include <engine/engine.h>

#include <string>



oe::graphics::MultiTextureShader* shader;
oe::graphics::Renderer* renderer;
oe::graphics::Font* font;

void render(float update_fraction) {
	// clear framebuffer
	oe::graphics::Window::clearWindow();

	// begin submitting
	renderer->begin();
	renderer->clear();

	// submitting
#if 0
	oe::graphics::Text::submit(*renderer, "0@#%()[]{}<>,.;:?!|/\\", { 0.0f, -0.2f }, 0.14f, oe::graphics::alignment::center_center);
	oe::graphics::Text::submit(*renderer, "1234567890 =+-/*", { 0.0f, 0.0f }, 0.14f, oe::graphics::alignment::center_center);
	oe::graphics::Text::submit(*renderer, "The quick brown fox jumps over the lazy dog.", { 0.0f, 0.4f }, 0.14f, oe::graphics::alignment::center_center);
#else
	oe::graphics::Text::submit(*renderer, "blue:<#0000ff> test", { 0.0f, -0.2f }, 0.14f, oe::graphics::alignment::center_center);
	oe::graphics::Text::submit(*renderer, "incomplete:<#542> test", { 0.0f, 0.0f }, 0.14f, oe::graphics::alignment::center_center);
	oe::graphics::Text::submit(*renderer, "faulty:<#5f>>>>>>>>> test", { 0.0f, 0.2f }, 0.14f, oe::graphics::alignment::center_center);
	oe::graphics::Text::submit(*renderer, "with 0x:<#0x4354> test", { 0.0f, 0.4f }, 0.14f, oe::graphics::alignment::center_center);
	oe::graphics::Text::submit(*renderer, "negative:<#-43531> test", { 0.0f, 0.6f }, 0.14f, oe::graphics::alignment::center_center);
#endif

	// bind font texture and shader
	oe::graphics::Text::getFont()->bindTexture();
	shader->bind();

	// stop submitting and render
	renderer->end();
	renderer->render();

	// swap buffers and poll events
	oe::graphics::Window::updateWindow();
	oe::graphics::Window::pollEvents();

	// check if needs to close
	if (oe::graphics::Window::windowShouldClose()) oe::utils::GameLoop::stop();
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
	oe::graphics::Window::WindowConfig window_config;
	window_config.title = "Test 2 - Colored Text";
	window_config.multisamples = 4;
	window_config.opengl_debugmode = true;
	oe::graphics::Window::init(window_config);
	oe::graphics::Window::setResizeCallback(resize);
	oe::graphics::GL::setBackFaceCulling(true);
	oe::graphics::GL::enableBlending();

	// drawing
	font = new oe::graphics::Font();
	oe::graphics::Text::setFont(*font);
	renderer = new oe::graphics::Renderer(oe::graphics::types::dynamicrender, oe::graphics::types::staticrender, 1000, nullptr);
	shader = new oe::graphics::MultiTextureShader();

	// matrices
	glm::mat4 pr = glm::ortho(-oe::graphics::Window::aspect(), oe::graphics::Window::aspect(), 1.0f, -1.0f);
	shader->projectionMatrix(pr);
	shader->useTexture(true);
	
	// start
	oe::utils::GameLoop::init(render, update, 1);

	// closing
	oe::graphics::Window::closeWindow();
	delete shader;
	delete renderer;
	delete font;

	return 0;
}
