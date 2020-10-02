#include "textLabel.hpp"

#include <sstream>
#include <iostream>
#include <ostream>

#include "font.hpp"
#include "engine/engine.hpp"
#include "engine/graphics/interface/window.hpp"
#include "engine/graphics/interface/framebuffer.hpp"
#include "engine/graphics/renderer.hpp"
#include "engine/assets/default_shader/default_shader.hpp"



namespace oe::graphics {
	
	class TextLabelRenderer
	{
	private:
		static TextLabelRenderer* singleton;
		TextLabelRenderer(const TextLabelRenderer& copy) = delete;
		TextLabelRenderer()
			: fb_shader()
			, fb_renderer(2048)
		{}

	public:
		static TextLabelRenderer& getSingleton() { if(!singleton) singleton = new TextLabelRenderer(); return *singleton; }

		oe::assets::DefaultShader fb_shader;
		oe::graphics::Renderer fb_renderer;
	};
	TextLabelRenderer* TextLabelRenderer::singleton = nullptr;



	template<typename char_type>
	void BasicTextLabel<char_type>::generate(const string_t& text, const Window& window, const glm::vec4& color)
	{
		if (m_text == text && initial_generated) return; // why render the same text again?
		regenerate(text, window, color);
	}
	
	template<typename char_type>
	void BasicTextLabel<char_type>::regenerate(const string_t& text, const Window& window, const glm::vec4& color)
	{
		m_text = text;
		initial_generated = true;

		// size of the framebuffer
		m_size = BasicText<char_type>::size(m_font, text, glm::vec2(m_font.getResolution()));
		m_size.x = std::max(m_size.x, 1.0f);
		m_size.y = std::max(m_size.y, 1.0f);

		if (!m_framebuffer || m_fb_size.x < m_size.x || m_fb_size.y < m_size.y)
		{
			// create new framebuffer
			oe::FrameBufferInfo fb_info = {};
			fb_info.size = static_cast<glm::ivec2>(m_size) * 2; // double the required size, to make room for future reuse
			m_framebuffer = FrameBuffer(fb_info);
			
			m_fb_size = fb_info.size;
		}

		m_framebuffer->bind();
		m_framebuffer->clear(color);

		// render to the framebuffer
		auto& tbr = TextLabelRenderer::getSingleton();
		BasicText<char_type>::submit(tbr.fb_renderer, m_font, text, { 0.0f, 0.0f }, m_font.getResolution(), alignments::top_left, glm::vec4(1.0f));
		glm::mat4 pr_matrix = glm::ortho(0.0f, m_fb_size.x, m_fb_size.y, 0.0f);
		tbr.fb_shader.setProjectionMatrix(pr_matrix);
		tbr.fb_shader.bind();
		tbr.fb_renderer.render();
		tbr.fb_renderer.clear();
		
		tbr.fb_shader.unbind(); // just so the user couldnt accidentally modify this shader

		// bind window framebuffer
		window->bind();

		// generate the sprite for user
		const glm::vec2 size_ratio = m_size / m_fb_size;

		m_sprite.m_owner = m_framebuffer->getTexture();
		m_sprite.position = { 0.0f, 1.0f };
		m_sprite.size = { size_ratio.x, -size_ratio.y };
		m_sprite.opacity = true;
	}
	
	template class BasicTextLabel<char>;
	template class BasicTextLabel<wchar_t>;
	template class BasicTextLabel<char16_t>;
	template class BasicTextLabel<char32_t>;
	
	// template<> void BasicTextLabel<char>::generate(const string_type& text, Window& window, const glm::vec4& color);
	// template<> void BasicTextLabel<wchar_t>::generate(const string_type& text, Window& window, const glm::vec4& color);
	// template<> void BasicTextLabel<char16_t>::generate(const string_type& text, Window& window, const glm::vec4& color);
	// template<> void BasicTextLabel<char32_t>::generate(const string_type& text, Window& window, const glm::vec4& color);
	// 
	// template<> void BasicTextLabel<char>::regenerate(const string_type& text, Window& window, const glm::vec4& color);
	// template<> void BasicTextLabel<wchar_t>::regenerate(const string_type& text, Window& window, const glm::vec4& color);
	// template<> void BasicTextLabel<char16_t>::regenerate(const string_type& text, Window& window, const glm::vec4& color);
	// template<> void BasicTextLabel<char32_t>::regenerate(const string_type& text, Window& window, const glm::vec4& color);

	template<typename char_type>
	bool index_to_char(size_t i, char_type& c, oe::graphics::Font::Glyph const *& glyph, const text_render_input<char_type>& text, glm::vec2& advance, const glm::vec2& size, Font& font)
	{
		c = text.string.at(i);

		if(c == '\n') // new line
		{
			advance.x = 0.0f;
			advance.y += size.y;
			return true;
		}
		if(c == '\t') // tab
		{
			advance.x = std::ceil(advance.x / size.x) * size.x;
			return true;
		}

		glyph = font.getGlyph(c);
		if (!glyph) glyph = font.getGlyph(0);
		if (!glyph) oe_error_terminate("NULL glyph in 0");
		
		return false;
	}

	template<typename char_type>
	glm::vec2 calculate_final_size(const text_render_input<char_type>& text, const glm::vec2& size, Font& font)
	{
		float left(0.0f);
		float right(0.0f);

		glm::vec2 advance = { 0.0f, 0.0f };
		const oe::graphics::Font::Glyph* glyph;
		for (size_t i = 0; i < text.string.size(); i++) {
			char_type c;
			if (index_to_char(i, c, glyph, text, advance, size, font))
			{
				continue;
			}

			const glm::vec2 glyph_top_left = advance + glyph->top_left * size;
			left = std::min(left, glyph_top_left.x);
			right = std::max(right, glyph_top_left.x + glyph->size.x * size.x);

			advance += glm::vec2(glyph->advance.x, 0.0f) * size;
		}

		return { right - left, advance.y + size.y };
	}

	template<typename char_type>
	glm::vec2 BasicText<char_type>::size(Font& font, const string_t& text, const glm::vec2& size)
	{
		return calculate_final_size(text, size, font);
	}

	template<typename char_type>
	void BasicText<char_type>::submit(Renderer& renderer, Font& font, const string_t& text, const glm::vec2& pos, const glm::vec2& size, const glm::vec2& align, const glm::vec4& bg_color)
	{
		const glm::vec2 final_size = calculate_final_size<char_type>(text, size, font);
		const float avg_top = font.getGlyph('|')->top_left.y;
	
		// get width
		glm::vec2 advance = alignmentOffset(-final_size, align);
		const oe::graphics::Font::Glyph* glyph;
		glm::vec4 color = text.string_color_map.at(0);
		for (size_t i = 0; i < text.string.size(); i++) {
			char_type c;
			if (index_to_char(i, c, glyph, text, advance, size, font))
			{
				continue;
			}

			auto iter = text.string_color_map.find(i);
			if(iter != text.string_color_map.end())
				color = iter->second;

			auto quad = renderer.create();
			quad->setPosition(pos + advance + glyph->top_left * size - glm::vec2(0.0f, avg_top * size.y));
			quad->setSize(glyph->size * size);
			quad->setSprite(glyph->sprite);
			quad->setColor(color);
			
			renderer.forget(std::move(quad));

			advance += glm::vec2(glyph->advance.x, 0.0f) * size;
		}
	}
	
	template class BasicText<char>;
	template class BasicText<wchar_t>;
	template class BasicText<char16_t>;
	template class BasicText<char32_t>;

}