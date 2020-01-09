#include <engine/engine.h>

#include <string>



oe::graphics::MultiTextureShader* shader;
oe::graphics::SingleTextureShader* shader_single;
oe::graphics::Renderer* renderer;
oe::graphics::Font* font;
oe::graphics::Texture* texture;
constexpr int width = 64;
constexpr int height = 64;
constexpr int channels = 4;
unsigned char data[width * height * channels];

void render(float update_fraction) {
	// clear framebuffer
	oe::graphics::Window::clear();

	// Texture
	renderer->begin();
	renderer->clear();
	texture->data2D(data, 0, 0, width, height);
	renderer->submit({ -0.5f, -0.5f }, { 1.0f, 1.0f });
	texture->bind();
	shader_single->bind();
	renderer->end();
	renderer->render();

	// begin submitting
	renderer->begin();
	renderer->clear();

	// submitting
	oe::graphics::Text::submit(*renderer, "The quick brown fox jumps over the lazy dog.", { 0.0f,  0.2f }, 0.14f, oe::graphics::alignment::center_center);
	oe::graphics::Text::submit(*renderer, "1234567890 =+-/*", { 0.0f,  0.0f }, 0.14f, oe::graphics::alignment::center_center);
	oe::graphics::Text::submit(*renderer, "@#%()[]{}<>,.;:?!|/\\", { 0.0f, -0.2f }, 0.14f, oe::graphics::alignment::center_center);

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
	spdlog::info("FPS: " + std::to_string(oe::utils::GameLoop::getFPS()));

#if 1
	for (size_t x = 0; x < width; x++) {
		for (size_t y = 0; y < height; y++) {
			unsigned char c = rand() % 256;
			data[x * channels + y * width * channels + 0] = c;
			data[x * channels + y * width * channels + 1] = c;
			data[x * channels + y * width * channels + 2] = c;
			data[x * channels + y * width * channels + 3] = 255;
		}
	}
#endif
}

void resize(int width, int height) {
	if (height == 0) height = 1;
	float aspect_rato = (float)width / (float)height;

	glm::mat4 pr_matrix = glm::ortho(-aspect_rato, aspect_rato, 1.0f, -1.0f);
	shader->projectionMatrix(pr_matrix);
	shader->useTexture(true);
	shader_single->projectionMatrix(pr_matrix);
	shader_single->useTexture(true);
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
	shader_single = new oe::graphics::SingleTextureShader();
	texture = new oe::graphics::Texture();
	texture->empty2D(64, 64);
	std::thread *worker = new std::thread([] {while (true) { update(); } });
	worker->detach();

	// matrices
	glm::mat4 pr = glm::ortho(-oe::graphics::Window::getAspect(), oe::graphics::Window::getAspect(), 1.0f, -1.0f);
	shader->projectionMatrix(pr);
	shader->useTexture(true);
	shader_single->projectionMatrix(pr);
	shader_single->useTexture(true);
	
	// start
	oe::utils::GameLoop::init(render, update, 1);

	// closing
	oe::graphics::Window::close();
	delete shader;
	delete renderer;
	delete font;

	return 0;
}
