#include "gui_manager.h"

#include <engine/graphics/window.h>
#include <engine/graphics/renderer.h>
#include <engine/graphics/textureManager.h>
#include <engine/graphics/assets/multiTextureShader.h>



namespace oe::gui {

	constexpr int border = 5;

	std::unique_ptr<Form> m_main_frame;
	oe::graphics::Renderer* m_renderer;
	oe::graphics::Renderer* m_font_renderer;
	oe::graphics::MultiTextureShader* m_shader;
	oe::graphics::Font* m_font;
	
	GUI::GUI() {
		m_renderer = new oe::graphics::Renderer(oe::graphics::dynamicrender, oe::graphics::staticrender, 10000);
		m_font_renderer = new oe::graphics::Renderer(oe::graphics::dynamicrender, oe::graphics::staticrender, 10000);
		m_shader = new oe::graphics::MultiTextureShader();
		m_font = new oe::graphics::Font();

		auto form = new oe::gui::Form(oe::graphics::Window::getSize() - glm::vec2(2 * border));
		form->size() = { border, border };
		m_main_frame = std::unique_ptr<Form>(form);
		resize();
	}

	GUI::~GUI() {
		delete m_font;
		delete m_renderer;
		delete m_font_renderer;
		delete m_shader;
	}

	void GUI::render() {
		m_renderer->begin();
		m_renderer->clear();
		m_font_renderer->begin();
		m_font_renderer->clear();

		if (m_main_frame) m_main_frame->__render(*m_renderer, *m_font_renderer);
		
#if _DEBUG && 0
		oe::graphics::GL::setPolygonMode(1);
#else
#endif // _DEBUG
		m_shader->useTexture(false);
		m_renderer->end();
		m_renderer->render();

		oe::graphics::GL::setPolygonMode(0);
		m_font->bindTexture();
		m_shader->useTexture(true);
		m_font_renderer->end();
		m_font_renderer->render();
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

}

