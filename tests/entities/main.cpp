#include <engine/include.hpp>

#include <string>



constexpr unsigned int updates_per_second = 60;
constexpr float inverse_ups = 1.0f / updates_per_second;

const oe::graphics::Sprite* sprite;
oe::graphics::Window* window;
oe::graphics::SpritePack* pack;
oe::graphics::Renderer* renderer;
oe::assets::DefaultShader* shader;

const int entities = 200;
entt::registry entity_registry;
b2World box2d_world(b2Vec2(0.0f, 9.81f));
b2RevoluteJoint* motor_joint;


// <<<<----- box2d stuff
struct component_renderable {
	oe::graphics::Quad* quad;

	static void on_construct(entt::registry& reg, entt::entity e)
	{
	}

	static void on_destroy(entt::registry& reg, entt::entity e)
	{
		renderer->destroyQuad(reg.get<component_renderable>(e).quad);
	}
};

struct component_body {
	b2Body* body;

	static void on_destroy(entt::registry& reg, entt::entity e)
	{
		renderer->destroyQuad(reg.get<component_renderable>(e).quad);
	}
};

b2Body* box(bool is_static, const glm::vec2& pos, const glm::vec2& size, float angle, const glm::vec4& color, float density = 1.0f)
{
	b2BodyDef bodydef = {};
	b2FixtureDef fixturedef = {};
	b2PolygonShape polygonshape = {};
	bodydef.gravityScale = 1.0f;
	fixturedef.density = density;
	fixturedef.friction = 1.0f;
	fixturedef.restitution = 0.1f;

	bodydef.position = { pos.x, pos.y };
	bodydef.type = is_static ? b2BodyType::b2_staticBody : b2BodyType::b2_dynamicBody;
	bodydef.angle = angle;
	auto body = box2d_world.CreateBody(&bodydef);
	polygonshape.SetAsBox(size.x / 2.0f, size.y / 2.0f);
	fixturedef.shape = &polygonshape;
	body->CreateFixture(&fixturedef);
	auto entity = entity_registry.create();
	auto quad = renderer->createQuad();
	quad->setSize(size);
	quad->setRotationAlignment(oe::alignments::center_center);
	quad->setColor(color);
	entity_registry.assign<component_renderable>(entity, quad);
	entity_registry.assign<component_body>(entity, body);

	return body;
}

b2RevoluteJoint* joint(b2Body* a, b2Body* b)
{
	b2RevoluteJointDef jointDef = {};
	jointDef.bodyA = a;
	jointDef.bodyB = b;
	jointDef.enableMotor = true;
	jointDef.motorSpeed = 3.0f;
	jointDef.maxMotorTorque = 1e20f;
	
	return static_cast<b2RevoluteJoint*>(box2d_world.CreateJoint(&jointDef));
}
// box2d stuff ----->>>>

// scene setup
void setup() {
	auto& random = oe::utils::Random::getSingleton();

	entity_registry.on_construct<component_renderable>().connect<component_renderable::on_construct>();
	entity_registry.on_destroy<component_renderable>().connect<component_renderable::on_destroy>();
	
	// ground box
	auto body_0 = box(false, { 10.0f, 15.0f }, { 1.5f, 15.0f }, glm::quarter_pi<float>(), glm::vec4(random.randomVec3(0.0f, 1.0f), 1.0f), 0.1f);
	auto body_1 = box(true, { 0.0f, 15.0f }, { 1.0f, 1.0f }, glm::quarter_pi<float>(), glm::vec4(random.randomVec3(0.0f, 1.0f), 1.0f));
	motor_joint = joint(body_0, body_1);

	// random falling boxes
	for (auto i = 2; i < entities; ++i) {
		glm::vec2 pos = { random.randomf(-2.0f, 2.0f), random.randomf(-30.0f, 5.0f) };
		glm::vec2 size = random.randomVec2(0.1f, 1.0f);
		box(false, pos, size, random.randomf(0.0f, glm::two_pi<float>()), glm::vec4(random.randomVec3(0.0f, 1.0f), 1.0f));
	}

}

// render func
void render(float update_fraction) {
	window->clear(oe::colors::transparent);

	// submitting
	entity_registry.view<component_renderable, component_body>().each([&](entt::entity entity, component_renderable& renderable, component_body& body)
	{
		// rendering
		auto pos = body.body->GetPosition();
		auto vel = body.body->GetLinearVelocity();
		auto rotation = body.body->GetAngle();
		pos.x += (vel.x * update_fraction * inverse_ups);
		pos.y += (vel.y * update_fraction * inverse_ups);

		renderable.quad->setPosition({ pos.x, pos.y });
		renderable.quad->setSprite(sprite);
		renderable.quad->setRotation(rotation);
		renderable.quad->update();
	});

	// stop submitting and render
	shader->bind();
	renderer->render();
}

// framebuffer resize
void resize(const oe::ResizeEvent& event) {
	glm::mat4 pr_matrix = glm::ortho(-20.0f * event.aspect, 20.0f * event.aspect, 20.0f, -20.0f);
	shader->bind();
	shader->setProjectionMatrix(pr_matrix);
	shader->useTexture(true);
}

// update 2 times per second
void update_2() {
	auto& gameloop = window->getGameloop(); 
	spdlog::info("frametime: {:3.3f} ms ({} fps)", gameloop.getFrametimeMS(), gameloop.getAverageFPS());
}

// update 60 times per second
void update_60() {
	entity_registry.view<component_body>().each([&](entt::entity entity, component_body& body)
	{
		// rendering
		auto& pos = body.body->GetPosition();
		if (pos.y >= 20.0f) { body.body->SetTransform(b2Vec2(0.0f, -20.0f), 0.0f); body.body->SetLinearVelocity(b2Vec2(0.0f, 0.0f)); }
	});

	float motor_joint_speed = std::pow(window->getCursorTransformed().x * 2.0f, 3.0f);
	motor_joint->SetMotorSpeed(motor_joint_speed);

	box2d_world.Step(inverse_ups, 8, 8);
}



int main(int argc, char* argv[]) {
	auto& engine = oe::Engine::getSingleton();

	// engine
	oe::EngineInfo engine_info = {};
	engine_info.api = oe::graphics_api::OpenGL;
	engine_info.debug_messages = false;
	engine.init(engine_info);

	// window
	oe::WindowInfo window_info;
	window_info.title = "Entities";
	window_info.multisamples = 4;
	window_info.transparent = true;
	// window_info.borderless = true;
	window = engine.createWindow(window_info);

	// connect events
	window->connect_listener<oe::ResizeEvent, &resize>();
	window->connect_render_listener<&render>();
	window->connect_update_listener<2, &update_2>();
	window->connect_update_listener<updates_per_second, &update_60>();

	// instance settings
	engine.culling(oe::culling_modes::back);
	engine.swapInterval(0);
	engine.blending();

	// renderer
	oe::RendererInfo renderer_info = {};
	renderer_info.arrayRenderType = oe::types::dynamicrender;
	renderer_info.indexRenderType = oe::types::staticrender;
	renderer_info.max_primitive_count = entities;
	renderer_info.staticVBOBuffer_data = nullptr;
	renderer = engine.createRenderer(renderer_info);
	setup();

	// shader
	shader = new oe::assets::DefaultShader();

	// sprites
	pack = new oe::graphics::SpritePack();
	sprite = pack->empty_sprite();
	pack->construct();

	// start
	window->getGameloop().start();

	// closing
	delete pack;
	delete shader;
	engine.destroyRenderer(renderer);
	engine.destroyWindow(window);

	return 0;
}