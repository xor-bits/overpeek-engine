#include <engine/include.hpp>

// NOTE: this test does not follow the best practises
// You should already know by those globals
// Check out the hello-world test for better guide



constexpr size_t updates_per_second = 60;
constexpr float inverse_ups = 1.0f / updates_per_second;
constexpr oe::RasterizerInfo fill_rasterizer{ oe::modes::enable, oe::depth_functions::less_than_or_equal, oe::culling_modes::back, oe::polygon_mode::fill };
constexpr oe::RasterizerInfo line_rasterizer{ oe::modes::enable, oe::depth_functions::less_than_or_equal, oe::culling_modes::back, oe::polygon_mode::lines };
float sim_speed = 1.0f;
int32_t entity_count;
std::vector<oe::ecs::Entity> entities;
oe::ecs::World* world;
b2World box2d_world(b2Vec2(0.0f, 9.81f));


oe::graphics::Window window;
oe::asset::DefaultShader* shader;
oe::graphics::SpritePack* pack;
oe::graphics::Sprite const* sprite;
oe::gui::GUI* gui_manager;
std::shared_ptr<oe::gui::TextPanel> text_label;
std::shared_ptr<oe::gui::Graph> graph_fps;
std::shared_ptr<oe::gui::Graph> graph_ups;
std::array<float, 200> perf_log_fps;
std::array<float, 200> perf_log_ups;

constexpr uint16_t dynamic_category = 0b01;
constexpr uint16_t  static_category = 0b10;


// more correct than the fmod
constexpr float modulo(float x, float y)
{
	if(y == 0.0f)
		return x;

	return x - y * floor(x / y);
}

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
		// bodydef.bullet = (body_type == b2BodyType::b2_dynamicBody);
		bodydef.angle = angle;
		b2PolygonShape polygonshape = {};
		polygonshape.SetAsBox(size.x / 2.0f, size.y / 2.0f);
		b2FixtureDef fixturedef = {};
		fixturedef.density = density;
		fixturedef.friction = 1.0f;
		fixturedef.restitution = 0.1f;
		fixturedef.shape = &polygonshape;

		fixturedef.filter.categoryBits = dynamic_category;
		fixturedef.filter.maskBits = dynamic_category | static_category;
		
		m_body = box2d_world.CreateBody(&bodydef);
		m_body->CreateFixture(&fixturedef);
		
		auto& quad = setComponent<oe::ecs::QuadComponent>();
		quad.quad_holder = m_world->m_renderer.create();
		quad.quad_holder->setColor(c);
		quad.quad_holder->setSize(size);
		quad.quad_holder->setSprite(sprite);
		quad.quad_holder->setRotationAlignment(oe::alignments::center_center);
	}

	void on_cleanup() override
	{
		box2d_world.DestroyBody(m_body);
	}

	void on_custom_update()
	{
		constexpr float padding = 1.0f;
		constexpr float scale = 20.0f + padding;

		const auto& pos = m_body->GetPosition();
		const auto& rot = m_body->GetAngle();
		const float a = window->getAspect();
		m_body->SetTransform(b2Vec2(
			modulo(pos.x + scale * a, 2.0f * scale * a) - scale * a,
			modulo(pos.y + scale, 2.0f * scale) - scale), rot);
	}

	void on_custom_render(float update_fraction)
	{
		auto& quad = getComponent<oe::ecs::QuadComponent>();

		const float correction = update_fraction * inverse_ups * sim_speed;

		// rendering
		auto pos = m_body->GetPosition();
		auto rotation = m_body->GetAngle();
		const auto l_vel = m_body->GetLinearVelocity();
		const auto a_vel = m_body->GetAngularVelocity();
		pos.x += l_vel.x * correction;
		pos.y += l_vel.y * correction;
		rotation += a_vel * correction;

		quad.quad_holder->setPosition({ pos.x, pos.y });
		quad.quad_holder->setSprite(sprite);
		quad.quad_holder->setRotation(rotation);
	}
};

struct MotorScript : public oe::ecs::Behaviour
{
	b2RevoluteJoint* motor_joint{};
	b2MouseJoint* mouse_joint{};

	void on_init(const oe::ecs::Entity& attached_to, const oe::ecs::Entity& ground)
	{
		auto& generic_src_a = getScriptComponent<GenericScript>();
		auto& generic_src_b = attached_to.getScriptComponent<GenericScript>();
		auto& generic_src_g = ground.getScriptComponent<GenericScript>();

		generic_src_a.m_body->SetFixedRotation(true);

		{
			b2RevoluteJointDef jointDef = {};
			jointDef.bodyA = generic_src_a.m_body;
			jointDef.bodyB = generic_src_b.m_body;
			jointDef.enableMotor = true;
			jointDef.motorSpeed = 0.0f;
			jointDef.maxMotorTorque = std::numeric_limits<float>::max();

			motor_joint = static_cast<b2RevoluteJoint*>(box2d_world.CreateJoint(&jointDef));
		}
		{
			b2MouseJointDef jointDef = {};
			jointDef.bodyA = generic_src_g.m_body;
			jointDef.bodyB = generic_src_a.m_body;
			jointDef.target = generic_src_a.m_body->GetPosition();
			jointDef.maxForce = std::numeric_limits<float>::max();
			jointDef.frequencyHz = 1000.0f;
			jointDef.dampingRatio = 140.0f;

			mouse_joint = dynamic_cast<b2MouseJoint*>(box2d_world.CreateJoint(&jointDef));
		}
	}

	void on_custom_update()
	{
		auto body_a = mouse_joint->GetBodyA();
		auto body_b = mouse_joint->GetBodyB();
		body_a->SetAwake(true);
		body_b->SetAwake(true);

		mouse_joint->SetTarget({ window->getCursorTransformed().x * 20.0f, window->getCursorTransformed().y * 20.0f });
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
	auto& generic_src_motor = motor_mid.setScriptComponent<GenericScript>(b2BodyType::b2_dynamicBody, glm::vec2{ 0.0f, 15.0f }, glm::vec2{ 1.0f, 1.0f }, glm::quarter_pi<float>(), oe::color(random.randomVec3(0.0f, 1.0f), 1.0f), 1.0f);
	
	// ground box
	auto gbox = world->create(); // world::create()
	auto& generic_src_gbox = gbox.setScriptComponent<GenericScript>(b2BodyType::b2_staticBody, glm::vec2{ -15.0f, 0.0f }, glm::vec2{ 10.0f, 2.0f }, 0.0f, oe::color(random.randomVec3(0.0f, 1.0f), 1.0f), 0.1f);
	
	// motor script
	motor_mid.setScriptComponent<MotorScript>(box, gbox);
	
	// static filter
	b2Filter static_filter;
	static_filter.categoryBits = static_category;
	static_filter.maskBits = dynamic_category;
	generic_src_box.m_body->GetFixtureList()[0].SetFilterData(static_filter);
	generic_src_motor.m_body->GetFixtureList()[0].SetFilterData(static_filter);
	generic_src_gbox.m_body->GetFixtureList()[0].SetFilterData(static_filter);
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
	// the worst way to get first fill and then lines
	// doing this in the shader would be faster
	auto& engine = oe::Engine::getSingleton();
	// rp1 - fill
	shader->bind();
	shader->setTexture(true);
	shader->setColor(oe::colors::white);
	engine.setRasterizerInfo(fill_rasterizer);
	world->m_renderer.render();
	// rp2 - lines
	shader->setTexture(false);
	shader->setColor(oe::colors::black);
	engine.setRasterizerInfo(line_rasterizer);
	world->m_renderer.render();

	gui_manager->render();

}
// framebuffer resize
void resize(const oe::ResizeEvent& event)
{
	glm::mat4 pr_matrix = glm::ortho(-20.0f * event.aspect, 20.0f * event.aspect, 20.0f, -20.0f);
	shader->setProjectionMatrix(pr_matrix);
}

// update event 30 times per second
void update_30(oe::UpdateEvent<30>)
{
	auto& gameloop = window->getGameloop();
	std::u32string perf_info = fmt::format(
		U"- frametime: {:3.3f} ms ({} fps)\n- updatetime: {:3.3f} ms ({} ups)\n- BC: {}",
		std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(gameloop.getFrametime()).count(),
		gameloop.getAverageFPS(),
		std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(gameloop.getUpdatetime<updates_per_second>()).count(),
		gameloop.getAverageUPS<updates_per_second>(),
		box2d_world.GetBodyCount());
	text_label->text_panel_info.text = { perf_info, oe::colors::white };

	auto lambda_transform_gen = [](const oe::utils::PerfLogger& logger){
		return [&logger](std::chrono::nanoseconds ns)->float{
			auto cast_to_float_millis = [](auto dur){ return std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(dur); };
			return oe::utils::map(
				cast_to_float_millis(ns).count(),
				cast_to_float_millis(logger.m_min_time).count(),
				cast_to_float_millis(logger.m_max_time).count(),
				0.0f,
				1.0f
			);
		};
	};

	auto& fps_log = window->getGameloop().getFramePerfLogger();
	std::transform(fps_log.m_average_time.begin(), fps_log.m_average_time.begin() + perf_log_fps.size(), perf_log_fps.begin(), lambda_transform_gen(fps_log));
	std::rotate(perf_log_fps.begin(), perf_log_fps.begin() + (fps_log.m_total_count % perf_log_fps.size()), perf_log_fps.end());
	if (graph_fps) graph_fps->m_graph_info.graph_data = { perf_log_fps.data(), perf_log_fps.size() };

	auto& ups_log = window->getGameloop().getUpdatePerfLogger<updates_per_second>();
	std::transform(ups_log.m_average_time.begin(), ups_log.m_average_time.begin() + perf_log_ups.size(), perf_log_ups.begin(), lambda_transform_gen(ups_log));
	std::rotate(perf_log_ups.begin(), perf_log_ups.begin() + (ups_log.m_total_count % perf_log_ups.size()), perf_log_ups.end());
	if (graph_ups) graph_ups->m_graph_info.graph_data = { perf_log_ups.data(), perf_log_ups.size() };
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

	box2d_world.Step(inverse_ups * sim_speed, 8, 3);
}

void gui()
{
	gui_manager = new oe::gui::GUI(window);
	int32_t y = 75;
	{
		oe::gui::TextPanel::info_t tp_info;
		tp_info.widget_info.align_parent = oe::alignments::top_left;
		tp_info.widget_info.align_render = oe::alignments::top_left;
		tp_info.text = { U"placeholder", oe::colors::white };
		tp_info.text_options.size = 22;
		tp_info.text_options.outline_weight = 0.3f;
		text_label = gui_manager->create(tp_info);
	}
	{
		oe::gui::Graph::info_t graph_info;
		graph_info.bg_panel_info.widget_info.size = { 100, 50 };
		graph_info.bg_panel_info.widget_info.offset_position = { 0, y };
		graph_info.bg_panel_info.widget_info.align_parent = oe::alignments::top_left;
		graph_info.bg_panel_info.widget_info.align_render = oe::alignments::top_left;
		graph_info.bg_panel_info.sprite = pack->emptySprite();
		graph_info.bg_panel_info.color_tint = { 0.0f, 0.0f, 0.0f, 0.5f };
		graph_info.graph_color = oe::colors::green;
		graph_fps = gui_manager->create(graph_info);

		graph_info.bg_panel_info.widget_info.offset_position = { 105, y };
		graph_info.graph_color = oe::colors::blue;
		graph_ups = gui_manager->create(graph_info);
		
		y += 55;
	}
	{
		oe::gui::fSliderInput::info_t s_info;
		s_info.knob_size = { 6, 20 };
		s_info.widget_info.size = { 150, 20 };
		s_info.widget_info.align_parent = oe::alignments::top_left;
		s_info.widget_info.align_render = oe::alignments::top_left;
		s_info.widget_info.offset_position = { 0, y };
		s_info.slider_lcolor = oe::colors::red;
		s_info.slider_rcolor = oe::colors::green;
		s_info.linear_color = true;
		s_info.slider_sprite = pack->emptySprite();
		s_info.value_bounds = { -10.0f, 10.0f, };
		s_info.text_format = [](float v){ return fmt::format(U"motor speed: {:.2f}", v); };
		auto slider = gui_manager->create(s_info);

		y += 25;

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
		oe::gui::fSliderInput::info_t s_info;
		s_info.knob_size = { 6, 20 };
		s_info.widget_info.size = { 150, 20 };
		s_info.widget_info.align_parent = oe::alignments::top_left;
		s_info.widget_info.align_render = oe::alignments::top_left;
		s_info.widget_info.offset_position = { 0, y };
		s_info.slider_lcolor = oe::colors::dark_grey;
		s_info.slider_rcolor = oe::colors::dark_blue;
		s_info.slider_sprite = pack->emptySprite();
		s_info.value_bounds = { 0.0f, 2.0f, };
		s_info.text_format = [](float v){ return fmt::format(U"simulation speed: {:.2f}", v); };
		auto slider = gui_manager->create(s_info, sim_speed);

		y += 25;
	}
	{
		oe::gui::iNumberInput::info_t n_info;
		n_info.widget_info.size = { 80, 30 };
		n_info.widget_info.align_parent = oe::alignments::top_left;
		n_info.widget_info.align_render = oe::alignments::top_left;
		n_info.widget_info.offset_position = { 0, y };
		n_info.value_bounds = { 0, 1000 };
		n_info.interact_flags = oe::interact_type_flags::scroll | oe::interact_type_flags::cursor;
		n_info.text_format = [](int32_t v){ return fmt::format("entities: {}", v); };
		auto count_input = gui_manager->create(n_info, entity_count);

		y += 85;
	}
}

void init()
{
	auto& engine = oe::Engine::getSingleton();

	// connect events (this can also be done in (int main()))
	window->connect_listener<oe::ResizeEvent, &resize>();
	window->connect_listener<oe::RenderEvent, &render>();
	window->connect_listener<oe::UpdateEvent<30>, &update_30>();
	window->connect_listener<oe::UpdateEvent<updates_per_second>, &update>();

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

void cleanup()
{
	// closing
	text_label.reset();
	graph_fps.reset();
	graph_ups.reset();
	delete gui_manager;
	delete pack;
	delete shader;
	delete world;

	window.reset();
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
	window_info.multisamples = 0; // multisample will break the window transparency, possibly a bug with glfw 3.3.2?
	window_info.transparent = true;
	window_info.swap_interval = 1;
	// window_info.borderless = true;
	window = oe::graphics::Window(window_info);

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
	
	init();
	window->getGameloop().start();
	cleanup();

	if(ctest_close_thread.joinable())
		ctest_close_thread.join();

	return 0;
}