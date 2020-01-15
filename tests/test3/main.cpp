#include <engine/engine.h>

#include <string>


oe::graphics::SingleTextureShader* shader;
oe::graphics::Renderer* renderer;
oe::graphics::Font* font;
oe::gui::GUI* gui;

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
	static float t;
	t += 0.005 * oe::utils::GameLoop::getFrameMilliseconds();
	shader->bind();
	glm::mat4 ml_matrix = glm::mat4(1.0f);
	ml_matrix = glm::rotate(glm::mat4(1.0f), t, glm::vec3(0.0f, 1.0f, 0.0f));
	shader->modelMatrix(ml_matrix);
	oe::graphics::GL::setPolygonMode(1);
	renderer->end();
	renderer->render();
	oe::graphics::GL::setPolygonMode(0);
}

void render(float update_fraction) {
	// clear framebuffer
	oe::graphics::Window::clear();

	// submitting
	cube();

	// gui
	gui->render();

	// swap buffers and poll events
	oe::graphics::Window::update();
	oe::graphics::Window::input();

	// check if needs to close
	if (oe::graphics::Window::shouldClose()) oe::utils::GameLoop::stop();
}

void resize(int width, int height) {
	gui->resize();

	shader->bind();
	shader->useTexture(false);
	glm::mat4 pr_matrix = glm::perspectiveFov(30.0f, (float)width, (float)height, 0.0f, 1000.0f);
	shader->projectionMatrix(pr_matrix);
	glm::mat4 vw_matrix = glm::lookAt(glm::vec3(0.0f, 0.0f,-5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader->viewMatrix(vw_matrix);
}

void update() {
	spdlog::info("Performance\nFPS: {}\nmspf: {}\n--------\n", oe::utils::GameLoop::getFPS(), oe::utils::GameLoop::getFrameMilliseconds());

	// debugger
	resize(oe::graphics::Window::getWidth(), oe::graphics::Window::getHeight());
}

void setup_gui() {
	{
		auto widget = new oe::gui::Form({ 200, 200 });
		widget->align_parent() = oe::graphics::alignment::top_center;
		widget->align_render() = oe::graphics::alignment::top_center;
		gui->addSubWidget(widget);

		auto widget2 = new oe::gui::Button({ 50, 50 });
		widget2->align_parent() = oe::graphics::alignment::center_center;
		widget2->align_render() = oe::graphics::alignment::center_center;
		widget2->setCallback([] { spdlog::info("Button pressed"); });
		widget->addSubWidget(widget2);
		
		{

			/*auto text = new oe::gui::TextView({ 50, 50 });
			text->align_parent() = oe::graphics::alignment::center_center;
			text->align_render() = oe::graphics::alignment::center_center;
			text->string() = "Test text view";
			widget->addSubWidget(text);*/
		}
	}
	{
		auto widget = new oe::gui::Form({ 100, 40 });
		widget->align_parent() = oe::graphics::alignment::bottom_right;
		widget->align_render() = oe::graphics::alignment::bottom_right;
		gui->addSubWidget(widget);

		{
			/*auto button = new oe::gui::TextView({ 50, 50 });
			button->align_parent() = oe::graphics::alignment::center_center;
			button->align_render() = oe::graphics::alignment::center_center;
			button->string() = "Button";
			widget->addSubWidget(button);*/
		}
	}
}

void cursor_pos(const glm::vec2& cursor) {
	float aspect = oe::graphics::Window::getAspect();
	glm::vec2 window = { oe::graphics::Window::getWidth(), oe::graphics::Window::getHeight() };
	gui->cursor(0, 0, 
		oe::utils::map(cursor.x, -aspect, aspect, 0.0f, window.x), 
		oe::utils::map(cursor.y, 1.0f, -1.0f, 0.0f, window.y)
	);
}

void button(int button, int action) {
	const glm::vec2& cursor = oe::graphics::Window::getMousePos();
	gui->cursor(button, action, cursor.x, cursor.y);
}

int main() {
	// engine
	oe::init();

	// window
	oe::graphics::Window::init(900, 600, "Test 2 - Text", WINDOW_GL_DEBUG);
	oe::graphics::Window::setCursorPositionCallback(cursor_pos);
	oe::graphics::Window::setResizeCallback(resize);
	oe::graphics::Window::setButtonCallback(button);
	oe::graphics::GL::setBackFaceCulling(false);
	oe::graphics::GL::enableBlending();

	// drawing
	font = new oe::graphics::Font();
	oe::graphics::Text::setFont(*font);
	gui = new oe::gui::GUI();
	setup_gui();
	renderer = new oe::graphics::Renderer(oe::graphics::types::dynamicrender, oe::graphics::types::staticrender, 6, nullptr);
	shader = new oe::graphics::SingleTextureShader();
	resize(oe::graphics::Window::getWidth(), oe::graphics::Window::getHeight());

	// start
	oe::utils::GameLoop::init(render, update, 1);

	// closing
	oe::graphics::Window::close();
	delete gui;
	delete font;
	delete renderer;
	delete shader;

	return 0;
}