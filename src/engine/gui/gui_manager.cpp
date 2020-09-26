#include "gui_manager.hpp"

#include "engine/assets/default_shader/default_shader.hpp"
#include "engine/graphics/interface/window.hpp"
#include "engine/graphics/renderer.hpp"
#include "engine/graphics/font.hpp"
#include "engine/include.hpp"



namespace oe::gui
{
	GUI::GUI(const oe::graphics::Window& window, const std::string& default_font, int32_t renderer_primitive_count)
		: m_window(window)
		, m_default_font_path(default_font)
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
		m_offset = { 0, 0 };
		m_old_window_size = { 0, 0 };

		// initial resize
		short_resize();
		
		// event listeners
		m_window->connect_listener<oe::ResizeEvent, &GUI::on_resize>(this);
	}

	GUI::~GUI()
	{
		delete m_renderer;
		delete m_line_renderer;
		delete m_shader_fill;
		delete m_shader_lines;
		
		// event listeners
		m_window->disconnect_listener<oe::ResizeEvent, &GUI::on_resize>(this);
	}

	void GUI::offset(const glm::vec2& offset)
	{
		m_offset = offset;

		glm::mat4 ml_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(offset, 0.0f));
		m_shader_fill->bind();
		m_shader_fill->setModelMatrix(ml_matrix);
		m_shader_lines->bind();
		m_shader_lines->setModelMatrix(ml_matrix);
	}

	void GUI::render()
	{
		auto& engine = oe::Engine::getSingleton();
		auto old_depth = engine.getDepth();
		engine.depth(oe::depth_functions::less_than_or_equal);

		render_empty();

		m_shader_fill->bind();
		if(m_needs_sorting) { m_renderer->sort(); m_needs_sorting = false; }
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
		latest_resize_event.framebuffer_size = m_window->getSize();
		on_resize(latest_resize_event);
	}

	void GUI::on_resize(const oe::ResizeEvent& event)
	{
		m_main_frame->m_info.size = static_cast<glm::vec2>(event.framebuffer_size) - glm::vec2(2 * border);
		m_main_frame->m_info.offset_position = { border, border };

		/*
		    0                  0
		  0 +------------------+ w
		    |                  |
		    |                  |
		    |                  |
		  0 +------------------+ w
		    h                  h
		*/

		if (m_old_window_size == event.framebuffer_size) return;
		glm::mat4 pr_matrix = glm::ortho(0.0f, (float)event.framebuffer_size.x, (float)event.framebuffer_size.y, 0.0f, -100000.0f, 100000.0f);
		m_shader_fill->bind();
		m_shader_fill->setTexture(true);
		m_shader_fill->setProjectionMatrix(pr_matrix);
		m_shader_lines->bind();
		m_shader_lines->setTexture(true);
		m_shader_lines->setProjectionMatrix(pr_matrix);
		m_old_window_size = event.framebuffer_size;
	}

	oe::graphics::Font& GUI::getFont(uint16_t resolution, const std::string& _font)
	{
		const std::string& font = _font.empty() ? m_default_font_path : _font;

		auto sizeiter = m_fontmap.find(resolution);
		if(sizeiter == m_fontmap.end())
		{
			auto font_ptr = new oe::graphics::Font(resolution, font);
			m_fontmap[resolution][font] = font_ptr;
			return *font_ptr;
		}

		auto fontiter = sizeiter->second.find(font);
		if(fontiter == sizeiter->second.end())
		{
			auto font_ptr = new oe::graphics::Font(resolution, font);
			m_fontmap[resolution][font] = font_ptr;
			return *font_ptr;
		}

		return *fontiter->second;
	}
}