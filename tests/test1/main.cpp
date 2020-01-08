#include <engine/engine.h>
#include <engine/internal_libs.h>
#include <engine/graphics/textureManager.h>
#include <engine/graphics/assets/singleTextureShader.h>

#include <string>



oe::graphics::SingleTextureShader* shader;
oe::graphics::Renderer* renderer;

float t = 0;
void render(float update_fraction) {
	oe::graphics::Window::clear();
	t += oe::utils::GameLoop::getFrameUpdateScale();

	// quads
	renderer->begin();
	renderer->clear();
	
	renderer->submit(glm::vec2(-0.5f,  0.5f), glm::vec2(0.4f), 0, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), oe::graphics::alignment::center_center, t);
	renderer->submit(glm::vec2( 0.5f,  0.5f), glm::vec2(0.4f), 0, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), oe::graphics::alignment::center_center, sin(t));
	renderer->submit(glm::vec2(0.5f, -0.5f), glm::vec2(0.4f), 0, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), oe::graphics::alignment::center_center, tan(t));
	renderer->submit(glm::vec2(-0.5f, -0.5f), glm::vec2(0.4f), 0, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f), oe::graphics::alignment::center_center, round(t));
	renderer->submit(glm::vec2(0.0f, 0.0f), glm::vec2(0.4f), 0, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), oe::graphics::alignment::center_center, t * 100.0f);

	renderer->end();
	renderer->render();
	
	oe::graphics::Window::update();
	oe::graphics::Window::input();
	if (oe::graphics::Window::shouldClose()) oe::utils::GameLoop::stop();
}

int n = 0;
void update() {
	if (n++%100==99) spdlog::info("FPS: " + std::to_string(oe::utils::GameLoop::getFPS()));
}

int main() {
	oe::init();

	oe::graphics::Window::init(900, 600, "Test 1 - Renderer", NULL);
	oe::graphics::Window::setClearColor(0.0f, 0.0f, 0.0f, 0.2f);
	oe::graphics::GL::setPolygonMode(0);
	renderer = new oe::graphics::Renderer(oe::graphics::types::dynamicrender, oe::graphics::types::staticrender, 5, nullptr);
	shader = new oe::graphics::SingleTextureShader();

	// pr matrix
	glm::mat4 pr = glm::ortho(-oe::graphics::Window::getAspect(), oe::graphics::Window::getAspect(), 1.0f, -1.0f);
	shader->projectionMatrix(pr);
	shader->useTexture(false);

	//oe::TextureManager::loadTextureAtlas("res/texture/atlas.png", 0, 16);
	oe::graphics::GL::setBackFaceCulling(false);
	oe::graphics::GL::setSwapInterval(0);
	oe::graphics::FrameBuffer::unbind();
	oe::graphics::Window::viewport();
	
	// start
	oe::utils::GameLoop::init(render, update, 100);

	// closing
	oe::graphics::Window::close();
	delete shader;
	delete renderer;

	return 0;
}
