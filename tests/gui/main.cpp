#include <engine/engine.h>

#include <string>

#include "tex.h"



oe::gui::TextInput* textbox;
oe::gui::TextPanel* textpanel;

const oe::graphics::Sprite* sprite;
oe::graphics::SpritePack* pack;
oe::graphics::SingleTextureShader* shader;
oe::graphics::Renderer* renderer;
oe::graphics::Font* font;
oe::gui::GUI* gui;

glm::vec3 rotate(0.0f, 1.0f, 0.0f);
float speed = 0.01f;

void cube() {
	// begin submitting
	renderer->begin();
	renderer->clear();

	// front
	renderer->submitVertexData({ -1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f }, 0, oe::colors::orange);
	renderer->submitVertexData({ -1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f }, 0, oe::colors::orange);
	renderer->submitVertexData({  1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f }, 0, oe::colors::orange);
	renderer->submitVertexData({  1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f }, 0, oe::colors::orange);
	renderer->incrementQuadCount();

	// back
	renderer->submitVertexData({ -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f }, 0, oe::colors::orange);
	renderer->submitVertexData({ -1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f }, 0, oe::colors::orange);
	renderer->submitVertexData({  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f }, 0, oe::colors::orange);
	renderer->submitVertexData({  1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f }, 0, oe::colors::orange);
	renderer->incrementQuadCount();

	// top
	renderer->submitVertexData({ -1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f }, 0, oe::colors::orange);
	renderer->submitVertexData({ -1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f }, 0, oe::colors::orange);
	renderer->submitVertexData({  1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f }, 0, oe::colors::orange);
	renderer->submitVertexData({  1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f }, 0, oe::colors::orange);
	renderer->incrementQuadCount();

	// bottom
	renderer->submitVertexData({ -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f }, 0, oe::colors::orange);
	renderer->submitVertexData({ -1.0f, -1.0f,  1.0f }, { 0.0f, 1.0f }, 0, oe::colors::orange);
	renderer->submitVertexData({  1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f }, 0, oe::colors::orange);
	renderer->submitVertexData({  1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f }, 0, oe::colors::orange);
	renderer->incrementQuadCount();

	// left
	renderer->submitVertexData({ -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f }, 0, oe::colors::orange);
	renderer->submitVertexData({ -1.0f, -1.0f,  1.0f }, { 0.0f, 1.0f }, 0, oe::colors::orange);
	renderer->submitVertexData({ -1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f }, 0, oe::colors::orange);
	renderer->submitVertexData({ -1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f }, 0, oe::colors::orange);
	renderer->incrementQuadCount();

	// right
	renderer->submitVertexData({  1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f }, 0, oe::colors::orange);
	renderer->submitVertexData({  1.0f, -1.0f,  1.0f }, { 0.0f, 1.0f }, 0, oe::colors::orange);
	renderer->submitVertexData({  1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f }, 0, oe::colors::orange);
	renderer->submitVertexData({  1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f }, 0, oe::colors::orange);
	renderer->incrementQuadCount();

	// stop submitting and rendera
	static glm::mat4 ml_matrix = glm::mat4(1.0f);
	shader->bind();
	ml_matrix = glm::rotate(ml_matrix, speed, rotate);
	shader->modelMatrix(ml_matrix);
	oe::graphics::GL::setPolygonMode(oe::graphics::GL::polygon_mode::lines);
	renderer->end();
	renderer->render();
	oe::graphics::GL::setPolygonMode(oe::graphics::GL::polygon_mode::fill);
}

void render(float update_fraction) {
	// clear framebuffer
	oe::graphics::Window::clearWindow();

	// submitting
	cube();

	// gui
	pack->bind();
	gui->render();

	// swap buffers and poll events
	oe::graphics::Window::updateWindow();
	oe::graphics::Window::pollEvents();

	// check if needs to close
	if (oe::graphics::Window::windowShouldClose()) oe::utils::GameLoop::stop();
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
		button->align_parent() = oe::graphics::alignment::top_center;
		button->align_render() = oe::graphics::alignment::top_center;
		button->setCallback(
			[](int button, int action) {
				if (action == OE_PRESS && button == OE_MOUSE_BUTTON_LEFT) {
					// spdlog::info("Button pressed"); 
					std::swap(rotate.x, rotate.y);
					std::swap(rotate.y, rotate.z);
				}
				else if (action == -1 && button == -1) {
					// spdlog::info("Button hovered");
				}
			}
		);
		gui->addSubWidget(button);

		auto button_background = new oe::gui::SpritePanel({ 175, 50 });
		button_background->align_parent() = oe::graphics::alignment::center_center;
		button_background->align_render() = oe::graphics::alignment::center_center;
		button_background->setSprite(pack->empty_sprite());
		button_background->color() = oe::colors::grey;
		button->addSubWidget(button_background);

		auto button_text = new oe::gui::TextPanel(40);
		button_text->align_parent() = oe::graphics::alignment::center_center;
		button_text->align_render() = oe::graphics::alignment::center_center;
		button_text->string() = "New dir";
		button_background->addSubWidget(button_text);
	}
	{
		auto button = new oe::gui::Button({ 175, 50 });
		button->offset_position() = { 0, 70 };
		button->align_parent() = oe::graphics::alignment::top_center;
		button->align_render() = oe::graphics::alignment::top_center;
		button->setCallback(
			[](int button, int action) {
				if (action == OE_PRESS && button == OE_MOUSE_BUTTON_LEFT) {
					// spdlog::info("Button pressed"); 
					rotate *= -1;
				}
				else if (action == -1 && button == -1) {
					// spdlog::info("Button hovered");
				}
			}
		);
		gui->addSubWidget(button);

		auto button_background = new oe::gui::SpritePanel({ 175, 50 });
		button_background->align_parent() = oe::graphics::alignment::center_center;
		button_background->align_render() = oe::graphics::alignment::center_center;
		button_background->setSprite(pack->empty_sprite());
		button_background->color() = oe::colors::grey;
		button->addSubWidget(button_background);

		auto button_text = new oe::gui::TextPanel(40);
		button_text->align_parent() = oe::graphics::alignment::center_center;
		button_text->align_render() = oe::graphics::alignment::center_center;
		button_text->string() = "Reverse";
		button_background->addSubWidget(button_text);
	}
	{
		auto box = new oe::gui::SpritePanel({ 150, 150 });
		box->align_parent() = oe::graphics::alignment::bottom_left;
		box->align_render() = oe::graphics::alignment::bottom_left;
		box->offset_position() = { 10, -10 };
		box->setSprite(sprite);
		gui->addSubWidget(box);
	}
	{
		textbox = new oe::gui::TextInput({ 200, 80 });
		textbox->align_parent() = oe::graphics::alignment::bottom_right;
		textbox->align_render() = oe::graphics::alignment::bottom_right;
		textbox->font_size() = 14;
		textbox->setSprite(pack->empty_sprite());
		gui->addSubWidget(textbox);

		{
			auto button = new oe::gui::DecoratedButton({ 175, 50 }, pack->empty_sprite(), "Log");
			button->offset_position() = { 0, -10 };
			button->align_parent() = oe::graphics::alignment::top_center;
			button->align_render() = oe::graphics::alignment::bottom_center;
			button->setCallback(
				[](int button, int action) {
					if (action == OE_PRESS && button == OE_MOUSE_BUTTON_LEFT) {
						spdlog::info(textbox->string());
						textbox->string() = "";
					}
					else if (action == -1 && button == -1) {
						// spdlog::info("Button hovered");
					}
				}
			);
			textbox->addSubWidget(button);
		}
	}
	{
		auto box = new oe::gui::Slider({ 400, 30 });
		box->align_parent() = oe::graphics::alignment::bottom_center;
		box->align_render() = oe::graphics::alignment::bottom_center;
		box->setSprite(pack->empty_sprite());
		box->min() = -0.3f;
		box->max() = 0.3f;
		box->setCallback([](float val) {speed = val; });
		gui->addSubWidget(box);
	}
	{
		textpanel = new oe::gui::TextPanel(20);
		textpanel->align_parent() = oe::graphics::alignment::top_left;
		textpanel->align_render() = oe::graphics::alignment::top_left;
		textpanel->string() = "FPS:";
		gui->addSubWidget(textpanel);
	}
}

void cursor_pos(const glm::vec2& cursor_window, const glm::vec2& cursor) {
	gui->cursor(-1, -1, cursor_window);
}

void button(int button, int action) {
	const glm::vec2& cursor = oe::graphics::Window::getCursorWindow();
	gui->cursor(button, action, cursor);
}

void text(unsigned int character, int mods) {
	gui->text(character, mods);
}

void keyboard(int key, int action, int mods) {
	gui->key(key, action, mods);
}

int main(int argc, char* argv[]) {
	// engine
	oe::init(argc, argv);

	// window
	oe::graphics::Window::WindowConfig window_config;
	window_config.title = "Test 3 - GUIs";
	window_config.multisamples = 4;
	window_config.opengl_debugmode = true;
	oe::graphics::Window::init(window_config);
	oe::graphics::Window::setCursorPositionCallback(cursor_pos);
	oe::graphics::Window::setResizeCallback(resize);
	oe::graphics::Window::setButtonCallback(button);
	oe::graphics::Window::setCharmodCallback(text);
	oe::graphics::Window::setKeyboardCallback(keyboard);
	oe::graphics::GL::setSwapInterval(0);
	oe::graphics::GL::setCulling(oe::graphics::GL::culling::neither);
	oe::graphics::GL::enableBlending();

	// drawing
	pack = new oe::graphics::SpritePack();
	unsigned char* data = new unsigned char[16 * 16 * 4];
	for (size_t i = 0; i < 16 * 16 * 4; i++) data[i] = texture_data[i];
	oe::utils::image_data img(data, 16, 16, 4);
	sprite = pack->addSprite(img);
	gui = new oe::gui::GUI();
	setup_gui();
	renderer = new oe::graphics::Renderer(oe::graphics::types::dynamicrender, oe::graphics::types::staticrender, 6, nullptr);
	shader = new oe::graphics::SpriteShader();
	resize(oe::graphics::Window::getSize());

	// font
	font = new oe::graphics::Font(pack);
	oe::graphics::Text::setFont(*font);
	pack->construct();

	// start
	oe::utils::GameLoop::init(render, update, 1);

	// closing
	oe::graphics::Window::closeWindow();
	delete gui;
	delete font;
	delete renderer;
	delete shader;
	delete pack;

	return 0;
}