#include <engine/engine.h>

#include <string>



constexpr unsigned int updates_per_second = 60;
constexpr float inverse_ups = 1.0f / updates_per_second;

oe::graphics::SpriteShader* shader;
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
	oe::graphics::Window::clearWindow();

	// rendering
	shader->bind();
	pack->bind();
	renderer->clear();
	renderer->begin();
	registry.view<position>().each([&](position& pos) {
		renderer->submit(pos, { 0.2f, 0.2f }, sprite);
	});

	renderer->end();
	renderer->render();

	// swap buffers and poll events
	oe::graphics::Window::updateWindow();
	oe::graphics::Window::pollEvents();

	// check if needs to close
	if (oe::graphics::Window::windowShouldClose()) oe::utils::GameLoop::stop();
}

void resize(const glm::vec2& window_size) {
	float aspect = oe::graphics::Window::aspect();

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
	oe::init(argc, argv);
	ecsSetup(argc, argv);

	// window
	oe::graphics::Window::WindowConfig window_config;
	window_config.title = "entities";
	window_config.multisamples = 4;
	window_config.opengl_debugmode = true;
	oe::graphics::Window::init(window_config);
	oe::graphics::Window::setResizeCallback(resize);
	oe::graphics::GL::setSwapInterval(1);
	oe::graphics::GL::setCulling(oe::graphics::GL::culling::back);
	oe::graphics::GL::enableBlending();

	pack = new oe::graphics::SpritePack();
	sprite = pack->empty_sprite();
	pack->construct();

	// drawing
	renderer = new oe::graphics::Renderer(oe::graphics::types::dynamicrender, oe::graphics::types::staticrender, 600, nullptr);
	shader = new oe::graphics::SpriteShader();
	resize(oe::graphics::Window::getSize());

	// start
	oe::utils::GameLoop::init(render, update, updates_per_second);

	// closing
	oe::graphics::Window::closeWindow();
	delete renderer;
	delete shader;
	delete pack;

	return 0;
}
