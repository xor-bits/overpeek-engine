#include "gui_manager.hpp"

#include "engine/graphics/interface/window.hpp"
#include "engine/graphics/interface/renderer.hpp"
#include "engine/graphics/font.hpp"
#include "widgets/form.hpp"
#include "engine/include.hpp"



namespace oe::gui {

	constexpr int border = 5;

	
	GUI::GUI(oe::graphics::Window* window) 
		: m_window(window)
	{
		// renderer
		oe::RendererInfo renderer_info = {};
		renderer_info.arrayRenderType = oe::types::dynamicrender;
		renderer_info.indexRenderType = oe::types::staticrender;
		renderer_info.max_primitive_count = 10000;
		renderer_info.staticVBOBuffer_data = nullptr;
		m_renderer = oe::Engine::getSingleton().createRenderer(renderer_info);
		m_late_renderer = oe::Engine::getSingleton().createRenderer(renderer_info);

		// shader
		m_shader = new oe::assets::DefaultShader();

		FormInfo form_info = {};
		form_info.size = m_window->getSize() - glm::vec2(2 * border);
		form_info.offset_position = { border, border };
		m_main_frame = new oe::gui::Form(this, form_info);
		m_offset = { 0, 0 };
		m_old_window_size = { 0, 0 };
		resize();
	}

	GUI::~GUI() {
		delete m_main_frame;
		oe::Engine::getSingleton().destroyRenderer(m_renderer);
		delete m_shader;
	}

	void GUI::offset(const glm::vec2& offset) {
		m_offset = offset;

		glm::mat4 ml_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(offset, 0.0f));
		m_shader->bind();
		m_shader->setModelMatrix(ml_matrix);
	}

	void GUI::render() {
		resize();

		auto& engine = oe::Engine::getSingleton();
		auto old_depth = engine.getDepth();
		engine.depth(oe::depth_functions::always);

		float z = 0.0f;
		if (m_main_frame) m_main_frame->__render(z, m_renderer);
		m_shader->bind();
		m_renderer->render();
		m_late_renderer->render();
		
		engine.depth(old_depth);
	}

	void GUI::render_empty()
	{
		resize();
		float z = 0.0f;
		if (m_main_frame) m_main_frame->__render(z, m_renderer);
	}

	void GUI::resize() {
		resize(m_window->getSize());
	}

	void GUI::resize(const glm::vec2& window_size) {
		m_main_frame->size = window_size - glm::vec2(2 * border);
		m_main_frame->offset_position = { border, border };
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

		if (m_old_window_size == window_size) return;
		glm::mat4 pr_matrix = glm::ortho(0.0f, (float)window_size.x, (float)window_size.y, 0.0f, -100000.0f, 10.0f);
		// glm::mat4 pr_matrix = glm::perspectiveFov(60.0f, (float)window_size.x, (float)window_size.y, 0.0f, 1000.0f);
		// pr_matrix = glm::lookAt(glm::vec3{ 0.0f, 0.0f, 50.0f }, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, -1.0f, 0.0f}) * pr_matrix;
		m_shader->bind();
		m_shader->useTexture(true);
		m_shader->setProjectionMatrix(pr_matrix);
		m_old_window_size = window_size;
	}

	void GUI::addSubWidget(Widget* widget) {
		m_main_frame->addSubWidget(widget);
		render_empty();
	}

	void GUI::cursor(oe::mouse_buttons button, oe::actions action, const glm::vec2& cursor_window) {
		glm::vec2 cursor_window_final = cursor_window - m_offset;

		m_main_frame->__cursor(button, action, cursor_window_final);
	}

	void GUI::text(uint32_t codepoint, oe::modifiers mods) {
		m_main_frame->__text(codepoint, mods);
	}

	void GUI::key(oe::keys key, oe::actions action, oe::modifiers mods) {
		m_main_frame->__key(key, action, mods);
	}

}

