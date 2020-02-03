#include "gui_manager.h"

#include <engine/graphics/window.h>
#include <engine/graphics/renderer.h>
#include <engine/graphics/text/font.h>
#include <engine/graphics/assets/singleTextureShader.h>



namespace oe::gui {

	constexpr int border = 5;

	std::unique_ptr<Form> m_main_frame;
	oe::graphics::Renderer* m_renderer;
	oe::graphics::Renderer* m_font_renderer;
	oe::graphics::SpriteShader* m_shader;
	
	GUI::GUI() {
		m_renderer = new oe::graphics::Renderer(oe::graphics::dynamicrender, oe::graphics::staticrender, 10000);
		m_shader = new oe::graphics::SpriteShader();

		auto form = new oe::gui::Form(oe::graphics::Window::getSize() - glm::vec2(2 * border));
		form->size() = { border, border };
		m_main_frame = std::unique_ptr<Form>(form);
		resize();
	}

	GUI::~GUI() {
		delete m_renderer;
		delete m_shader;
	}

	void GUI::render() {
		m_renderer->begin();
		m_renderer->clear();

		if (m_main_frame) m_main_frame->__render(*m_renderer);
		
		m_shader->bind();
		m_renderer->end();
		m_renderer->render();
	}

	void GUI::resize() {
		resize(oe::graphics::Window::getSize());
	}

	void GUI::resize(const glm::vec2& window_size) {
		m_main_frame->size() = window_size - glm::vec2(2 * border);
		m_main_frame->offset_position() = { border, border };
		m_main_frame->__resize();

		/*
		    0                  0
		  0 +------------------+ w
		    |                  |
		    |                  |
		    |                  |
		  0 +------------------+ w
		    h                  h
		*/

		glm::mat4 pr_matrix = glm::ortho(0.0f, (float)window_size.x, (float)window_size.y, 0.0f);
		m_shader->bind();
		m_shader->useTexture(true);
		m_shader->projectionMatrix(pr_matrix);
	}

	void GUI::addSubWidget(Widget* widget) {
		m_main_frame->addSubWidget(widget);
	}

	void GUI::cursor(int button, int action, const glm::vec2& cursor_window) {
		m_main_frame->__cursor(button, action, cursor_window);
	}

	void GUI::text(unsigned int character, int mods) {
		m_main_frame->__text(character, mods);
	}

	void GUI::key(int key, int action, int mods) {
		m_main_frame->__key(key, action, mods);
	}

}

