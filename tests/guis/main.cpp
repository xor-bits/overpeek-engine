#include <engine/include.hpp>

#include <string>
#include <cmath>



oe::gui::GUI* gui;
std::shared_ptr<oe::gui::TextInput> textbox;
std::shared_ptr<oe::gui::TextPanel> textpanel;
std::shared_ptr<oe::gui::SpritePanel> box;
std::shared_ptr<oe::gui::List> list;
std::shared_ptr<oe::gui::Checkbox> checkbox;
std::shared_ptr<oe::gui::VecSlider<4>> quat_slider;
std::shared_ptr<oe::gui::Graph> graph_fps;
std::shared_ptr<oe::gui::Graph> graph_ups;
std::shared_ptr<oe::gui::ColorPicker> color_picker;
std::array<float, 200> perf_log_fps;
std::array<float, 50> perf_log_ups;

oe::graphics::Window window;
oe::assets::DefaultShader* shader_fill;
oe::assets::DefaultShader* shader_lines;
oe::graphics::PrimitiveRenderer renderer;
oe::graphics::SpritePack* pack;
const oe::graphics::Sprite* sprite;

glm::vec4 color = { 0.4f, 0.5f, 0.4f, 1.0f };
glm::quat cube_rotation;
constexpr bool graphs = true;


static glm::mat4 ml_matrix = glm::mat4(1.0f);
static oe::gui::TextPanelInfo tpi;
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
	const std::vector<oe::graphics::VertexData> box_vertices = 
	{
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
	};

	// submit
	renderer->begin();
	renderer->clear();
	renderer->submitVertex(box_vertices);
	renderer->end();
}

void cube()
{
	// shader and model matrix
	glm::vec4 quat_slider_val = quat_slider ? quat_slider->m_value : glm::vec4(0.0f);
	if (checkbox && checkbox->m_checkbox_info.initial)
	{
		cube_rotation = glm::angleAxis(quat_slider_val.w, glm::normalize(glm::vec3(quat_slider_val.x, quat_slider_val.y, quat_slider_val.z)));
	}
	else
	{
		if(list)
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
		else
		{
			cube_rotation = glm::angleAxis(-quat_slider_val.w, glm::normalize(glm::vec3(quat_slider_val.x, quat_slider_val.y, quat_slider_val.z)));
		}
		
	}
	ml_matrix = glm::mat4_cast(cube_rotation);

	// render
	shader_lines->bind();
	shader_lines->setModelMatrix(ml_matrix);
	shader_lines->setColor(color);
	renderer->render();
	shader_lines->unbind();
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
	
	shader_fill->bind();
	shader_fill->setTexture(false);
	shader_fill->setProjectionMatrix(pr_matrix);
	shader_fill->setViewMatrix(vw_matrix);

	shader_lines->bind();
	shader_lines->setTexture(false);
	shader_lines->setProjectionMatrix(pr_matrix);
	shader_lines->setViewMatrix(vw_matrix);
}

// update 30 times per second
void update_30(oe::UpdateEvent<30>)
{
	auto& gameloop = window->getGameloop(); 
	std::u32string str = fmt::format(
		U"frametime: {:3.3f} ms ({} fps) updatetime: {:3.3f} ms ({} ups)",
		std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(gameloop.getFrametime()).count(),
		gameloop.getAverageFPS(),
		std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(gameloop.getUpdatetime<30>()).count(),
		gameloop.getAverageUPS<30>());
	if(textpanel) textpanel->text_panel_info.text = str;

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
	if(!list) return;
	// list->add(new Checkpoint(quat));
}

// gui
void setup_gui()
{
	{
		oe::gui::SpritePanelInfo sprite_panel_info;
		sprite_panel_info.widget_info = { { 150, 150 }, { 0, 0 }, oe::alignments::bottom_left, oe::alignments::bottom_left };
		sprite_panel_info.sprite = sprite;
		box = gui->create<oe::gui::SpritePanel>(sprite_panel_info);
	}
	{
		oe::gui::TextInputInfo text_input_info;
		text_input_info.widget_info = { { 200, 80 }, { 0, 0 }, oe::alignments::bottom_right, oe::alignments::bottom_right };
		text_input_info.font_size = 14;
		text_input_info.sprite = pack->emptySprite();
		textbox = gui->create<oe::gui::TextInput>(text_input_info);
	}
	{
		oe::gui::VecSliderInfo<4> vecslider_info;
		vecslider_info.slider_info.widget_info = { { 400, 30 }, { 0, 0 }, oe::alignments::bottom_center, oe::alignments::bottom_center };
		vecslider_info.slider_info.slider_sprite = pack->emptySprite();
		vecslider_info.slider_info.knob_sprite = sprite;
		vecslider_info.slider_info.draw_value = true;
		vecslider_info.min_values = { -glm::pi<float>(), -1.0f, -1.0f, -1.0f };
		vecslider_info.max_values = { glm::pi<float>(), 1.0f, 1.0f, 1.0f };
		vecslider_info.initial_values = { 0.0f, 1.0f, 1.0f, 1.0f };
		quat_slider = gui->create<oe::gui::VecSlider<4>>(vecslider_info);
	}
	{
		oe::gui::CheckboxInfo ci;
		ci.widget_info = { { 24, 24 }, { 0, -35 }, oe::alignments::bottom_center, oe::alignments::bottom_center };
		ci.sprite = pack->emptySprite();
		checkbox = gui->create<oe::gui::Checkbox>(ci);

		{
			oe::gui::DecoratedButtonInfo button_info;
			button_info.button_info.widget_info = { { 60, 24 }, { 5, 0 }, oe::alignments::center_right, oe::alignments::center_left };
			button_info.sprite = pack->emptySprite();
			button_info.text = U"log";
			button_info.text_font_size = 18;
			auto button = checkbox->create<oe::gui::DecoratedButton>(button_info);

			auto callback_lambda = [&](const oe::gui::ButtonUseEvent& e) {
				if (e.action == oe::actions::release && e.button == oe::mouse_buttons::button_left) {
					glm::vec4 quat_slider_val = quat_slider->m_value;
					append_list(glm::angleAxis(quat_slider_val.w, glm::normalize(glm::vec3(quat_slider_val.x, quat_slider_val.y, quat_slider_val.z))));
				}
			};
			button->connect_listener<oe::gui::ButtonUseEvent, &decltype(callback_lambda)::operator()>(callback_lambda);
		}
	}
	{
		oe::gui::ColorPickerInfo color_picker_info;
		color_picker_info.initial_color = color;
		color_picker_info.widget_info = { { 200, 100 }, { 0, 0 }, oe::alignments::center_left, oe::alignments::center_left };
		color_picker_info.sprite = pack->emptySprite();
		color_picker_info.popup_color_picker_wheel = true;
		color_picker = gui->create<oe::gui::ColorPicker>(color_picker_info);

		auto picker_callback_lambda = [&](const oe::gui::ColorPickerUseEvent& e)
		{
			color = e.value;
		};
		color_picker->connect_listener<oe::gui::ColorPickerUseEvent, &decltype(picker_callback_lambda)::operator()>(picker_callback_lambda);
	}
	{
		oe::gui::TextPanelInfo text_panel_info;
		text_panel_info.widget_info = { { 0, 0 }, { 0, 0 }, oe::alignments::top_left, oe::alignments::top_left };
		text_panel_info.font_size = 20;
		text_panel_info.text = U"placeholder";
		text_panel_info.font_file = oe::utils::FontFile{ oe::default_full_font_path_bolditalic };
		/* text_panel_info.background_color = oe::colors::translucent_black; */
		textpanel = gui->create<oe::gui::TextPanel>(text_panel_info);

		if constexpr (graphs) {
			oe::gui::GraphInfo graph_info;
			graph_info.bg_panel_info.widget_info.size = { 200, 100 };
			graph_info.bg_panel_info.widget_info.offset_position = { 0, 5 };
			graph_info.bg_panel_info.widget_info.align_parent = oe::alignments::bottom_left;
			graph_info.bg_panel_info.widget_info.align_render = oe::alignments::top_left;
			graph_info.bg_panel_info.sprite = pack->emptySprite();
			graph_info.graph_color = oe::colors::green;
			graph_fps = textpanel->create<oe::gui::Graph>(graph_info);
			
			graph_info.bg_panel_info.widget_info.offset_position = { 205, 5 };
			graph_info.graph_color = oe::colors::blue;
			graph_ups = textpanel->create<oe::gui::Graph>(graph_info);
		}
	}
	if constexpr(false) {
		oe::gui::ListInfo list_info;
		list_info.widget_info = { { 200, 400 }, { 0, 0 }, oe::alignments::top_right, oe::alignments::top_right };
		list_info.sprite = pack->emptySprite();
		list_info.title = U"Loglist";
		list_info.title_height = 28;
		list = gui->create<oe::gui::List>(list_info);
	}

	tpi = {};
	tpi.font_size = 14;
	tpi.widget_info.align_parent = oe::alignments::top_left;
	tpi.widget_info.align_render = oe::alignments::top_left;
	tpi.text = U"placeholder";
	
	auto& random = oe::utils::Random::getSingleton();
	for(int i = 0; i < 5; i++) append_list(glm::angleAxis(random.randomf(-glm::pi<float>(), glm::pi<float>()), glm::normalize(random.randomVec3(-1.0f, 1.0f))));
}

int main()
{
	auto& engine = oe::Engine::getSingleton();

	// engine
	oe::EngineInfo engine_info = {};
	engine_info.api = oe::graphics_api::OpenGL;
	engine_info.debug_mode = false;
	engine_info.ignore_errors = false;
	engine.init(engine_info);

	// window
	oe::WindowInfo window_config = {};
	window_config.title = "GUIs";
	window_config.multisamples = 2;
	window = oe::graphics::Window(window_config);

	// connect events
	window->connect_listener<oe::ResizeEvent, &resize>();
	window->connect_listener<oe::RenderEvent, &render>();
	window->connect_listener<oe::UpdateEvent<30>, &update_30>();

	// instance settings
	engine.swapInterval(1);
	engine.culling(oe::culling_modes::neither);
	engine.blending(oe::modes::enable);

	// renderer
	oe::RendererInfo renderer_info = {};
	renderer_info.arrayRenderType = oe::types::dynamic_type;
	renderer_info.indexRenderType = oe::types::static_type;
	renderer_info.max_primitive_count = 6;
	renderer_info.staticVBOBuffer_data = nullptr;
	renderer = oe::graphics::PrimitiveRenderer(renderer_info);
	initCube();
	
	// shader
	shader_fill = new oe::assets::DefaultShader(oe::polygon_mode::fill);
	shader_lines = new oe::assets::DefaultShader(oe::polygon_mode::lines);

	// spritepack
	auto img = oe::assets::TextureSet::oe_logo_img;
	pack = new oe::graphics::SpritePack();
	sprite = pack->create(img);
	pack->construct();

	// gui
	gui = new oe::gui::GUI(window, oe::utils::FontFile{ oe::default_full_font_path_bold });
	setup_gui();

	// start
	window->getGameloop().start();

	// cleanup
	textbox.reset();
	textpanel.reset();
	box.reset();
	list.reset();
	checkbox.reset();
	quat_slider.reset();
	graph_fps.reset();
	graph_ups.reset();
	color_picker.reset();
	delete gui;
	delete pack;
	delete shader_fill;
	delete shader_lines;

	return 0;
}