#include <engine/engine.hpp>

#include <string>



constexpr unsigned int updates_per_second = 60;
constexpr float inverse_ups = 1.0f / updates_per_second;

const oe::graphics::Sprite* sprite;
oe::graphics::Instance* instance;
oe::graphics::Window* window;
oe::graphics::SpritePack* pack;
oe::graphics::Renderer* renderer;
oe::assets::DefaultShader* shader;
entt::registry registry;



struct position : public glm::vec2 {};
struct velocity : public glm::vec2 {};

void ecsSetup(int argc, char* argv[]) {
	for (auto i = 0; i < 100; ++i) {
		glm::vec2 pos = oe::utils::Random::getSingleton().randomVec2(-17.5f, 17.5);
		glm::vec2 vel = oe::utils::Random::getSingleton().randomVec2(-5.0f, 5.0);

		auto entity = registry.create();
		registry.assign<position>(entity, pos);
		if (i % 2 == 0) { registry.assign<velocity>(entity, vel); }
	}
}

void render(float update_fraction) {
	// clear framebuffer
	window->clear();

	// begin submitting
	renderer->begin();
	renderer->clear();

	// submitting
	registry.view<position>().each([&](position& pos) {
		// rendering
		renderer->submit(pos, { 0.2f, 0.2f }, sprite);
		});

	// stop submitting and render
	pack->bind();
	shader->bind();
	renderer->end();
	renderer->render();

	// swap buffers and poll events
	window->update();

	// check if needs to close
	if (window->shouldClose()) oe::utils::GameLoop::getSingleton().stop();
}

void resize(const glm::vec2& window_size) {
	float aspect = window->aspect();

	glm::mat4 pr_matrix = glm::ortho(-20.0f * aspect, 20.0f * aspect, 20.0f, -20.0f);
	shader->bind();
	shader->setProjectionMatrix(pr_matrix);
	shader->useTexture(true);
}

void update() {
	registry.view<position, velocity>().each([&](position& pos, velocity& vel) {
		pos += vel * inverse_ups;
		});
	registry.view<velocity>().each([](velocity& vel) {
		// vel.value = { 0.0f, 0.0f };
	});
}

int main(int argc, char* argv[]) {
	// engine
	oe::EngineInfo engine_info = {};
	engine_info.api = oe::graphics_api::OpenGL;
	oe::Engine::getSingleton().init(engine_info);
	ecsSetup(argc, argv);

	// instance
	oe::InstanceInfo instance_info = {};
	instance_info.debug_messages = true;
	// instance_info.favored_gpu_vulkan = oe::gpu::dedicated;
	instance = oe::Engine::getSingleton().createInstance(instance_info);

	// window
	oe::WindowInfo window_info;
	window_info.title = "Test 1 - Renderer";
	window_info.multisamples = 4;
	window_info.resize_callback = resize;
	window = instance->createWindow(window_info);

	// instance settings
	instance->culling(oe::culling_modes::back);
	instance->swapInterval(1);
	instance->blending();

	// renderer
	oe::RendererInfo renderer_info = {};
	renderer_info.arrayRenderType = oe::types::dynamicrender;
	renderer_info.indexRenderType = oe::types::staticrender;
	renderer_info.max_quad_count = 100;
	renderer_info.staticVBOBuffer_data = nullptr;
	renderer = window->createRenderer(renderer_info);

	// shader
	shader = new oe::assets::DefaultShader(window);

	// sprites
	pack = new oe::graphics::SpritePack(window);
	sprite = pack->empty_sprite();
	pack->construct();

	// start
	resize(window->getSize());
	oe::utils::GameLoop::getSingleton().start(render, update, 60);

	// closing
	delete pack;
	delete shader;
	window->destroyRenderer(renderer);
	instance->destroyWindow(window);

	return 0;
}
