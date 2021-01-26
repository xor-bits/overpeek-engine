#include <engine/include.hpp>

// NOTE: this test does not follow the best practises
// You should already know by those globals
// Check out the hello-world test for better guide



oe::gui::GUI* gui;
std::shared_ptr<oe::gui::BasicTextInput<char32_t>> textbox;
std::shared_ptr<oe::gui::TextPanel> textpanel;
std::shared_ptr<oe::gui::SpritePanel> box;
/* std::shared_ptr<oe::gui::List> list; */
std::shared_ptr<oe::gui::Checkbox> checkbox;
std::shared_ptr<oe::gui::Vec<oe::gui::SliderInput, 4>> quat_slider;
std::shared_ptr<oe::gui::Graph> graph_fps;
std::shared_ptr<oe::gui::Graph> graph_ups;
std::array<float, 200> perf_log_fps;
std::array<float, 200> perf_log_ups;

oe::graphics::Window window;
constexpr oe::RasterizerInfo line_rasterizer{ oe::modes::enable, oe::depth_functions::less_than_or_equal, oe::culling_modes::neither, oe::polygon_mode::lines };
oe::asset::DefaultShader* shader;
oe::graphics::PrimitiveRenderer renderer;

oe::graphics::Sprite sprite_logo;
oe::graphics::Sprite sprite_empty;

oe::color color = { 0.4f, 0.5f, 0.4f, 1.0f };
glm::quat cube_rotation;
constexpr bool graphs = true;


static glm::mat4 ml_matrix = glm::mat4(1.0f);
static oe::gui::TextPanel::info_t tpi;
class Checkpoint : public oe::gui::TextPanel
{
public:
	glm::quat quaternion;

	Checkpoint(oe::gui::Widget* parent, oe::gui::GUI& gui_manager, const glm::quat& quat)
		: TextPanel(parent, gui_manager, tpi)
	{
		quaternion = quat;
		// text_panel_info.text = fmt::format(U"{:.1f}", quaternion);
	}
};

// render cube
void initCube()
{
	// vertices
	constexpr static std::array<oe::graphics::VertexData, 24> box_vertices = 
	{{
		// front
		{ { -1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ { -1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ {  1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ {  1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		
		// back
		{ { -1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ {  1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		
		// top
		{ { -1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ { -1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ {  1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ {  1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		
		// bottom
		{ { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ { -1.0f, -1.0f,  1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ {  1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ {  1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		
		// left
		{ { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ { -1.0f, -1.0f,  1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ { -1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ { -1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		
		// right
		{ {  1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ {  1.0f, -1.0f,  1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ {  1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ {  1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	}};

	// submit
	renderer->begin();
	renderer->clear();
	renderer->submitVertex(box_vertices);
	renderer->end();
}

void cube()
{
	// shader and model matrix
	const glm::vec4 quat_slider_val = quat_slider ? quat_slider->m_value : glm::vec4(0.0f);
	
	if (checkbox && checkbox->m_value)
	{
		cube_rotation = glm::angleAxis(quat_slider_val.x, glm::normalize(glm::vec3(quat_slider_val.y, quat_slider_val.z, quat_slider_val.w)));
	}
	else
	{
		/* if(list)
		{
			auto points = list->get();
			size_t list_len = points.size();
			if (list_len == 0)
			{
				// nothing
			}
			else if (list_len == 1)
			{
				// single point
				cube_rotation = reinterpret_cast<Checkpoint*>(points.at(0))->quaternion;
			}
			else
			{
				// lerp
				float t = oe::utils::Clock::getSingleton().getSessionMillisecond() / 500.0f;
				t = (sin(t) * 0.5f + 0.5f) * (list_len - 1);
				t = ::fmodf(t, static_cast<float>(list_len - 1));

				size_t index = t;
				float modt = ::fmodf(t, 1.0f);
				
				const glm::quat& a = reinterpret_cast<Checkpoint*>(points.at(index))->quaternion;
				const glm::quat& b = reinterpret_cast<Checkpoint*>(points.at(index + 1))->quaternion;

				cube_rotation = glm::mix(a, b, modt);
			}
		}
		else */
		{
			cube_rotation = glm::angleAxis(-quat_slider_val.x, glm::normalize(glm::vec3(quat_slider_val.y, quat_slider_val.z, quat_slider_val.w)));
		}
		
	}
	ml_matrix = glm::mat4_cast(cube_rotation);

	// render
	oe::Engine::getSingleton().setRasterizerInfo(line_rasterizer);
	shader->bind();
	shader->setModelMatrix(ml_matrix);
	shader->setColor(color);
	renderer->render();
}

// render event
void render(oe::RenderEvent)
{
	cube();

	// gui
	gui->render();
}

// framebuffer resize
void resize(const oe::ResizeEvent& event)
{
	if (event.framebuffer_size == glm::uvec2{ 0, 1 }) return;

	glm::mat4 pr_matrix = glm::perspectiveFov(30.0f, (float)event.framebuffer_size.x, (float)event.framebuffer_size.y, 0.0f, 1000.0f);
	glm::mat4 vw_matrix = glm::lookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
	shader->bind();
	shader->setTexture(false);
	shader->setProjectionMatrix(pr_matrix);
	shader->setViewMatrix(vw_matrix);
}

// update 30 times per second
void update_30(oe::UpdateEvent<30>)
{
	auto& gameloop = window->getGameloop(); 
	std::u32string str = fmt::format(
		U"- frametime: {:3.3f} ms ({} fps)\n- updatetime: {:3.3f} ms ({} ups)",
		std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(gameloop.getFrametime()).count(),
		gameloop.getAverageFPS(),
		std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(gameloop.getUpdatetime<30>()).count(),
		gameloop.getAverageUPS<30>());
	if(textpanel)
		textpanel->text_panel_info.text = { str, oe::colors::white };

	if constexpr (!graphs) return;

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

	auto& ups_log = window->getGameloop().getUpdatePerfLogger<30>();
	std::transform(ups_log.m_average_time.begin(), ups_log.m_average_time.begin() + perf_log_ups.size(), perf_log_ups.begin(), lambda_transform_gen(ups_log));
	std::rotate(perf_log_ups.begin(), perf_log_ups.begin() + (ups_log.m_total_count % perf_log_ups.size()), perf_log_ups.end());
	if (graph_ups) graph_ups->m_graph_info.graph_data = { perf_log_ups.data(), perf_log_ups.size() };
}

void append_list(const glm::quat& quat)
{
	/* if(!list) return; */
	// list->add(new Checkpoint(quat));
}

// gui
void setup_gui()
{
	{
		oe::gui::SpritePanel::info_t sprite_panel_info;
		sprite_panel_info.widget_info.pixel_size = { 150, 150 };
		sprite_panel_info.widget_info.pixel_origon_offset = { 0, 0 };
		sprite_panel_info.widget_info.fract_origon_offset = oe::alignments::bottom_left;
		sprite_panel_info.widget_info.fract_render_offset = oe::alignments::bottom_left;
		sprite_panel_info.sprite = &sprite_logo;
		box = gui->create(sprite_panel_info);
	}
	{
		oe::gui::u32TextInput::info_t text_input_info;
		text_input_info.initial_value =
UR"(wwwwwwwwwwww
WWWWWWWWWW
yyyyyyyyyyyyy
|||||||||||||||||||)";
		text_input_info.widget_info.pixel_size = { 200, 80 };
		text_input_info.widget_info.pixel_origon_offset = { 0, 0 };
		text_input_info.widget_info.fract_origon_offset = oe::alignments::center_right;
		text_input_info.widget_info.fract_render_offset = oe::alignments::center_right;
		text_input_info.text_options.pixel_res(16);
		text_input_info.text_options.align = oe::alignments::center_center;
		text_input_info.sprite = &sprite_empty;
		textbox = gui->create(text_input_info);
	}
	{
		oe::gui::Vec<oe::gui::fSliderInput, 4>::info_t vecslider_info;
		vecslider_info.widget_info.pixel_size = { 400, 30 };
		// vecslider_info.widget_info.fract_size = { 0.5f, 0.0f };
		vecslider_info.widget_info.pixel_origon_offset = { 0, 0 };
		vecslider_info.widget_info.fract_origon_offset = oe::alignments::bottom_center;
		vecslider_info.widget_info.fract_render_offset = oe::alignments::bottom_center;
		oe::gui::fSliderInput::info_t common;
		common.slider_sprite = &sprite_empty;
		common.value_bounds = { -1.0f, 1.0f };
		common.initial_value = 1.0f;
		common.text_options.pixel_res(16);
		common.text_options.align = oe::alignments::center_center;
		vecslider_info.common.fill(common);
		vecslider_info.common[0].value_bounds *= glm::pi<float>();
		vecslider_info.common[0].initial_value = 0.0f;
		quat_slider = gui->create(vecslider_info);
	}
	{
		oe::gui::Checkbox::info_t ci;
		ci.widget_info.pixel_size = { 24, 24 };
		ci.widget_info.pixel_origon_offset = { 0, -35 };
		ci.widget_info.fract_origon_offset = oe::alignments::bottom_center;
		ci.widget_info.fract_render_offset = oe::alignments::bottom_center;
		ci.sprite = &sprite_empty;
		checkbox = gui->create(ci);

		{
			oe::gui::DecoratedButton::info_t button_info;
			button_info.button_info.pixel_size = { 60, 24 };
			button_info.button_info.pixel_origon_offset = { 5, 0 };
			button_info.button_info.fract_origon_offset = oe::alignments::center_right;
			button_info.button_info.fract_render_offset = oe::alignments::center_left;
			button_info.sprite = &sprite_empty;
			button_info.text = { U"log", oe::colors::white };
			button_info.text_options.pixel_res(20);
			button_info.text_options.align = oe::alignments::center_center;
			auto button = checkbox->create(button_info);

			button->create_event_cg().connect<oe::gui::ButtonUseEvent>(button->m_dispatcher, [&](const oe::gui::ButtonUseEvent& e) {
				if (e.action == oe::actions::release && e.button == oe::mouse_buttons::button_left) {
					glm::vec4 quat_slider_val = quat_slider->m_value;
					append_list(glm::angleAxis(quat_slider_val.w, glm::normalize(glm::vec3(quat_slider_val.x, quat_slider_val.y, quat_slider_val.z))));
				}
			});
		}
	}
	{ // color picker 1
		oe::gui::ColorInput::info_t color_picker_info;
		color_picker_info.widget_info.pixel_size = { 200, 120 };
		color_picker_info.widget_info.pixel_origon_offset = { 0, 35 };
		color_picker_info.widget_info.fract_origon_offset = oe::alignments::center_left;
		color_picker_info.widget_info.fract_render_offset = oe::alignments::center_left;
		color_picker_info.sprite = &sprite_empty;
		color_picker_info.popup_color_picker = true;
		color_picker_info.primary_input = oe::gui::input_type::slider;
		color_picker_info.text_options.pixel_res(16);
		color_picker_info.text_options.align = oe::alignments::center_center;
		color_picker_info.initial_color = color;
		gui->create(color_picker_info, color);
	}
	{ // color picker 2
		oe::gui::ColorInput::info_t color_picker_info;
		color_picker_info.widget_info.pixel_size = { 200, 20 };
		color_picker_info.widget_info.pixel_origon_offset = { 0, -40 };
		color_picker_info.widget_info.fract_origon_offset = oe::alignments::center_left;
		color_picker_info.widget_info.fract_render_offset = oe::alignments::center_left;
		color_picker_info.sprite = &sprite_empty;
		color_picker_info.popup_color_picker = true;
		color_picker_info.primary_input = oe::gui::input_type::dragger;
		color_picker_info.text_options.pixel_res(12);
		color_picker_info.text_options.align = oe::alignments::center_center;
		color_picker_info.initial_color = color;
		gui->create(color_picker_info, color);
	}
	{ // color picker 3
		oe::gui::ColorInput::info_t color_picker_info;
		color_picker_info.widget_info.pixel_size = { 20, 20 };
		color_picker_info.widget_info.pixel_origon_offset = { 0, -65 };
		color_picker_info.widget_info.fract_origon_offset = oe::alignments::center_left;
		color_picker_info.widget_info.fract_render_offset = oe::alignments::center_left;
		color_picker_info.sprite = &sprite_empty;
		color_picker_info.popup_color_picker = true;
		color_picker_info.primary_input = oe::gui::input_type::none;
		color_picker_info.initial_color = color;
		gui->create(color_picker_info, color);
	}
	{
		oe::gui::BasicNumberInput<uint32_t>::info_t number_input_info;
		number_input_info.widget_info.pixel_size = { 50, 20 };
		number_input_info.widget_info.pixel_origon_offset = { 0, 0 };
		number_input_info.widget_info.fract_origon_offset = oe::alignments::top_center;
		number_input_info.widget_info.fract_render_offset = oe::alignments::top_center;
		number_input_info.initial_value = 42;
		number_input_info.value_bounds = { std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max() };
		number_input_info.sprite = &sprite_empty;
		number_input_info.text_options.pixel_res(16);
		number_input_info.text_options.align = oe::alignments::center_center;
		gui->create(number_input_info);
	}
	{
		oe::gui::TextPanel::info_t text_panel_info;
		text_panel_info.widget_info.pixel_size = { 0, 0 };
		text_panel_info.widget_info.pixel_origon_offset = { 0, 0 };
		text_panel_info.widget_info.fract_origon_offset = oe::alignments::top_left;
		text_panel_info.widget_info.fract_render_offset = oe::alignments::top_left;
		text_panel_info.text = { U"placeholder", oe::colors::white };
		text_panel_info.text_options.font = oe::utils::FontFile{ oe::asset::Fonts::roboto_italic() };
		text_panel_info.text_options.pixel_res(24);
		/* text_panel_info.background_color = oe::colors::translucent_black; */
		textpanel = gui->create(text_panel_info);

		if constexpr (graphs) {
			oe::gui::Graph::info_t graph_info;
			graph_info.bg_panel_info.widget_info.pixel_size = { 200, 100 };
			graph_info.bg_panel_info.widget_info.pixel_origon_offset = { 0, 5 };
			graph_info.bg_panel_info.widget_info.fract_origon_offset = oe::alignments::bottom_left;
			graph_info.bg_panel_info.widget_info.fract_render_offset = oe::alignments::top_left;
			graph_info.bg_panel_info.sprite = &sprite_empty;
			graph_info.bg_panel_info.color_tint = oe::colors::translucent_black;
			graph_info.graph_color = oe::colors::green;
			graph_fps = textpanel->create(graph_info);
			
			graph_info.bg_panel_info.widget_info.pixel_origon_offset = { 205, 5 };
			graph_info.graph_color = oe::colors::blue;
			graph_ups = textpanel->create(graph_info);
		}
	}
	/* if constexpr(false) {
		oe::gui::ListInfo list_info;
		list_info.widget_info = { { 200, 400 }, { 0, 0 }, oe::alignments::top_right, oe::alignments::top_right };
		list_info.sprite = &sprite_empty;
		list_info.title = U"Loglist";
		list_info.title_height = 28;
		list = gui->create<oe::gui::List>(list_info);
	} */

	tpi = {};
	tpi.text_options.pixel_res(20);
	tpi.text_options.align = oe::alignments::center_center;
	tpi.widget_info.fract_origon_offset = oe::alignments::top_left;
	tpi.widget_info.fract_render_offset = oe::alignments::top_left;
	tpi.text = { U"placeholder", oe::colors::white };
	
	auto& random = oe::utils::Random::getSingleton();
	for(int i = 0; i < 5; i++) append_list(glm::angleAxis(random.randomf(-glm::pi<float>(), glm::pi<float>()), glm::normalize(random.randomVec3(-1.0f, 1.0f))));
}

int main(int argc, char** argv)
{
	auto& engine = oe::Engine::getSingleton();

	// engine
	oe::EngineInfo engine_info = {};
	engine_info.api = oe::graphics_api::OpenGL;
	engine.init(engine_info);

	// window
	oe::WindowInfo window_config = {};
	window_config.title = "GUIs";
	window_config.multisamples = 2;
	window_config.swap_interval = 0;
	window = oe::graphics::Window(window_config);

	// connect events
	window->connect_listener<oe::ResizeEvent, &resize>();
	window->connect_listener<oe::RenderEvent, &render>();
	window->connect_listener<oe::UpdateEvent<30>, &update_30>();

	// renderer
	oe::RendererInfo renderer_info = {};
	renderer_info.arrayRenderType = oe::types::dynamic_type;
	renderer_info.indexRenderType = oe::types::static_type;
	renderer_info.max_primitive_count = 6;
	renderer_info.staticVBOBuffer_data = nullptr;
	renderer = oe::graphics::PrimitiveRenderer(renderer_info);
	initCube();
	
	// shader
	shader = new oe::asset::DefaultShader();

	// sprites
	sprite_logo = oe::asset::TextureSet::sprites().at("logo");
	sprite_empty = oe::asset::TextureSet::sprites().at("empty");

	// gui
	gui = new oe::gui::GUI(window, oe::utils::FontFile{ oe::asset::Fonts::roboto_regular() });
	setup_gui();

	update_30({});

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

	// start
	window->getGameloop().start();

	if(ctest_close_thread.joinable())
		ctest_close_thread.join();

	// cleanup
	textbox.reset();
	textpanel.reset();
	box.reset();
	/* list.reset(); */
	checkbox.reset();
	quat_slider.reset();
	graph_fps.reset();
	graph_ups.reset();
	delete gui;
	delete shader;

	renderer.reset();
	window.reset();

	return 0;
}