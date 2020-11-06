#include <engine/include.hpp>

#include <string>
#include <thread>



constexpr size_t updates_per_second = 60;
constexpr float inverse_ups = 1.0f / updates_per_second;
int32_t entity_count;
b2World box2d_world(b2Vec2(0.0f, 9.81f));


oe::graphics::Window window;
oe::asset::DefaultShader* shader;
oe::graphics::SpritePack* pack;
oe::graphics::Sprite const* sprite;
oe::ecs::World* world;
std::vector<oe::ecs::Entity> entities;
oe::gui::GUI* gui_manager;
std::shared_ptr<oe::gui::TextPanel> text_label;



struct GenericScript : public oe::ecs::Behaviour
{
	b2Body* m_body{};

	void stop()
	{
		m_body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	}

	void on_init(b2BodyType body_type, const glm::vec2& pos, const glm::vec2& size, float angle, const oe::color& c, float density)
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
		quad.quad_holder->setColor(c);
		quad.quad_holder->setSize(size);
		quad.quad_holder->setSprite(sprite);
		quad.quad_holder->setRotationAlignment(oe::alignments::center_center);
	}

	void on_custom_update()
	{
		const auto& pos = m_body->GetPosition();
		if (pos.y > 20.0f)
			m_body->SetTransform(b2Vec2(pos.x, -20.0f), 0.0f);
		if (pos.y < -20.0f)
			m_body->SetTransform(b2Vec2(pos.x, 20.0f), 0.0f);
		if (pos.x > 20.0f * window->getAspect())
			m_body->SetTransform(b2Vec2(-20.0f * window->getAspect(), pos.y), 0.0f);
		if (pos.x < -20.0f * window->getAspect())
			m_body->SetTransform(b2Vec2(20.0f * window->getAspect(), pos.y), 0.0f);
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

	// rotor box
	auto box = world->create(); // world::create()
	auto motor_mid = oe::ecs::Entity(world); // or Entity constructor
	auto& generic_src_box = box.setScriptComponent<GenericScript>(b2BodyType::b2_dynamicBody, glm::vec2{ 10.0f, 15.0f }, glm::vec2{ 1.5f, 15.0f }, glm::quarter_pi<float>(), oe::color(random.randomVec3(0.0f, 1.0f), 1.0f), 0.1f);
	auto& generic_src_motor = motor_mid.setScriptComponent<GenericScript>(b2BodyType::b2_kinematicBody, glm::vec2{ 0.0f, 15.0f }, glm::vec2{ 1.0f, 1.0f }, glm::quarter_pi<float>(), oe::color(random.randomVec3(0.0f, 1.0f), 1.0f), 1.0f);
	motor_mid.setScriptComponent<MotorScript>(box);
	
	// ground box
	auto gbox = world->create(); // world::create()
	gbox.setScriptComponent<GenericScript>(b2BodyType::b2_staticBody, glm::vec2{ -15.0f, 0.0f }, glm::vec2{ 10.0f, 2.0f }, 0.0f, oe::color(random.randomVec3(0.0f, 1.0f), 1.0f), 0.1f);
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
	std::u32string perf_info = fmt::format(
		U"- frametime: {:3.3f} ms ({} fps)\n- updatetime: {:3.3f} ms ({} ups)",
		std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(gameloop.getFrametime()).count(),
		gameloop.getAverageFPS(),
		std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(gameloop.getUpdatetime<30>()).count(),
		gameloop.getAverageUPS<30>());
	text_label->text_panel_info.text = perf_info;
}

// update event <updates_per_second> times per second
void update()
{
	auto& random = oe::utils::Random::getSingleton();

	if(std::max(0, entity_count) > entities.size())
		for(size_t i = 0; i < std::max(0, entity_count) - entities.size(); i++)
		{
			glm::vec2 pos = { random.randomf(-2.0f, 2.0f), random.randomf(-30.0f, 5.0f) };
			glm::vec2 size = random.randomVec2(0.1f, 1.0f);
			auto entity = world->create();
			entity.setScriptComponent<GenericScript>(b2BodyType::b2_dynamicBody, pos, size, random.randomf(0.0f, glm::two_pi<float>()), oe::color(random.randomVec3(0.0f, 1.0f), 1.0f), 1.0f);
			entities.push_back(entity);
		}
	if(std::max(0, entity_count) < entities.size())
		for(size_t i = 0; i < entities.size() - std::max(0, entity_count); i++)
		{
			entities.back().destroy();
			entities.pop_back();
		}

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
		oe::gui::TextPanel::info_t tp_info;
		tp_info.font_size = 22;
		tp_info.widget_info.align_parent = oe::alignments::top_left;
		tp_info.widget_info.align_render = oe::alignments::top_left;
		tp_info.text = U"placeholder";
		text_label = gui_manager->create(tp_info);
	}
	{
		oe::gui::fSliderInput::info_t s_info;
		s_info.knob_size = { 6, 16 };
		s_info.widget_info.size = { 150, 16 };
		s_info.widget_info.align_parent = oe::alignments::top_left;
		s_info.widget_info.align_render = oe::alignments::top_left;
		s_info.widget_info.offset_position = { 0, 50 };
		s_info.slider_lcolor = oe::colors::red;
		s_info.slider_rcolor = oe::colors::green;
		s_info.linear_color = true;
		s_info.slider_sprite = pack->emptySprite();
		s_info.value_bounds = { -10.0f, 10.0f, };
		s_info.draw_value = true;
		auto slider = gui_manager->create(s_info);

		using event_t = oe::gui::fSliderInput::use_event_t;
		std::function<void(event_t)> e = [&](const event_t& e)
		{
			world->m_scene.view<std::unique_ptr<MotorScript>>().each([&e](std::unique_ptr<MotorScript>& src) {
				src->motor_joint->SetMotorSpeed(e.value);
			});
		};

		slider->create_event_cg().connect<event_t>(slider->m_dispatcher, e);
	}
	{
		oe::gui::iNumberInput::info_t n_info;
		n_info.widget_info.size = { 50, 20 };
		n_info.widget_info.align_parent = oe::alignments::top_left;
		n_info.widget_info.align_render = oe::alignments::top_left;
		n_info.widget_info.offset_position = { 0, 80 };
		n_info.initial_value = 200;
		n_info.interact_flags = oe::interact_type_flags::scroll | oe::interact_type_flags::cursor;
		auto count_input = gui_manager->create(n_info, entity_count);
	}
	{
		oe::gui::DecoratedButton::info_t b_info;
		b_info.button_info.size = { 50, 40 };
		b_info.button_info.align_parent = oe::alignments::top_left;
		b_info.button_info.align_render = oe::alignments::top_left;
		b_info.button_info.offset_position = { 0, 110 };
		b_info.text = U"R";
		auto reset_btn = gui_manager->create(b_info);

		reset_btn->create_event_cg().connect<oe::gui::Button::use_event_t>(reset_btn->m_dispatcher, [](const oe::gui::Button::use_event_t& e){
			for(size_t i = 0; i < entities.size(); i++)
				entities[i].getScriptComponent<GenericScript>().stop();
		});
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
	engine.swapInterval(1);
	engine.blending();

	// shader
	shader = new oe::asset::DefaultShader();

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
	engine_info.ignore_errors = true;
	engine_info.api = oe::graphics_api::OpenGL;
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

	// auto close ctest after 2 seconds
	std::thread ctest_close_thread;
	for (size_t i = 0; i < argc; i++)
		if(std::strcmp(argv[i], "--ctest") == 0)
		{
			ctest_close_thread = std::thread([](){
				// test for 2 seconds
				std::this_thread::sleep_for(std::chrono::seconds(2));

				// then stop the gameloop
				window->getGameloop().stop();
			});
			break;
		}
	
	window->getGameloop().start();

	if(ctest_close_thread.joinable())
		ctest_close_thread.join();

	return 0;
}