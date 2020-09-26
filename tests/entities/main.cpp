#include <engine/include.hpp>

#include <string>
#include <thread>



constexpr size_t updates_per_second = 60;
constexpr float inverse_ups = 1.0f / updates_per_second;
constexpr size_t entities = 200;
b2World box2d_world(b2Vec2(0.0f, 9.81f));

oe::graphics::Window window;
oe::assets::DefaultShader* shader;
oe::graphics::SpritePack* pack;
oe::graphics::Sprite const* sprite;
oe::ecs::World* world;
oe::gui::GUI* gui_manager;
std::shared_ptr<oe::gui::TextPanel> text_label;



struct GenericScript : public oe::ecs::Behaviour
{
	b2Body* m_body{};

	void on_init(b2BodyType body_type, const glm::vec2& pos, const glm::vec2& size, float angle, const glm::vec4& color, float density)
	{
		b2BodyDef bodydef = {};
		bodydef.gravityScale = 1.0f;
		bodydef.position = { pos.x, pos.y };
		bodydef.type = body_type;
		bodydef.angle = angle;
		b2PolygonShape polygonshape = {};
		polygonshape.SetAsBox(size.x / 2.0f, size.y / 2.0f);
		b2FixtureDef fixturedef = {};
		fixturedef.density = density;
		fixturedef.friction = 1.0f;
		fixturedef.restitution = 0.1f;
		fixturedef.shape = &polygonshape;
		
		m_body = box2d_world.CreateBody(&bodydef);
		m_body->CreateFixture(&fixturedef);
		
		auto& quad = setComponent<oe::ecs::QuadComponent>();
		quad.quad_holder = m_world->m_renderer.create();
		quad.quad_holder->setColor(color);
		quad.quad_holder->setSize(size);
		quad.quad_holder->setSprite(sprite);
		quad.quad_holder->setRotationAlignment(oe::alignments::center_center);
	}

	void on_custom_update()
	{
		auto& pos = m_body->GetPosition();
		if (pos.y >= 20.0f) { m_body->SetTransform(b2Vec2(0.0f, -20.0f), 0.0f); m_body->SetLinearVelocity(b2Vec2(0.0f, 0.0f)); }
	}

	void on_custom_render(float update_fraction)
	{
		auto& quad = getComponent<oe::ecs::QuadComponent>();

		// rendering
		auto pos = m_body->GetPosition();
		auto vel = m_body->GetLinearVelocity();
		auto rotation = m_body->GetAngle();
		pos.x += (vel.x * update_fraction * inverse_ups);
		pos.y += (vel.y * update_fraction * inverse_ups);

		quad.quad_holder->setPosition({ pos.x, pos.y });
		quad.quad_holder->setSprite(sprite);
		quad.quad_holder->setRotation(rotation);
		quad.quad_holder->update();
	}
};

struct MotorScript : public oe::ecs::Behaviour
{
	b2RevoluteJoint* motor_joint{};

	void on_init(const oe::ecs::Entity& attached_to)
	{
		auto& generic_src_a = getScriptComponent<GenericScript>();
		auto& generic_src_b = attached_to.getScriptComponent<GenericScript>();

		b2RevoluteJointDef jointDef = {};
		jointDef.bodyA = generic_src_a.m_body;
		jointDef.bodyB = generic_src_b.m_body;
		jointDef.enableMotor = true;
		jointDef.motorSpeed = 0.0f;
		jointDef.maxMotorTorque = std::numeric_limits<float>::max();

		motor_joint = static_cast<b2RevoluteJoint*>(box2d_world.CreateJoint(&jointDef));
	}

	void on_custom_update()
	{
		auto& generic_src = getScriptComponent<GenericScript>();

		auto force = window->getCursorTransformed() - glm::vec2{ generic_src.m_body->GetPosition().x, generic_src.m_body->GetPosition().y } / 20.0f;
		force *= 500.0f;
		generic_src.m_body->SetLinearVelocity({ force.x, force.y });
	}
};

// scene setup
void setup()
{
	auto& random = oe::utils::Random::getSingleton();

	// ground box
	auto box = world->create(); // world::create()
	auto motor_mid = oe::ecs::Entity(world); // or Entity constructor
	auto& generic_src_box = box.setScriptComponent<GenericScript>(b2BodyType::b2_dynamicBody, glm::vec2{ 10.0f, 15.0f }, glm::vec2{ 1.5f, 15.0f }, glm::quarter_pi<float>(), glm::vec4(random.randomVec3(0.0f, 1.0f), 1.0f), 0.1f);
	auto& generic_src_motor = motor_mid.setScriptComponent<GenericScript>(b2BodyType::b2_kinematicBody, glm::vec2{ 0.0f, 15.0f }, glm::vec2{ 1.0f, 1.0f }, glm::quarter_pi<float>(), glm::vec4(random.randomVec3(0.0f, 1.0f), 1.0f), 1.0f);
	motor_mid.setScriptComponent<MotorScript>(box);

	// random falling boxes
	for (auto i = 2; i < entities; ++i) {
		glm::vec2 pos = { random.randomf(-2.0f, 2.0f), random.randomf(-30.0f, 5.0f) };
		glm::vec2 size = random.randomVec2(0.1f, 1.0f);
		auto entity = world->create();
		entity.setScriptComponent<GenericScript>(b2BodyType::b2_dynamicBody, pos, size, random.randomf(0.0f, glm::two_pi<float>()), glm::vec4(random.randomVec3(0.0f, 1.0f), 1.0f), 1.0f);
	}
}

// render event
void render(oe::RenderEvent)
{
	window->clear(oe::colors::transparent);

	// submitting
	float update_fraction = window->getGameloop().getUpdateLag<updates_per_second>();
	world->m_scene.view<std::unique_ptr<GenericScript>>().each([update_fraction](std::unique_ptr<GenericScript>& src) {
		src->on_custom_render(update_fraction);
	});
	
	// stop submitting and render
	shader->bind();
	world->m_renderer.render();
	shader->unbind();

	gui_manager->render();

}
// framebuffer resize
void resize(const oe::ResizeEvent& event)
{
	glm::mat4 pr_matrix = glm::ortho(-20.0f * event.aspect, 20.0f * event.aspect, 20.0f, -20.0f);
	shader->bind();
	shader->setProjectionMatrix(pr_matrix);
	shader->setTexture(true);
}

// update event 30 times per second
void update_30(oe::UpdateEvent<30>)
{
	auto& gameloop = window->getGameloop();
	std::u32string perf_info = fmt::format(U"- frametime: {:3.3f} ms ({} fps)\n- updatetime: {:3.3f} ms ({} ups)", gameloop.getFrametimeMS(), gameloop.getAverageFPS(), gameloop.getUpdatetimeMS<updates_per_second>(), gameloop.getAverageUPS<updates_per_second>());
	text_label->text_panel_info.text = perf_info;
}

// update event <updates_per_second> times per second
void update()
{
	world->m_scene.view<std::unique_ptr<GenericScript>>().each([](std::unique_ptr<GenericScript>& src) {
		src->on_custom_update();
	});
	world->m_scene.view<std::unique_ptr<MotorScript>>().each([](std::unique_ptr<MotorScript>& src) {
		src->on_custom_update();
	});

	box2d_world.Step(inverse_ups, 8, 3);
}

void gui()
{
	gui_manager = new oe::gui::GUI(window);
	{
		oe::gui::SliderInfo s_info;
		s_info.widget_info.size = { 250, 30 };
		s_info.widget_info.align_parent = oe::alignments::top_left;
		s_info.widget_info.align_render = oe::alignments::top_left;
		s_info.widget_info.offset_position = { 0, 50 };
		s_info.slider_lcolor = oe::colors::red;
		s_info.slider_rcolor = oe::colors::green;
		s_info.slider_sprite = pack->emptySprite();
		s_info.value_bounds = { -10.0f, 10.0f, };
		auto slider = gui_manager->create<oe::gui::Slider>(s_info);
		auto callback_lambda = [&](const oe::gui::SliderUseEvent& e)
		{
			world->m_scene.view<std::unique_ptr<MotorScript>>().each([&e](std::unique_ptr<MotorScript>& src) {
				src->motor_joint->SetMotorSpeed(e.value);
			});
		};
		slider->connect_listener<oe::gui::SliderUseEvent, &decltype(callback_lambda)::operator()>(callback_lambda);
	}
	{
		oe::gui::TextPanelInfo tp_info;
		tp_info.font_size = 22;
		tp_info.widget_info.align_parent = oe::alignments::top_left;
		tp_info.widget_info.align_render = oe::alignments::top_left;
		tp_info.text = U"placeholder";
		text_label = gui_manager->create<oe::gui::TextPanel>(tp_info);
	}
}

void init(oe::InitEvent)
{
	auto& engine = oe::Engine::getSingleton();
	engine.depth(oe::depth_functions::always);

	// connect events (this can also be done in (int main()))
	window->connect_listener<oe::ResizeEvent, &resize>();
	window->connect_listener<oe::RenderEvent, &render>();
	window->connect_listener<oe::UpdateEvent<30>, &update_30>();
	window->connect_listener<oe::UpdateEvent<updates_per_second>, &update>();

	// instance settings
	engine.culling(oe::culling_modes::back);
	engine.swapInterval(0);
	engine.blending();

	// shader
	shader = new oe::assets::DefaultShader();

	// sprites
	pack = new oe::graphics::SpritePack();
	sprite = pack->emptySprite();
	pack->construct();

	gui();

	// entities
	world = new oe::ecs::World();
	setup();
}

void cleanup(oe::CleanupEvent)
{
	// closing
	text_label.reset();
	delete gui_manager;
	delete pack;
	delete shader;
	delete world;
}

int main(int argc, char* argv[])
{
	auto& engine = oe::Engine::getSingleton();

	// engine
	oe::EngineInfo engine_info = {};
	engine_info.api = oe::graphics_api::OpenGL;
	engine_info.debug_mode = false;
	engine.init(engine_info);

	// window
	oe::WindowInfo window_info;
	window_info.title = "Entities";
	window_info.multisamples = 4;
	window_info.transparent = true;
	// window_info.borderless = true;
	window = oe::graphics::Window(window_info);
	window->connect_listener<oe::InitEvent, &init>();
	window->connect_listener<oe::CleanupEvent, &cleanup>();
	
	window->getGameloop().start();
	return 0;
}