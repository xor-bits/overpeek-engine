#include "../overpeek-engine/engine.h"
#include "../overpeek-engine/internal_libs.h"
#include "../overpeek-engine/graphics/textureManager.h"
#include "../overpeek-engine/graphics/assets/singleTextureShader.h"

#include <Windows.h>
#include <string>



oe::SingleTextureShader* shader;
oe::Renderer *renderer;

float t = 0;
void render(float corrector) {
	oe::Window::clear();
	t += oe::GameLoop::getFrameUpdateScale();

	// quad
	renderer->clear();

	renderer->begin();
	renderer->submit(glm::vec2(-0.5f,  0.5f), glm::vec2(0.4f), 0, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), oe::alignment::center_center, t);
	renderer->submit(glm::vec2( 0.5f,  0.5f), glm::vec2(0.4f), 0, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), oe::alignment::center_center, sin(t));
	renderer->submit(glm::vec2( 0.5f, -0.5f), glm::vec2(0.4f), 0, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), oe::alignment::center_center, tan(t));
	renderer->submit(glm::vec2(-0.5f, -0.5f), glm::vec2(0.4f), 0, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f), oe::alignment::center_center, round(t));
	renderer->submit(glm::vec2( 0.0f,  0.0f), glm::vec2(0.4f), 0, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), oe::alignment::center_center, t * 100.0f);
	renderer->end();

	renderer->render();
	
	oe::Window::update();
	oe::Window::input();
	if (oe::Window::shouldClose()) oe::GameLoop::stop();
}

int n = 0;
void update() {
	if (n++%100==99) spdlog::info("FPS: " + std::to_string(oe::GameLoop::getFPS()));
}

int main() {
	oe::init();

	oe::Window::init(900, 600, "overpeek-engine", WINDOW_GL_DEBUG | WINDOW_MULTISAMPLE_X2 | WINDOW_TRANSPARENT);
	oe::Window::setClearColor(0.0f, 0.0f, 0.0f, 0.2f);
	oe::GL::setPolygonMode(0);
	renderer = new oe::Renderer(oe::types::dynamicrender, oe::types::staticrender, 5, nullptr);
	shader = new oe::SingleTextureShader();

	// pr matrix
	glm::mat4 pr = glm::ortho(-oe::Window::getAspect(), oe::Window::getAspect(), 1.0f, -1.0f);
	shader->projectionMatrix(pr);
	shader->useTexture(false);

	//oe::TextureManager::loadTextureAtlas("res/texture/atlas.png", 0, 16);
	oe::GL::setBackFaceCulling(false);
	oe::GL::setSwapInterval(0);
	oe::FrameBuffer::unbind();
	oe::Window::viewport();
	
	// start
	oe::GameLoop::init(render, update, 100);

	// closing
	oe::Window::close();
	delete shader;
	delete renderer;

	return 0;
}