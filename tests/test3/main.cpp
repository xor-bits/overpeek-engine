#include <engine/engine.h>

#include <string>


oe::graphics::SingleTextureShader* shader;
oe::graphics::Renderer* renderer;
oe::graphics::Font* font;
oe::gui::GUI* gui;
glm::vec3 rotate(0.0f, 1.0f, 0.0f);

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
	ml_matrix = glm::rotate(ml_matrix, 0.01f, rotate);
	shader->modelMatrix(ml_matrix);
	oe::graphics::GL::setPolygonMode(1);
	renderer->end();
	renderer->render();
	oe::graphics::GL::setPolygonMode(0);
}

void render(float update_fraction) {
	// clear framebuffer
	oe::graphics::Window::clearWindow();

	// submitting
	cube();

	// gui
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
	// debugger
	// resize(oe::graphics::Window::getSize());
}

void setup_gui() {
	auto button = new oe::gui::Button({ 175, 50 });
	button->offset_position() = { 0, 20 };
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

	auto button_text = new oe::gui::TextView({ 40, 40 });
	button_text->align_parent() = oe::graphics::alignment::center_center;
	button_text->align_render() = oe::graphics::alignment::center_center;
	button_text->string() = "Button";
	button->addSubWidget(button_text);
}

void cursor_pos(const glm::vec2& cursor_window, const glm::vec2& cursor) {
	gui->cursor(-1, -1, cursor_window);
}

void button(int button, int action) {
	const glm::vec2& cursor = oe::graphics::Window::getCursorWindow();
	gui->cursor(button, action, cursor);
}

int main() {
	// engine
	oe::init();

	// window
	oe::graphics::Window::WindowConfig window_config;
	window_config.title = "Test 3 - GUIs";
	window_config.multisamples = 4;
	window_config.opengl_debugmode = true;
	oe::graphics::Window::init(window_config);
	oe::graphics::Window::setCursorPositionCallback(cursor_pos);
	oe::graphics::Window::setResizeCallback(resize);
	oe::graphics::Window::setButtonCallback(button);
	oe::graphics::GL::setSwapInterval(1);
	oe::graphics::GL::setBackFaceCulling(false);
	oe::graphics::GL::enableBlending();

	// drawing
	font = new oe::graphics::Font();
	oe::graphics::Text::setFont(*font);
	gui = new oe::gui::GUI();
	setup_gui();
	renderer = new oe::graphics::Renderer(oe::graphics::types::dynamicrender, oe::graphics::types::staticrender, 6, nullptr);
	shader = new oe::graphics::SingleTextureShader();
	resize(oe::graphics::Window::getSize());

	// start
	oe::utils::GameLoop::init(render, update, 1);

	// closing
	oe::graphics::Window::closeWindow();
	delete gui;
	delete font;
	delete renderer;
	delete shader;

	return 0;
}