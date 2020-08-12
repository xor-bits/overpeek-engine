#include <engine/include.hpp>

#include <string>



oe::gui::GUI* gui;
oe::gui::TextInput* textbox;
oe::gui::TextPanel* textpanel;
oe::gui::SpritePanel* box;
oe::gui::List* list;
oe::gui::Checkbox* checkbox;
oe::gui::VecSlider<4>* quat_slider;

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
	if (checkbox->m_checkbox_info.initial)
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

// update 30 times per second
void update_30() {
	auto& gameloop = window->getGameloop(); 
	std::u32string str = fmt::format(U"frametime: {:3.3f} ms ({} fps)", gameloop.getFrametimeMS(), gameloop.getAverageFPS());
	if(textpanel) textpanel->text_panel_info.text = str;
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
		sprite_panel_info.size = { 150, 150 };
		sprite_panel_info.align_parent = oe::alignments::bottom_left;
		sprite_panel_info.align_render = oe::alignments::bottom_left;
		sprite_panel_info.sprite = sprite;
		box = new oe::gui::SpritePanel(sprite_panel_info);
		gui->addSubWidget(box);
	}
	{
		oe::gui::TextInputInfo text_input_info;
		text_input_info.size = { 200, 80 };
		text_input_info.align_parent = oe::alignments::bottom_right;
		text_input_info.align_render = oe::alignments::bottom_right;
		text_input_info.font_size = 14;
		text_input_info.sprite = pack->emptySprite();
		textbox = new oe::gui::TextInput(text_input_info);
		gui->addSubWidget(textbox);
	}
	{
		oe::gui::DecoratedButtonInfo button_info;
		button_info.size = { 175, 50 };
		button_info.align_parent = oe::alignments::top_center;
		button_info.align_render = oe::alignments::top_center;
		button_info.sprite = pack->emptySprite();
		button_info.text = U"log";
		auto button = new oe::gui::DecoratedButton(button_info);
		gui->addSubWidget(button);

		auto callback_lambda = [&](const oe::gui::ButtonUseEvent& e) {
			if (e.action == oe::actions::release && e.button == oe::mouse_buttons::button_left) {
				glm::vec4 quat_slider_val = quat_slider->getGLM();
				append_list(glm::angleAxis(quat_slider_val.w, glm::normalize(glm::vec3(quat_slider_val.x, quat_slider_val.y, quat_slider_val.z))));
			}
		};
		button->connect_listener<oe::gui::ButtonUseEvent, &decltype(callback_lambda)::operator()>(callback_lambda);
	}
	{
		oe::gui::VecSliderInfo<4> vecslider_info;
		vecslider_info.slider_size = { 400, 30 };
		vecslider_info.knob_size = { 45, 45 };
		vecslider_info.align_parent = oe::alignments::bottom_center;
		vecslider_info.align_render = oe::alignments::bottom_center;
		vecslider_info.slider_sprite = pack->emptySprite();
		vecslider_info.knob_sprite = sprite;
		vecslider_info.min_values = { -glm::pi<float>(), -1.0f, -1.0f, -1.0f };
		vecslider_info.max_values = { glm::pi<float>(), 1.0f, 1.0f, 1.0f };
		vecslider_info.initial_values = { 0.0f, 1.0f, 1.0f, 1.0f };
		vecslider_info.draw_value = true;
		quat_slider = new oe::gui::VecSlider<4>(vecslider_info);
		gui->addSubWidget(quat_slider);
	}
	{
		oe::gui::CheckboxInfo ci;
		ci.align_parent = oe::alignments::bottom_center;
		ci.align_render = oe::alignments::bottom_center;
		ci.offset_position = { 0, -40 };
		ci.sprite = pack->emptySprite();
		checkbox = new oe::gui::Checkbox(ci);
		gui->addSubWidget(checkbox);
	}
	{
		oe::gui::TextPanelInfo text_panel_info;
		text_panel_info.font_size = 20;
		text_panel_info.align_parent = oe::alignments::top_left;
		text_panel_info.align_render = oe::alignments::top_left;
		text_panel_info.text = U"placeholder";
		text_panel_info.font_path = oe::default_font_path + std::string("arialbi.ttf");
		textpanel = new oe::gui::TextPanel(text_panel_info);
		gui->addSubWidget(textpanel);
	}
	{
		oe::gui::ColorPickerInfo color_picker_info;
		color_picker_info.size = { 200, 100 };
		color_picker_info.initial_color = color;
		color_picker_info.align_parent = oe::alignments::center_left;
		color_picker_info.align_render = oe::alignments::center_left;
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
		oe::gui::ListInfo list_info;
		list_info.size = { 200, 400 };
		list_info.align_parent = oe::alignments::top_right;
		list_info.align_render = oe::alignments::top_right;
		list_info.sprite = pack->emptySprite();
		list_info.title = U"Loglist";
		list_info.title_height = 28;
		list = new oe::gui::List(list_info);
		gui->addSubWidget(list);
	}

	tpi = {};
	tpi.font_size = 14;
	tpi.align_parent = oe::alignments::top_left;
	tpi.align_render = oe::alignments::top_left;
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