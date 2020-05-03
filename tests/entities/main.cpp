#include <engine/include.hpp>

#include <string>



constexpr unsigned int updates_per_second = 60;
constexpr float inverse_ups = 1.0f / updates_per_second;

const oe::graphics::Sprite* sprite;
oe::graphics::Window* window;
oe::graphics::SpritePack* pack;
oe::graphics::Renderer* renderer;
oe::assets::DefaultShader* shader;
entt::registry registry;
const int entities = 1000;
std::unordered_map<entt::entity, oe::graphics::Quad*> quads;



struct position : public glm::vec2 {};
struct velocity : public glm::vec2 {};

void ecsSetup(int argc, char* argv[]) {
	for (auto i = 0; i < entities; ++i) {

		auto& random = oe::utils::Random::getSingleton();
		glm::vec2 pos = random.randomVec2(-17.5f, 17.5);
		glm::vec2 vel = random.randomVec2(-5.0f, 5.0);

		auto entity = registry.create();
		registry.assign<position>(entity, pos);
		registry.assign<velocity>(entity, vel);
		quads.insert(std::make_pair(entity, renderer->createQuad()));
	}
}

void render(float update_fraction) {
	// submitting
	registry.view<position>().each([&](entt::entity entity, position& pos) {
		auto quad = quads[entity];

		// rendering
		quad->setPosition(pos);
		quad->setSize({ 0.2f, 0.2f });
		quad->setSprite(sprite);
		quad->update();
	});

	// stop submitting and render
	shader->bind();
	renderer->render();
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
}

int main(int argc, char* argv[]) {
	auto& engine = oe::Engine::getSingleton();

	// engine
	oe::EngineInfo engine_info = {};
	engine_info.api = oe::graphics_api::OpenGL;
	engine_info.debug_messages = true;
	engine.init(engine_info);

	// window
	oe::WindowInfo window_info;
	window_info.title = "Test 1 - Renderer";
	window_info.multisamples = 4;
	window_info.resize_callback = resize;
	window_info.render_callback = render;
	window_info.update_callback = update;
	window = engine.createWindow(window_info);

	// instance settings
	engine.culling(oe::culling_modes::back);
	engine.swapInterval(1);
	engine.blending();

	// renderer
	oe::RendererInfo renderer_info = {};
	renderer_info.arrayRenderType = oe::types::dynamicrender;
	renderer_info.indexRenderType = oe::types::staticrender;
	renderer_info.max_primitive_count = entities;
	renderer_info.staticVBOBuffer_data = nullptr;
	renderer = engine.createRenderer(renderer_info);
	ecsSetup(argc, argv);

	// shader
	shader = new oe::assets::DefaultShader();

	// sprites
	pack = new oe::graphics::SpritePack();
	sprite = pack->empty_sprite();
	pack->construct();

	// start
	resize(window->getSize());
	window->getGameloop().start(60);

	// closing
	delete pack;
	delete shader;
	engine.destroyRenderer(renderer);
	engine.destroyWindow(window);

	return 0;
}
