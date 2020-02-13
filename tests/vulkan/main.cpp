#include <engine/engine.hpp>

#include <string>

#include "texture.png.hpp"



oe::gui::GUI* gui;
oe::gui::TextInput* textbox;
oe::gui::TextPanel* textpanel;

const oe::graphics::Sprite* sprite;
oe::graphics::Instance* instance;
oe::graphics::Window* window;
oe::graphics::SpritePack* pack;
oe::graphics::Shader* shader;
oe::graphics::Renderer* renderer;
oe::graphics::Font* font;

glm::vec3 rotate(0.0f, 1.0f, 0.0f);
float speed = 0.0f;

void cube() {
	// begin submitting
	renderer->begin();
	renderer->clear();
	
	// front
	renderer->submit(oe::graphics::VertexData({ -1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({ -1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({ 1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({ 1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f }, oe::colors::orange));
	renderer->quadCount()++;
	
	// back
	renderer->submit(oe::graphics::VertexData({ -1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({ -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({ 1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({ 1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f }, oe::colors::orange));
	renderer->quadCount()++;
	
	// top
	renderer->submit(oe::graphics::VertexData({ -1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({ -1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({ 1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({ 1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f }, oe::colors::orange));
	renderer->quadCount()++;
	
	// bottom
	renderer->submit(oe::graphics::VertexData({ -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({ -1.0f, -1.0f,  1.0f }, { 0.0f, 1.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({ 1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({ 1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f }, oe::colors::orange));
	renderer->quadCount()++;
	
	// left
	renderer->submit(oe::graphics::VertexData({ -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({ -1.0f, -1.0f,  1.0f }, { 0.0f, 1.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({ -1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({ -1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f }, oe::colors::orange));
	renderer->quadCount()++;
	
	// right
	renderer->submit(oe::graphics::VertexData({ 1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({ 1.0f, -1.0f,  1.0f }, { 0.0f, 1.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({ 1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({ 1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f }, oe::colors::orange));
	renderer->quadCount()++;
	
	// stop submitting and rendera
	static glm::mat4 ml_matrix = glm::mat4(1.0f);
	shader->bind();
	ml_matrix = glm::rotate(ml_matrix, speed * -0.1f, rotate);
	shader->setUniformMat4("ml_matrix", ml_matrix);
	instance->polygonMode(oe::polygon_mode::lines);
	renderer->end();
	renderer->render();
	instance->polygonMode(oe::polygon_mode::fill);
}

void render(float update_fraction) {
	// clear framebuffer
	window->clear();

	// submitting
	// cube();

	// gui
	// pack->bind();
	// gui->render();

	// swap buffers and poll events
	window->update();

	// check if needs to close
	if (window->shouldClose()) oe::utils::GameLoop::stop();
}

void resize(const glm::vec2& window_size) {
	gui->resize();

	shader->bind();
	shader->setUniform1i("usetex", false);
	glm::mat4 pr_matrix = glm::perspectiveFov(30.0f, (float)window_size.x, (float)window_size.y, 0.0f, 1000.0f);
	shader->setUniformMat4("pr_matrix", pr_matrix);
	glm::mat4 vw_matrix = glm::lookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader->setUniformMat4("vw_matrix", vw_matrix);
}

void update() {
	textpanel->text_panel_info.text = fmt::format("FPS: {}", oe::utils::GameLoop::getFPS());
}

void setup_gui() {
	{
		oe::gui::DecoratedButtonInfo button_info = {};
		button_info.size = { 175, 50 };
		button_info.offset_position = { 0, 10 };
		button_info.align_parent = oe::alignments::top_center;
		button_info.align_render = oe::alignments::top_center;
		button_info.sprite = pack->empty_sprite();
		button_info.text = "new dir";
		button_info.callback = [](oe::mouse_buttons button, oe::actions action) {
			if (action == oe::actions::release && button == oe::mouse_buttons::button_left) {
				// spdlog::info("Button pressed"); 
				std::swap(rotate.x, rotate.y);
				std::swap(rotate.y, rotate.z);
			}
			else if (action == oe::actions::none || button == oe::mouse_buttons::none) {
				// spdlog::info("Button hovered");
			}
		};
		auto button = new oe::gui::DecoratedButton(button_info);
		gui->addSubWidget(button);
	}
	{
		oe::gui::DecoratedButtonInfo button_info = {};
		button_info.size = { 175, 50 };
		button_info.offset_position = { 0, 70 };
		button_info.align_parent = oe::alignments::top_center;
		button_info.align_render = oe::alignments::top_center;
		button_info.sprite = pack->empty_sprite();
		button_info.text = "reverse";
		button_info.callback = [](oe::mouse_buttons button, oe::actions action) {
			if (action == oe::actions::release && button == oe::mouse_buttons::button_left) {
				// spdlog::info("Button pressed"); 
				rotate *= -1;
			}
			else if (action == oe::actions::none || button == oe::mouse_buttons::none) {
				// spdlog::info("Button hovered");
			}
		};
		auto button = new oe::gui::DecoratedButton(button_info);
		gui->addSubWidget(button);
	}
	{
		oe::gui::SpritePanelInfo sprite_panel_info = {};
		sprite_panel_info.size = { 150, 150 };
		sprite_panel_info.align_parent = oe::alignments::bottom_left;
		sprite_panel_info.align_render = oe::alignments::bottom_left;
		sprite_panel_info.offset_position = { 10, -10 };
		sprite_panel_info.sprite = sprite;
		auto box = new oe::gui::SpritePanel(sprite_panel_info);
		gui->addSubWidget(box);
	}
	{
		oe::gui::TextInputInfo text_input_info = {};
		text_input_info.size = { 200, 80 };
		text_input_info.window_handle = window;
		text_input_info.align_parent = oe::alignments::bottom_right;
		text_input_info.align_render = oe::alignments::bottom_right;
		text_input_info.font_size = 14;
		text_input_info.sprite = pack->empty_sprite();
		textbox = new oe::gui::TextInput(text_input_info);
		gui->addSubWidget(textbox);

		{
			oe::gui::DecoratedButtonInfo button_info = {};
			button_info.size = { 175, 50 };
			button_info.offset_position = { 0, -10 };
			button_info.align_parent = oe::alignments::top_center;
			button_info.align_render = oe::alignments::bottom_center;
			button_info.sprite = pack->empty_sprite();
			button_info.text = "log";
			button_info.callback = [](oe::mouse_buttons button, oe::actions action) {
				if (action == oe::actions::release && button == oe::mouse_buttons::button_left) {
					spdlog::info(textbox->text_input_info.text);
					textbox->text_input_info.text = "";
				}
				else if (action == oe::actions::none || button == oe::mouse_buttons::none) {
					// spdlog::info("Button hovered");
				}
			};
			auto button = new oe::gui::DecoratedButton(button_info);
			textbox->addSubWidget(button);
		}
	}
	{
		oe::gui::SliderInfo slider_info = {};
		slider_info.slider_size = { 400, 30 };
		slider_info.knob_size = { 45, 45 };
		slider_info.align_parent = oe::alignments::bottom_center;
		slider_info.align_render = oe::alignments::bottom_center;
		slider_info.slider_sprite = pack->empty_sprite();
		slider_info.knob_sprite = sprite;
		slider_info.min_value = -1.0f;
		slider_info.max_value = 1.0f;
		slider_info.initial_value = 0.5f;
		slider_info.callback = [](float val) { speed = val; };
		auto slider = new oe::gui::Slider(slider_info);
		gui->addSubWidget(slider);
	}
	{
		oe::gui::TextPanelInfo text_panel_info = {};
		text_panel_info.font_size = 20;
		text_panel_info.align_parent = oe::alignments::top_left;
		text_panel_info.align_render = oe::alignments::top_left;
		text_panel_info.text = "FPS:";
		textpanel = new oe::gui::TextPanel(text_panel_info);
		gui->addSubWidget(textpanel);
	}
}

void cursor_pos(const glm::vec2& cursor, const glm::vec2& cursor_window) {
	// spdlog::debug("cursor_window {}, cursor {}", cursor_window, cursor);
	gui->cursor(oe::mouse_buttons::none, oe::actions::none, cursor_window);
}

void button(oe::mouse_buttons button, oe::actions action) {
	// spdlog::debug("button {}, action {}", button, action);
	const glm::vec2& cursor = window->getCursorWindow();
	gui->cursor(button, action, cursor);
}

void text(uint32_t character, oe::modifiers mods) {
	// spdlog::debug("character {}, mods {}", character, mods);
	gui->text(character, mods);
}

void keyboard(oe::keys key, oe::actions action, oe::modifiers mods) {
	// spdlog::debug("key {}, action {}, mods {}", key, action, mods);
	gui->key(key, action, mods);
}

int main() {
	// engine
	oe::EngineInfo engine_info = {};
	engine_info.api = oe::graphics_api::Vulkan;
	engine_info.networking = false;
	engine_info.audio = false;
	oe::Engine::init(engine_info);

	// instance
	oe::InstanceInfo instance_info = {};
	instance_info.debug_messages = true;
	instance_info.favored_gpu_vulkan = oe::gpu::dedicated;
	instance = oe::Engine::createInstance(instance_info);

	// window
	oe::WindowInfo window_config = {};
	window_config.title = "Test 3 - GUIs";
	window_config.multisamples = 4;
	window = instance->createWindow(window_config);
	window->setCursorPositionCallback(cursor_pos);
	window->setResizeCallback(resize);
	window->setButtonCallback(button);
	window->setCharmodCallback(text);
	window->setKeyboardCallback(keyboard);

	// print render info
	spdlog::info("GPU: {}", window->getGPU());
	spdlog::info("GPUVendor: {}", window->getGPUVendor());
	spdlog::info("API: {}", window->getAPI());
	spdlog::info("APIVersion: {}", window->getAPIVersion());

	// instance settings
	instance->swapInterval(1);
	instance->culling(oe::culling_modes::neither);
	instance->blending(oe::modes::enable);

	// renderer
	oe::RendererInfo renderer_info = {};
	renderer_info.arrayRenderType = oe::types::dynamicrender;
	renderer_info.indexRenderType = oe::types::staticrender;
	renderer_info.max_quad_count = 10000;
	renderer_info.staticVBOBuffer_data = nullptr;
	renderer = window->createRenderer(renderer_info);

	// shader
	shader = window->createShader(oe::ShaderInfo());

	// spritepack
	auto img = oe::utils::loadImageCopy(texture_png, 5, 5);
	pack = new oe::graphics::SpritePack(window);
	sprite = pack->addSprite(img);

	// font
	font = new oe::graphics::Font(pack);
	oe::graphics::Text::setFont(*font);
	pack->construct();

	// gui
	gui = new oe::gui::GUI(window);
	setup_gui();

	// start
	resize(window->getSize());
	oe::utils::GameLoop::init(render, update, 1);

	// cleanup
	delete gui;
	delete font;
	delete pack;
	delete shader;
	window->destroyRenderer(renderer);
	instance->destroyWindow(window);
	oe::Engine::deinit();

	return 0;
}