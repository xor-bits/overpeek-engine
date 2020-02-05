#include <engine/engine.h>

#include <string>

#include "texture.png.h"



const oe::graphics::Sprite* sprite;
const oe::graphics::Sprite* sprite_white;
oe::graphics::Window* window;
oe::graphics::SpritePack* pack;
oe::graphics::Shader* shader;
oe::graphics::Renderer* renderer;

float t = 0;
void render(float update_fraction) {
	t += oe::utils::GameLoop::getFrameUpdateScale();

	// clear framebuffer
	window->clear();

	// begin submitting
	renderer->begin();
	renderer->clear();
	
	// submitting
	renderer->submit(glm::vec2(-0.5f,  0.5f), glm::vec2(0.4f), sprite_white, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), oe::alignments::center_center, t);
	renderer->submit(glm::vec2( 0.5f,  0.5f), glm::vec2(0.4f), sprite_white, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), oe::alignments::center_center, sin(t));
	renderer->submit(glm::vec2(0.5f, -0.5f),  glm::vec2(0.4f), sprite_white, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), oe::alignments::center_center, tan(t));
	renderer->submit(glm::vec2(-0.5f, -0.5f), glm::vec2(0.4f), sprite_white, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f), oe::alignments::center_center, round(t));
	renderer->submit(glm::vec2(0.0f, 0.0f),   glm::vec2(0.4f), sprite_white, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), oe::alignments::center_center, t * 100.0f);

	// stop submitting and render
	pack->bind();
	renderer->end();
	renderer->render();
	
	// swap buffers and poll events
	window->update();

	// check if needs to close
	if (window->shouldClose()) oe::utils::GameLoop::stop();
}

void update() {
	spdlog::info("FPS: " + std::to_string(oe::utils::GameLoop::getFPS()));
}

void resize(const glm::vec2& window_size) {
	float aspect = window->aspect();
	glm::mat4 pr = glm::ortho(-aspect, aspect, 1.0f, -1.0f);
	spdlog::info(aspect);
	shader->setUniformMat4("pr_matrix", pr);
	shader->setUniform1i("usetex", 1);
}

void keyboard(int key, int action, int mods) {
	if (action == oe::press) {
		if (key == oe::key_escape) {
			oe::utils::GameLoop::stop();
		}
		else if (key == oe::key_enter) {
			window->setFullscreen(!window->getFullscreen());
		}
	}
}

int main(int argc, char** argv) {
	// engine
	oe::EngineInfo engine_info = {};
	engine_info.api = oe::Vulkan;
	oe::Engine::init(engine_info);

	// window
	oe::WindowInfo window_info;
	window_info.title = "Test 1 - Renderer";
	window_info.multisamples = 4;
	window_info.debug_messages = true;

	window = oe::Engine::createWindow(window_info);
	window->setResizeCallback(resize);
	window->setKeyboardCallback(keyboard);
	
	// oe::graphics::GL::setCulling(oe::graphics::GL::culling::back);
	// oe::graphics::GL::setSwapInterval(1);
	// oe::graphics::GL::enableBlending();

	// drawing
	oe::RendererInfo renderer_info = {};
	renderer_info.arrayRenderType = oe::dynamicrender;
	renderer_info.indexRenderType = oe::dynamicrender;
	renderer_info.max_quad_count = 5;
	renderer_info.staticVBOBuffer_data = nullptr;

	renderer = oe::Engine::createRenderer(renderer_info);
	shader = oe::Engine::createShader(oe::graphics::SingleTextureShader::singleTextureShader());
	
	auto img = oe::utils::loadImageCopy(texture_png, 5, 5);
	pack = new oe::graphics::SpritePack();
	sprite = pack->addSprite(img);
	sprite_white = pack->empty_sprite();
	pack->construct();

	// matrices
	resize(window->getSize());
	
	// start
	oe::utils::GameLoop::init(render, update, 1);

	// closing
	delete pack;
	oe::Engine::destroyShader(shader);
	oe::Engine::destroyRenderer(renderer);
	oe::Engine::destroyWindow(window);

	return 0;
}
