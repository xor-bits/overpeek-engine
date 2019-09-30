#include "../overpeek-engine/engine.h"
#include "../overpeek-engine/internal_libs.h"
#include "../overpeek-engine/graphics/textureManager.h"
#include "../overpeek-engine/graphics/assets/singleTextureShader.h"

#include <Windows.h>
#include <string>



oe::Shader* shader;
oe::Renderer *renderer;
oe::GameLoop *gameloop;

float t = 0;
void render(float corrector) {
	oe::Window::clear();
	t += 0.001f;

	// quad
	renderer->clear();

	renderer->begin();
	renderer->submit(glm::vec2(-0.5f, -0.5f), glm::vec2(1.0f, 1.0f), 0, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), oe::centerCenter, t);
	renderer->end();

	renderer->draw();
	
	oe::Window::update();
	oe::Window::input();
	if (oe::Window::shouldClose()) gameloop->stop();
}

int n = 0;
void update() {
	if (n++%100==99) spdlog::info("FPS: " + std::to_string(gameloop->getFPS()));
}

int main() {
	oe::init();

	oe::Window::init(900, 600, "overpeek-engine", WINDOW_GL_DEBUG);
	gameloop = new oe::GameLoop();
	renderer = new oe::Renderer(oe::types::dynamicDraw, oe::types::staticDraw, 1, nullptr);
	shader = new oe::SingleTextureShader();

	// pr matrix
	glm::mat4 pr = glm::ortho(-oe::Window::getAspect(), oe::Window::getAspect(), 1.0f, -1.0f);
	shader->setUniformMat4("pr_matrix", pr);

	//oe::TextureManager::loadTextureAtlas("res/texture/atlas.png", 0, 16);
	oe::Window::setClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	oe::GL::setBackFaceCulling(false);
	oe::GL::setSwapInterval(0);
	oe::FrameBuffer::unbind();
	oe::Window::viewport();
	
	// start
	gameloop->start(render, update, 100);

	// closing
	oe::Window::close();
	delete shader;
	delete gameloop;
	delete renderer;

	return 0;
}