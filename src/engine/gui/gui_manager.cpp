#include "gui_manager.h"

#include <engine/graphics/window.h>
#include <engine/graphics/renderer.h>
#include <engine/graphics/text/font.h>
#include <engine/graphics/assets/singleTextureShader.h>

#include "engine/engine.h"



namespace oe::gui {

	constexpr int border = 5;

	
	GUI::GUI(oe::graphics::Window* window) : m_window(window) {
		oe::RendererInfo renderer_info = {};
		renderer_info.arrayRenderType = oe::dynamicrender;
		renderer_info.indexRenderType = oe::staticrender;
		renderer_info.max_quad_count = 10000;
		renderer_info.staticVBOBuffer_data = nullptr;
		m_renderer = oe::Engine::createRenderer(renderer_info);

		oe::ShaderInfo shader_info = oe::graphics::SpriteShader::singleTextureShader();
		m_shader = oe::Engine::createShader(shader_info);

		auto form = new oe::gui::Form(m_window->getSize() - glm::vec2(2 * border));
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
		resize(m_window->getSize());
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
		m_shader->setUniform1i("usetex", 1);
		m_shader->setUniformMat4("pr_matrix", pr_matrix);
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

