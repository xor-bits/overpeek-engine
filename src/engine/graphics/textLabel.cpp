#include "textLabel.hpp"

#include <sstream>
#include <iostream>
#include <ostream>

#include "font.hpp"
#include "engine/engine.hpp"
#include "engine/graphics/interface/window.hpp"
#include "engine/graphics/interface/framebuffer.hpp"
#include "engine/graphics/renderer.hpp"
#include "engine/assets/font_shader/font_shader.hpp"



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

		oe::assets::FontShader fb_shader;
		oe::graphics::Renderer fb_renderer;
	};
	TextLabelRenderer* TextLabelRenderer::singleton = nullptr;



	template<typename char_type>
	void BasicTextLabel<char_type>::generate(const string_t& text, const Window& window, const oe::color& c, float width, float outline_width, const oe::color& outline_c)
	{
		if (m_text == text && initial_generated) return; // why render the same text again?
		regenerate(text, window, c, width, outline_width, outline_c);
	}
	
	template<typename char_type>
	void BasicTextLabel<char_type>::regenerate(const string_t& text, const Window& window, const oe::color& c, float width, float outline_width, const oe::color& outline_c)
	{
		m_text = text;
		initial_generated = true;

		// size of the framebuffer
		m_size = BasicText<char_type>::size(m_font, text, glm::vec2(m_font.getResolution() * m_res_mult));
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
		m_framebuffer->clear(c);

		// render to the framebuffer
		auto& tbr = TextLabelRenderer::getSingleton();
		BasicText<char_type>::submit(tbr.fb_renderer, m_font, text, { 0.0f, 0.0f }, m_font.getResolution() * m_res_mult, alignments::top_left);
		glm::mat4 pr_matrix = glm::ortho(0.0f, m_fb_size.x, m_fb_size.y, 0.0f);
		tbr.fb_shader.setProjectionMatrix(pr_matrix);
		tbr.fb_shader.setSDF(m_font.isSDF());
		tbr.fb_shader.setWidth(width);
		tbr.fb_shader.setOutlineWidth(outline_width);
		tbr.fb_shader.setOutlineColor(outline_c);
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
		if (!glyph)
			glyph = font.getGlyph(0);
		if (!glyph)
			throw std::runtime_error("Unexpected NULL glyph at slot 0");
		
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
	glm::vec2 BasicText<char_type>::charpos(Font& font, const string_t& text, size_t first, size_t idx, const glm::vec2& pos, const glm::vec2& size, const glm::vec2& align)
	{
		// get width
		glm::vec2 advance = (align == glm::vec2{ 0.0f, 0.0f } ? align : alignmentOffset(-calculate_final_size<char_type>(text, size, font), align));
		const oe::graphics::Font::Glyph* glyph;
		for (size_t i = first; i < std::min(idx, text.string.size()); i++) {
			char_type c;
			if (index_to_char(i, c, glyph, text, advance, size, font))
			{
				continue;
			}

			advance += glm::vec2(glyph->advance.x, 0.0f) * size;
		}

		return pos + advance;
	}

	template<typename char_type>
	void BasicText<char_type>::submit(Renderer& renderer, Font& font, const string_t& text, const glm::vec2& pos, const glm::vec2& size, const glm::vec2& align)
	{
		const float avg_top = font.getGlyph('|')->top_left.y;
	
		// get width
		glm::vec2 advance = (align == glm::vec2{ 0.0f, 0.0f } ? align : alignmentOffset(-calculate_final_size<char_type>(text, size, font), align));
		const oe::graphics::Font::Glyph* glyph;
		oe::color col = text.string_color_map.at(0);
		for (size_t i = 0; i < text.string.size(); i++) {
			char_type c;
			if (index_to_char(i, c, glyph, text, advance, size, font))
			{
				continue;
			}

			auto iter = text.string_color_map.find(i);
			if(iter != text.string_color_map.end())
				col = iter->second;

			auto quad = renderer.create();
			quad->setPosition(pos + advance + glyph->top_left * size - glm::vec2(0.0f, avg_top * size.y));
			quad->setSize(glyph->size * size);
			quad->setSprite(glyph->sprite);
			quad->setColor(col);
			
			renderer.forget(std::move(quad));

			advance += glm::vec2(glyph->advance.x, 0.0f) * size;
			if(i+1 < text.string.size())
				advance.x += font.getKerning(text.string.at(i), text.string.at(i+1)) * size.x;
		}
	}
	
	template class BasicText<char>;
	template class BasicText<wchar_t>;
	template class BasicText<char16_t>;
	template class BasicText<char32_t>;
}