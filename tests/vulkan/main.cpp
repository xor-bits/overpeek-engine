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
oe::graphics::DefaultShader* shader;
oe::graphics::Renderer* renderer;
oe::graphics::Font* font;

glm::vec3 rotate(0.0f, 1.0f, 0.0f);
float speed = 0.01f;

void cube() {
	// begin submitting
	renderer->begin();
	renderer->clear();

	// front
	renderer->submit(oe::graphics::VertexData({ -1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({ -1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({  1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({  1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f }, oe::colors::orange));
	renderer->quadCount()++;

	// back
	renderer->submit(oe::graphics::VertexData({ -1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({ -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({  1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f }, oe::colors::orange));
	renderer->quadCount()++;

	// top
	renderer->submit(oe::graphics::VertexData({ -1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({ -1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({  1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({  1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f }, oe::colors::orange));
	renderer->quadCount()++;

	// bottom
	renderer->submit(oe::graphics::VertexData({ -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({ -1.0f, -1.0f,  1.0f }, { 0.0f, 1.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({  1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({  1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f }, oe::colors::orange));
	renderer->quadCount()++;

	// left
	renderer->submit(oe::graphics::VertexData({ -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({ -1.0f, -1.0f,  1.0f }, { 0.0f, 1.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({ -1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({ -1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f }, oe::colors::orange));
	renderer->quadCount()++;

	// right
	renderer->submit(oe::graphics::VertexData({  1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({  1.0f, -1.0f,  1.0f }, { 0.0f, 1.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({  1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f }, oe::colors::orange));
	renderer->submit(oe::graphics::VertexData({  1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f }, oe::colors::orange));
	renderer->quadCount()++;

	// stop submitting and rendera
	static glm::mat4 ml_matrix = glm::mat4(1.0f);
	shader->bind();
	ml_matrix = glm::rotate(ml_matrix, speed, rotate);
	shader->modelMatrix(ml_matrix);
	instance->polygonMode(oe::polygon_mode::lines);
	renderer->end();
	renderer->render();
	instance->polygonMode(oe::polygon_mode::fill);
}

void render(float update_fraction) {
	// clear framebuffer
	window->clear();

	// submitting
	cube();

	// gui
	pack->bind();
	gui->render();

	// swap buffers and poll events
	window->update();

	// check if needs to close
	if (window->shouldClose()) oe::utils::GameLoop::stop();
}

void resize(const glm::vec2& window_size) {
	gui->resize();

	shader->bind();
	shader->useTexture(false);
	glm::mat4 pr_matrix = glm::perspectiveFov(30.0f, (float)window_size.x, (float)window_size.y, 0.0f, 1000.0f);
	shader->projectionMatrix(pr_matrix);
	glm::mat4 vw_matrix = glm::lookAt(glm::vec3(0.0f, 0.0f,-5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader->viewMatrix(vw_matrix);
}

void update() {
	textpanel->string() = fmt::format("FPS: {}", oe::utils::GameLoop::getFPS());
}

void setup_gui() {
	{
		auto button = new oe::gui::Button({ 175, 50 });
		button->offset_position() = { 0, 10 };
		button->align_parent() = oe::alignments::top_center;
		button->align_render() = oe::alignments::top_center;
		button->setCallback(
			[](oe::mouse_buttons button, oe::actions action) {
				if (action == oe::actions::press && button == oe::mouse_buttons::button_left) {
					// spdlog::info("Button pressed"); 
					std::swap(rotate.x, rotate.y);
					std::swap(rotate.y, rotate.z);
				}
				else if (action == oe::actions::none || button == oe::mouse_buttons::none) {
					// spdlog::info("Button hovered");
				}
			}
		);
		gui->addSubWidget(button);

		auto button_background = new oe::gui::SpritePanel({ 175, 50 });
		button_background->align_parent() = oe::alignments::center_center;
		button_background->align_render() = oe::alignments::center_center;
		button_background->setSprite(pack->empty_sprite());
		button_background->color() = oe::colors::grey;
		button->addSubWidget(button_background);

		auto button_text = new oe::gui::TextPanel(40);
		button_text->align_parent() = oe::alignments::center_center;
		button_text->align_render() = oe::alignments::center_center;
		button_text->string() = "New dir";
		button_background->addSubWidget(button_text);
	}
	{
		auto button = new oe::gui::Button({ 175, 50 });
		button->offset_position() = { 0, 70 };
		button->align_parent() = oe::alignments::top_center;
		button->align_render() = oe::alignments::top_center;
		button->setCallback(
			[](oe::mouse_buttons button, oe::actions action) {
				if (action == oe::actions::press && button == oe::mouse_buttons::button_left) {
					// spdlog::info("Button pressed"); 
					rotate *= -1;
				}
				else if (action == oe::actions::none || button == oe::mouse_buttons::none) {
					// spdlog::info("Button hovered");
				}
			}
		);
		gui->addSubWidget(button);

		auto button_background = new oe::gui::SpritePanel({ 175, 50 });
		button_background->align_parent() = oe::alignments::center_center;
		button_background->align_render() = oe::alignments::center_center;
		button_background->setSprite(pack->empty_sprite());
		button_background->color() = oe::colors::grey;
		button->addSubWidget(button_background);

		auto button_text = new oe::gui::TextPanel(40);
		button_text->align_parent() = oe::alignments::center_center;
		button_text->align_render() = oe::alignments::center_center;
		button_text->string() = "Reverse";
		button_background->addSubWidget(button_text);
	}
	{
		auto box = new oe::gui::SpritePanel({ 150, 150 });
		box->align_parent() = oe::alignments::bottom_left;
		box->align_render() = oe::alignments::bottom_left;
		box->offset_position() = { 10, -10 };
		box->setSprite(sprite);
		gui->addSubWidget(box);
	}
	{
		textbox = new oe::gui::TextInput(window, { 200, 80 });
		textbox->align_parent() = oe::alignments::bottom_right;
		textbox->align_render() = oe::alignments::bottom_right;
		textbox->font_size() = 14;
		textbox->setSprite(pack->empty_sprite());
		gui->addSubWidget(textbox);

		{
			auto button = new oe::gui::DecoratedButton({ 175, 50 }, pack->empty_sprite(), "Log");
			button->offset_position() = { 0, -10 };
			button->align_parent() = oe::alignments::top_center;
			button->align_render() = oe::alignments::bottom_center;
			button->setCallback(
				[](oe::mouse_buttons button, oe::actions action) {
					if (action == oe::actions::press && button == oe::mouse_buttons::button_left) {
						spdlog::info(textbox->string());
						textbox->string() = "";
					}
					else if (action == oe::actions::none || button == oe::mouse_buttons::none) {
						// spdlog::info("Button hovered");
					}
				}
			);
			textbox->addSubWidget(button);
		}
	}
	{
		auto box = new oe::gui::Slider({ 400, 30 });
		box->align_parent() = oe::alignments::bottom_center;
		box->align_render() = oe::alignments::bottom_center;
		box->setSprite(pack->empty_sprite());
		box->min() = -0.3f;
		box->max() = 0.3f;
		box->setCallback([](float val) {speed = val; });
		gui->addSubWidget(box);
	}
	{
		textpanel = new oe::gui::TextPanel(20);
		textpanel->align_parent() = oe::alignments::top_left;
		textpanel->align_render() = oe::alignments::top_left;
		textpanel->string() = "FPS:";
		gui->addSubWidget(textpanel);
	}
}

void cursor_pos(const glm::vec2& cursor_window, const glm::vec2& cursor) {
	gui->cursor(oe::mouse_buttons::none, oe::actions::none, cursor_window);
}

void button(oe::mouse_buttons button, oe::actions action) {
	const glm::vec2& cursor = window->getCursorWindow();
	gui->cursor(button, action, cursor);
}

void text(uint32_t character, oe::modifiers mods) {
	gui->text(character, mods);
}

void keyboard(oe::keys key, oe::actions action, oe::modifiers mods) {
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
	// instance_info.favored_gpu_vulkan = oe::gpu::dedicated;
	instance = oe::Engine::createInstance(instance_info);

	spdlog::info("API: {}", instance->getAPI());
	spdlog::info("APIVersion: {}", instance->getAPIVersion());
	spdlog::info("GPU: {}", instance->getGPU());
	spdlog::info("GPUVendor: {}", instance->getGPUVendor());

	// window
	oe::WindowInfo window_config = {};
	window_config.title = "Test 5 - Vulkan";
	window_config.multisamples = 4;
	window = instance->createWindow(window_config);
	window->setCursorPositionCallback(cursor_pos);
	window->setResizeCallback(resize);
	window->setButtonCallback(button);
	window->setCharmodCallback(text);
	window->setKeyboardCallback(keyboard);

	// instance settings
	instance->swapInterval(0);
	instance->culling(oe::culling_modes::neither);
	instance->blending(oe::modes::enable);

	// renderer
	oe::RendererInfo renderer_info = {};
	renderer_info.arrayRenderType = oe::types::dynamicrender;
	renderer_info.indexRenderType = oe::types::staticrender;
	renderer_info.max_quad_count = 10000;
	renderer_info.staticVBOBuffer_data = nullptr;
	renderer = instance->createRenderer(renderer_info);

	// shader
	shader = new oe::graphics::DefaultShader(instance);

	// spritepack
	auto img = oe::utils::loadImageCopy(texture_png, 5, 5);
	pack = new oe::graphics::SpritePack(instance);
	sprite = pack->addSprite(img);

	// font
	font = new oe::graphics::Font(pack);
	oe::graphics::Text::setFont(*font);
	pack->construct();

	// gui
	gui = new oe::gui::GUI(instance, window);
	setup_gui();

	// start
	resize(window->getSize());
	oe::utils::GameLoop::init(render, update, 1);

	// cleanup
	delete gui;
	delete font;
	delete pack;
	delete shader;
	instance->destroyWindow(window);
	instance->destroyRenderer(renderer);
	oe::Engine::deinit();

	return 0;
}