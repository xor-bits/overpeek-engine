#include "../overpeek-engine/engine.h"
#include "../overpeek-engine/internal_libs.h"
#include "../overpeek-engine/graphics/textureManager.h"
#include "../overpeek-engine/graphics/assets/singleTextureShader.h"

#include <Windows.h>
#include <string>



oe::SingleTextureShader* shader;
oe::Renderer* renderer_0;
oe::Renderer* renderer_1;

float t = 0;
void render(float update_fraction) {
	oe::Window::clear();
	t += oe::GameLoop::getFrameUpdateScale();

	// quads
	/* scramble test */

	renderer_0->begin();
	renderer_0->clear();
	renderer_1->begin();
	renderer_1->clear();
	
	// renderer 0 submits
	renderer_0->submit(glm::vec2(-0.5f,  0.5f), glm::vec2(0.4f), 0, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), oe::alignment::center_center, t);
	renderer_0->submit(glm::vec2( 0.5f,  0.5f), glm::vec2(0.4f), 0, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), oe::alignment::center_center, sin(t));

	// renderer 1 submits
	renderer_1->submit(glm::vec2(0.5f, -0.5f), glm::vec2(0.4f), 0, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), oe::alignment::center_center, tan(t));
	renderer_1->submit(glm::vec2(-0.5f, -0.5f), glm::vec2(0.4f), 0, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f), oe::alignment::center_center, round(t));
	renderer_1->submit(glm::vec2(0.0f, 0.0f), glm::vec2(0.4f), 0, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), oe::alignment::center_center, t * 100.0f);

	renderer_0->end();
	renderer_0->render();
	renderer_1->end();
	//renderer_1->render();
	
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
	renderer_0 = new oe::Renderer(oe::types::dynamicrender, oe::types::staticrender, 5, nullptr);
	renderer_1 = new oe::Renderer(oe::types::dynamicrender, oe::types::staticrender, 5, nullptr);
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
	delete renderer_0;
	delete renderer_1;

	return 0;
}