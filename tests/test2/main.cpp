#include <engine/engine.h>

#include <string>
#include "FastNoise.h"



oe::graphics::MultiTextureShader* shader;
oe::graphics::Renderer* renderer;
oe::graphics::Font* font;
<<<<<<< HEAD
=======
oe::graphics::Texture* texture;
constexpr int width = 64;
constexpr int height = 64;
constexpr int channels = 4;
unsigned char data[width * height * channels];
FastNoise* noise;
>>>>>>> 8407de2847ef13a7573967e21e079fde70e9df00

void render(float update_fraction) {
	// clear framebuffer
	oe::graphics::Window::clear();

<<<<<<< HEAD
=======
	// Update texture
	// This is stupid
	// Just for testing
	static glm::vec2 delta = { 0.0f, 0.0f };
	const glm::vec2& cursor = oe::graphics::Window::getMousePos();
	delta += cursor * (0.00001f * oe::utils::GameLoop::getMSPerFrame());
	memset(data, (char)255, width * height * channels);
	for (size_t x = 0; x < width; x++) {
		for (size_t y = 0; y < height; y++) {
			unsigned char value = (noise->GetSimplexFractal((float)x / (float)width + delta.x, (float)y / (float)height + delta.y) + 1.0f) * 128.0f;
			data[x * channels + y * width * channels + 0] = value;
			data[x * channels + y * width * channels + 1] = value;
			data[x * channels + y * width * channels + 2] = value;
			data[x * channels + y * width * channels + 3] = 255;
		}
	}

	// Texture
	renderer->begin();
	renderer->clear();
	texture->data2D(data, 0, 0, width, height);
	renderer->submit({ -0.8f, -0.8f }, { 1.6f, 1.6f });
	texture->bind();
	shader_single->bind();
	renderer->end();
	renderer->render();

>>>>>>> 8407de2847ef13a7573967e21e079fde70e9df00
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
<<<<<<< HEAD
=======
	shader_single = new oe::graphics::SingleTextureShader();
	texture = new oe::graphics::Texture();
	texture->empty2D(width, height);
	noise = new FastNoise(oe::utils::Clock::getMicroseconds());
	noise->SetFractalOctaves(8);
	noise->SetFractalGain(0.5f);
	noise->SetFractalLacunarity(1.72f);
	noise->SetFrequency(2.0f);
	std::thread *worker = new std::thread([] {while (true) { update(); } });
	worker->detach();
>>>>>>> 8407de2847ef13a7573967e21e079fde70e9df00

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
