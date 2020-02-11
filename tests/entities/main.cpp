#include <engine/engine.hpp>

#include <string>



constexpr unsigned int updates_per_second = 60;
constexpr float inverse_ups = 1.0f / updates_per_second;

oe::graphics::Instance* instance;
oe::graphics::Window* window;
oe::graphics::DefaultShader* shader;
const oe::graphics::Sprite* sprite;
oe::graphics::SpritePack* pack;
oe::graphics::Renderer* renderer;
entt::registry registry;



struct position : public glm::vec2 {};
struct velocity : public glm::vec2 {};

void update(entt::registry& registry) {
	registry.view<velocity>().each([](velocity& vel) {
		// vel.value = { 0.0f, 0.0f };
	});
}

void update(float delta_time, entt::registry& registry) {
	registry.view<position, velocity>().each([&](position& pos, velocity& vel) {
		pos += vel * delta_time;
	});
}

void ecsSetup(int argc, char* argv[]) {
	for (auto i = 0; i < 100; ++i) {
		glm::vec2 pos = oe::utils::randomVec2(-17.5f, 17.5);
		glm::vec2 vel = oe::utils::randomVec2(-5.0f, 5.0);

		auto entity = registry.create();
		registry.assign<position>(entity, pos);
		if (i % 2 == 0) { registry.assign<velocity>(entity, vel); }
	}
}

void render(float update_fraction) {
	// clear framebuffer
	window->clear();

	// rendering
	shader->bind();
	pack->bind();
	renderer->clear();
	renderer->begin();
	registry.view<position>().each([&](position& pos) {
		// rendering
		renderer->submit(pos, { 0.2f, 0.2f }, sprite);
	});

	renderer->end();
	renderer->render();

	// swap buffers and poll events
	window->update();

	// check if needs to close
	if (window->shouldClose()) oe::utils::GameLoop::stop();
}

void resize(const glm::vec2& window_size) {
	float aspect = window->aspect();

	shader->bind();
	shader->useTexture(false);
	glm::mat4 pr_matrix = glm::ortho(-20.0f * aspect, 20.0f * aspect, 20.0f, -20.0f);
	shader->projectionMatrix(pr_matrix);
}

void update() {
	update(inverse_ups, registry);
	update(registry);
}

int main(int argc, char* argv[]) {
	// engine
	oe::EngineInfo engine_info = {};
	engine_info.api = oe::graphics_api::OpenGL;
	oe::Engine::init(engine_info);
	ecsSetup(argc, argv);

	// instance
	oe::InstanceInfo instance_info = {};
	instance_info.debug_messages = true;
	// instance_info.favored_gpu_vulkan = oe::gpu::dedicated;
	instance = oe::Engine::createInstance(instance_info);

	// window
	oe::WindowInfo window_info = {};
	window_info.title = "entities";
	window_info.multisamples = 4;
	window = instance->createWindow(window_info);
	window->setResizeCallback(resize);

	// instance settings
	instance->swapInterval(1);
	instance->culling(oe::culling_modes::back);

	pack = new oe::graphics::SpritePack(instance);
	sprite = pack->empty_sprite();
	pack->construct();

	// renderer
	oe::RendererInfo renderer_info = {};
	renderer_info.arrayRenderType = oe::types::dynamicrender;
	renderer_info.indexRenderType = oe::types::staticrender;
	renderer_info.max_quad_count = 6;
	renderer_info.staticVBOBuffer_data = nullptr;
	renderer = instance->createRenderer(renderer_info);

	// shader
	shader = new oe::graphics::DefaultShader(instance);

	// start
	oe::utils::GameLoop::init(render, update, updates_per_second);

	// closing
	delete shader;
	delete pack;
	instance->destroyRenderer(renderer);
	instance->destroyWindow(window);

	return 0;
}
