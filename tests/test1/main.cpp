#include <engine/engine.h>
#include <engine/internal_libs.h>
#include <engine/graphics/textureManager.h>
#include <engine/graphics/assets/singleTextureShader.h>

#include <string>



oe::graphics::SingleTextureShader* shader;
oe::graphics::Renderer* renderer;

float t = 0;
void render(float update_fraction) {
	t += oe::utils::GameLoop::getFrameUpdateScale();

	// clear framebuffer
	oe::graphics::Window::clearWindow();

	// begin submitting
	renderer->begin();
	renderer->clear();
	
	// submitting
	renderer->submit(glm::vec2(-0.5f,  0.5f), glm::vec2(0.4f), 0, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), oe::graphics::alignment::center_center, t);
	renderer->submit(glm::vec2( 0.5f,  0.5f), glm::vec2(0.4f), 0, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), oe::graphics::alignment::center_center, sin(t));
	renderer->submit(glm::vec2(0.5f, -0.5f), glm::vec2(0.4f), 0, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), oe::graphics::alignment::center_center, tan(t));
	renderer->submit(glm::vec2(-0.5f, -0.5f), glm::vec2(0.4f), 0, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f), oe::graphics::alignment::center_center, round(t));
	renderer->submit(glm::vec2(0.0f, 0.0f), glm::vec2(0.4f), 0, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), oe::graphics::alignment::center_center, t * 100.0f);

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
	spdlog::info("FPS: " + std::to_string(oe::utils::GameLoop::getFPS()));
}

void resize(int width, int height) {
	float aspect = oe::graphics::Window::aspect();
	glm::mat4 pr = glm::ortho(-aspect, aspect, 1.0f, -1.0f);
	shader->projectionMatrix(pr);
	shader->useTexture(false);
}

void keyboard(int key, int action) {
	if (action == OE_PRESS) {
		if (key == OE_KEY_ESCAPE) {
			oe::utils::GameLoop::stop();
		}
		else if (key == OE_KEY_ENTER) {
			oe::graphics::Window::setFullscreen(!oe::graphics::Window::getFullscreen());
		}
	}
}

int main() {
	// engine
	oe::init();

	// window
	oe::graphics::Window::WindowConfig window_config;
	window_config.title = "Test 1 - Renderer";
	window_config.multisamples = 4;
	window_config.opengl_debugmode = true;
	oe::graphics::Window::init(window_config);
	oe::graphics::Window::setResizeCallback(resize);
	oe::graphics::Window::setKeyboardCallback(keyboard);
	oe::graphics::GL::setBackFaceCulling(true);

	// drawing
	renderer = new oe::graphics::Renderer(oe::graphics::types::dynamicrender, oe::graphics::types::staticrender, 5, nullptr);
	shader = new oe::graphics::SingleTextureShader();

	// matrices
	resize(oe::graphics::Window::getSize().x, oe::graphics::Window::getSize().y);
	
	// start
	oe::utils::GameLoop::init(render, update, 1);

	// closing
	oe::graphics::Window::closeWindow();
	delete shader;
	delete renderer;

	return 0;
}
