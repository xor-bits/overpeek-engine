#include "gui_manager.hpp"

#include "engine/assets/default_shader/default_shader.hpp"
#include "engine/graphics/interface/window.hpp"
#include "engine/graphics/renderer.hpp"
#include "engine/graphics/font.hpp"
#include "engine/include.hpp"



namespace oe::gui
{
	GUI::GUI(const oe::graphics::Window& window, const oe::utils::FontFile& font_file, int32_t renderer_primitive_count)
		: m_window(window)
		, m_default_font_file(font_file.getID() == 0 ? oe::utils::FontFile{oe::default_full_font_path} : font_file)
	{
		// renderer
		m_renderer = new oe::graphics::Renderer(renderer_primitive_count);
		m_line_renderer = new oe::graphics::Renderer(renderer_primitive_count);

		// shader
		m_shader_fill = new oe::assets::DefaultShader(oe::polygon_mode::fill);
		m_shader_lines = new oe::assets::DefaultShader(oe::polygon_mode::lines);

		FormInfo form_info;
		form_info.widget_info = { m_window->getSize() - glm::ivec2(2 * border), { border, border }, oe::alignments::top_left, oe::alignments::top_left };
		m_main_frame = std::make_shared<Form>(nullptr, *this, form_info);
		m_old_render_size = { 0.0f, 0.0f };
		
		m_offset = { 0, 0 };
		m_size_mult = { 1.0f, 1.0f };
		updateModelMatrix();

		// initial resize
		short_resize();
		
		// event listeners
		m_cg_resize.connect<ResizeEvent, &GUI::on_resize, GUI>(m_window, this);
		m_cg_codepoint.connect<CodepointEvent, &GUI::on_codepoint, GUI>(m_window, this);
		m_cg_key.connect<KeyboardEvent, &GUI::on_key, GUI>(m_window, this);
		m_cg_cursor_pos.connect<CursorPosEvent, &GUI::on_cursor_pos, GUI>(m_window, this);
		m_cg_button.connect<MouseButtonEvent, &GUI::on_button, GUI>(m_window, this);
		m_cg_scroll.connect<ScrollEvent, &GUI::on_scroll, GUI>(m_window, this);
	}

	GUI::~GUI()
	{
		delete m_renderer;
		delete m_line_renderer;
		delete m_shader_fill;
		delete m_shader_lines;
	}

	void GUI::offset(const glm::vec2& offset)
	{
		bool changed = m_offset != offset;
		m_offset = offset;

		if(changed)
			updateModelMatrix();
	}

	void GUI::zoom(const glm::vec2& mult)
	{
		bool changed = m_size_mult != mult;
		m_size_mult = mult;

		if(changed)
			updateModelMatrix();
	}

	void GUI::updateModelMatrix()
	{
		m_render_ml_matrix = glm::mat4(1.0f);
		m_render_ml_matrix = glm::translate(m_render_ml_matrix, glm::vec3(m_offset, 0.0f));
		m_render_ml_matrix = glm::scale(m_render_ml_matrix, glm::vec3(1.0f / m_size_mult, 1.0f));
		m_render_ml_matrix = glm::translate(m_render_ml_matrix, glm::vec3((m_size_mult - 1.0f) / 2.0f * m_old_render_size, 0.0f));

		m_cursor_ml_matrix = glm::mat4(1.0f);
		m_cursor_ml_matrix = glm::translate(m_cursor_ml_matrix, glm::vec3(m_offset, 0.0f));
		m_cursor_ml_matrix = glm::translate(m_cursor_ml_matrix, -glm::vec3(m_old_render_size * (m_size_mult - 1.0f) / 2.0f, 0.0f));
		m_cursor_ml_matrix = glm::scale(m_cursor_ml_matrix, glm::vec3(m_size_mult, 1.0f));

		m_shader_fill->bind();
		m_shader_fill->setModelMatrix(m_render_ml_matrix);
		m_shader_lines->bind();
		m_shader_lines->setModelMatrix(m_render_ml_matrix);
	}

	void GUI::render()
	{
		auto& engine = oe::Engine::getSingleton();
		auto old_depth = engine.getDepth();
		engine.depth(oe::depth_functions::less_than_or_equal);

		render_empty();

		m_shader_fill->bind();
		m_renderer->render();
		// m_shader_lines->bind();
		// m_line_renderer->render();
		
		engine.depth(old_depth);
	}

	void GUI::render_empty()
	{
		short_resize();
		
		dispatcher.trigger(GUIPreRenderEvent{});
		dispatcher.trigger(GUIRenderEvent{});
	}

	void GUI::short_resize()
	{
		latest_resize_event.framebuffer_size_old = latest_resize_event.framebuffer_size;
		latest_resize_event.framebuffer_size = m_window->getWindowInfo().size;
		on_resize(latest_resize_event);
	}

	oe::graphics::Font& GUI::getFont(uint16_t resolution, const oe::utils::FontFile& _font)
	{
		const oe::utils::FontFile& font = _font.getID() == 0 ? m_default_font_file : _font;

		m_fontmap[resolution].try_emplace(font, resolution, font);
		return m_fontmap.at(resolution).at(font);
	}

	void GUI::on_resize(const oe::ResizeEvent& event)
	{
		m_main_frame->m_info.size = static_cast<glm::vec2>(event.framebuffer_size) - glm::vec2(2 * border);
		m_main_frame->m_info.offset_position = { border, border };

		/*
		                             wn
					/""""""""""""""""""""""""""""""""""\
		                x 
		            /"""""""\
		  --     -- +----------------------------------+
          :     y:  |                w0                |
		  :      :  |       /""""""""""""""""""\       |
		  :      -- |    -- +------------------+       |
		  :         |    :  |                  |       |
		hn:         |  h0:  |       gui        |       |
		  :         |    :  |                  |       |
		  :         |    -- +------------------+       |
          :         |           zoomed area            |
          :         |                                  |
		  --        +----------------------------------+
		
		yes, I just spent my time on this
		*/

		const glm::vec2 render_size = static_cast<glm::vec2>(event.framebuffer_size);

		if (m_old_render_size == render_size) return;
		glm::mat4 pr_matrix = glm::ortho(0.0f, render_size.x, render_size.y, 0.0f, -100000.0f, 100000.0f);
		m_shader_fill->bind();
		m_shader_fill->setTexture(true);
		m_shader_fill->setProjectionMatrix(pr_matrix);
		m_shader_lines->bind();
		m_shader_lines->setTexture(true);
		m_shader_lines->setProjectionMatrix(pr_matrix);
		m_old_render_size = render_size;
		
		dispatcher.trigger(event);
	}

	void GUI::on_codepoint(const CodepointEvent& event)
	{
		dispatcher.trigger(event);
	}

	void GUI::on_key(const KeyboardEvent& event)
	{
		dispatcher.trigger(event);
	}

	void GUI::on_cursor_pos(const CursorPosEvent& event)
	{
		CursorPosEvent copy = event;
		copy.cursor_windowspace = m_cursor_ml_matrix * glm::vec4(event.cursor_windowspace, 0.0f, 1.0f);
		dispatcher.trigger(copy);
	}

	void GUI::on_button(const MouseButtonEvent& event)
	{
		MouseButtonEvent copy = event;
		copy.cursor_pos.cursor_windowspace = m_cursor_ml_matrix * glm::vec4(event.cursor_pos.cursor_windowspace, 0.0f, 1.0f);
		dispatcher.trigger(copy);
		
		/* auto debug = m_renderer->create();
		debug->setPosition(copy.cursor_pos.cursor_windowspace);
		debug->setSize({ 50, 50 });
		debug->update();
		m_renderer->forget(std::move(debug)); */
	}

	void GUI::on_scroll(const ScrollEvent& event)
	{
		dispatcher.trigger(event);
	}
}