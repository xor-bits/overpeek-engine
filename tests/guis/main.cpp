#include <engine/include.hpp>

#include <string>



oe::gui::GUI* gui;
oe::gui::TextInput* textbox;
oe::gui::TextPanel* textpanel;
oe::gui::SpritePanel* box;
oe::gui::List* list;
oe::gui::Checkbox* checkbox;
oe::gui::VecSlider<4>* quat_slider;
oe::gui::Graph* graph_fps;
oe::gui::Graph* graph_ups;
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


static glm::mat4 ml_matrix = glm::mat4(1.0f);
static oe::gui::TextPanelInfo tpi;
class Checkpoint : public oe::gui::TextPanel
{
public:
	glm::quat quaternion;

	Checkpoint(const glm::quat& quat)
		: TextPanel(tpi)
	{
		quaternion = quat;
		text_panel_info.text = fmt::format(U"{:.1f}", quaternion);
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
	if (checkbox && checkbox->m_checkbox_info.initial)
	{
		glm::vec4 quat_slider_val = quat_slider->getGLM();
		cube_rotation = glm::angleAxis(quat_slider_val.w, glm::normalize(glm::vec3(quat_slider_val.x, quat_slider_val.y, quat_slider_val.z)));
	}
	else
	{
		if(!list) return;
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
			t = std::fmodf(t, static_cast<float>(list_len - 1));

			size_t index = t;
			float modt = std::fmodf(t, 1.0f);
			
			const glm::quat& a = reinterpret_cast<Checkpoint*>(points.at(index))->quaternion;
			const glm::quat& b = reinterpret_cast<Checkpoint*>(points.at(index + 1))->quaternion;

			cube_rotation = glm::mix(a, b, modt);
		}
	}
	ml_matrix = glm::mat4_cast(cube_rotation);

	// render
	shader_lines->bind();
	shader_lines->setModelMatrix(ml_matrix);
	shader_lines->setColor(color);
	renderer->render();
}

// render event
void render(float update_fraction) {
	cube();

	// gui
	gui->render();
}

// framebuffer resize
void resize(const oe::ResizeEvent& event) {
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

template <typename to>
struct transform_cast
{
	to inmin, inmax;
	transform_cast(to mi, to mx) : inmin(mi), inmax(mx) {}

	template <typename from>
	to operator()(const from& x) const
	{
		return oe::utils::map(static_cast<to>(x), inmin, inmax, static_cast<to>(0.0f), static_cast<to>(1.0f));
	}
};

// update 30 times per second
void update_30() {
	auto& gameloop = window->getGameloop(); 
	std::u32string str = fmt::format(U"frametime: {:3.3f} ms ({} fps) updatetime: {:3.3f} ms ({} ups)", gameloop.getFrametimeMS(), gameloop.getAverageFPS(), gameloop.getUpdatetimeMS<30>(), gameloop.getAverageUPS<30>());
	if(textpanel) textpanel->text_panel_info.text = str;

	auto& fps_log = window->getGameloop().getPerfLoggerFPS();
	std::transform(fps_log.m_average_time.begin(), fps_log.m_average_time.begin() + perf_log_fps.size(), perf_log_fps.begin(), transform_cast<float>(fps_log.m_min_time, fps_log.m_max_time));
	std::rotate(perf_log_fps.begin(), perf_log_fps.begin() + (fps_log.m_total_count % perf_log_fps.size()), perf_log_fps.end());
	graph_fps->m_graph_info.graph_data = { perf_log_fps.data(), perf_log_fps.size() };

	auto& ups_log = window->getGameloop().getPerfLoggerUPS<30>();
	std::transform(ups_log.m_average_time.begin(), ups_log.m_average_time.begin() + perf_log_ups.size(), perf_log_ups.begin(), transform_cast<float>(ups_log.m_min_time, ups_log.m_max_time));
	std::rotate(perf_log_ups.begin(), perf_log_ups.begin() + (ups_log.m_total_count % perf_log_ups.size()), perf_log_ups.end());
	graph_ups->m_graph_info.graph_data = { perf_log_ups.data(), perf_log_ups.size() };
}

void append_list(const glm::quat& quat)
{
	if(!list) return;
	list->add(new Checkpoint(quat));
}

// gui
void setup_gui() {
	{
		oe::gui::SpritePanelInfo sprite_panel_info;
		sprite_panel_info.widget_info = { { 150, 150 }, { 0, 0 }, oe::alignments::bottom_left, oe::alignments::bottom_left };
		sprite_panel_info.sprite = sprite;
		box = new oe::gui::SpritePanel(sprite_panel_info);
		gui->addSubWidget(box);
	}
	{
		oe::gui::TextInputInfo text_input_info;
		text_input_info.widget_info = { { 200, 80 }, { 0, 0 }, oe::alignments::bottom_right, oe::alignments::bottom_right };
		text_input_info.font_size = 14;
		text_input_info.sprite = pack->emptySprite();
		textbox = new oe::gui::TextInput(text_input_info);
		gui->addSubWidget(textbox);
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
		quat_slider = new oe::gui::VecSlider<4>(vecslider_info);
		gui->addSubWidget(quat_slider);
	}
	{
		oe::gui::CheckboxInfo ci;
		ci.widget_info = { { 24, 24 }, { 0, -35 }, oe::alignments::bottom_center, oe::alignments::bottom_center };
		ci.sprite = pack->emptySprite();
		checkbox = new oe::gui::Checkbox(ci);
		gui->addSubWidget(checkbox);

		{
			oe::gui::DecoratedButtonInfo button_info;
			button_info.button_info.widget_info = { { 60, 24 }, { 5, 0 }, oe::alignments::center_right, oe::alignments::center_left };
			button_info.sprite = pack->emptySprite();
			button_info.text = U"log";
			button_info.text_font_size = 18;
			auto button = new oe::gui::DecoratedButton(button_info);
			checkbox->addSubWidget(button);

			auto callback_lambda = [&](const oe::gui::ButtonUseEvent& e) {
				if (e.action == oe::actions::release && e.button == oe::mouse_buttons::button_left) {
					glm::vec4 quat_slider_val = quat_slider->getGLM();
					append_list(glm::angleAxis(quat_slider_val.w, glm::normalize(glm::vec3(quat_slider_val.x, quat_slider_val.y, quat_slider_val.z))));
				}
			};
			button->connect_listener<oe::gui::ButtonUseEvent, &decltype(callback_lambda)::operator()>(callback_lambda);
		}
	}
	{
		oe::gui::ColorPickerInfo color_picker_info;
		color_picker_info.widget_info = { { 200, 100 }, { 0, 0 }, oe::alignments::center_left, oe::alignments::center_left };
		color_picker_info.initial_color = color;
		color_picker_info.sprite = pack->emptySprite();
		auto color_picker = new oe::gui::ColorPicker(color_picker_info);
		gui->addSubWidget(color_picker);

		auto callback_lambda = [&](const oe::gui::ColorPickerUseEvent& e)
		{
			color = e.value;
		};
		color_picker->connect_listener<oe::gui::ColorPickerUseEvent, &decltype(callback_lambda)::operator()>(callback_lambda);
	}
	{
		oe::gui::TextPanelInfo text_panel_info;
		text_panel_info.widget_info = { { 0, 0 }, { 0, 0 }, oe::alignments::top_left, oe::alignments::top_left };
		text_panel_info.font_size = 20;
		text_panel_info.text = U"placeholder";
		text_panel_info.font_path = oe::default_font_path + std::string("arialbi.ttf");
		/* text_panel_info.background_color = oe::colors::translucent_black; */
		textpanel = new oe::gui::TextPanel(text_panel_info);
		gui->addSubWidget(textpanel);

		{
			oe::gui::GraphInfo graph_info;
			graph_info.bg_panel_info.widget_info.size = { 150, 100 };
			graph_info.bg_panel_info.widget_info.offset_position = { 0, 5 };
			graph_info.bg_panel_info.widget_info.align_parent = oe::alignments::bottom_left;
			graph_info.bg_panel_info.widget_info.align_render = oe::alignments::top_left;
			graph_info.bg_panel_info.sprite = pack->emptySprite();
			graph_info.graph_color = oe::colors::green;
			graph_fps = new oe::gui::Graph(graph_info);
			textpanel->addSubWidget(graph_fps);
			
			graph_info.bg_panel_info.widget_info.offset_position = { 155, 5 };
			graph_info.graph_color = oe::colors::blue;
			graph_ups = new oe::gui::Graph(graph_info);
			textpanel->addSubWidget(graph_ups);
		}
	}
	{
		oe::gui::ListInfo list_info;
		list_info.widget_info = { { 200, 400 }, { 0, 0 }, oe::alignments::top_right, oe::alignments::top_right };
		list_info.sprite = pack->emptySprite();
		list_info.title = U"Loglist";
		list_info.title_height = 28;
		list = new oe::gui::List(list_info);
		gui->addSubWidget(list);

		// list->toggle(false); // TODO: fix bug that makes list not disappear, but go to oe::allignments::top_left
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
	engine_info.debug_messages = true;
	engine_info.ignore_errors = false;
	engine.init(engine_info);

	// window
	oe::WindowInfo window_config = {};
	window_config.title = "GUIs";
	window_config.multisamples = 4;
	window = oe::graphics::Window(window_config);

	// connect events
	window->connect_listener<oe::ResizeEvent, &resize>();
	window->connect_render_listener<&render>();
	window->connect_update_listener<30, &update_30>();

	// instance settings
	engine.swapInterval(0);
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
	gui = new oe::gui::GUI(window, oe::default_font_path + std::string("arialbd.ttf"));
	setup_gui();

	// start
	window->getGameloop().start();

	// cleanup
	delete gui;
	delete pack;
	delete shader_fill;
	delete shader_lines;

	return 0;
}