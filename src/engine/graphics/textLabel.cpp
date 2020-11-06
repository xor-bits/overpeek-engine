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
		spdlog::debug("text label res: {}", m_resolution);
		m_size = BasicText<char_type>::size(m_font, text, glm::vec2(static_cast<float>(m_resolution)));
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
		BasicText<char_type>::submit(tbr.fb_renderer, m_font, text, { 0.0f, 0.0f }, static_cast<float>(m_resolution), alignments::top_left);
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
	


	template<typename string_t>
	void create_text_render_cache(text_render_cache& cache, Font& font, const string_t& text, const glm::vec2& origin_pos, const glm::vec2& size, const glm::vec4& default_color, const glm::vec2& align_to_origin)
	{
		// at the very least the size of the text
		// \0, \n, etc... will be removed
		cache.datapoints.reserve(text.string.size());

		glm::vec2 advance = origin_pos;
		glm::vec4 color = default_color;
		const oe::graphics::Font::Glyph* glyph = nullptr;
		char32_t codepoint = U'\0';
		size_t i = 0;

		for (; i < text.string.size(); i++)
		{
			// get the codepoint from the input text at i
			codepoint = static_cast<char32_t>(text.string.at(i));

			// test if the codepoint will be rendered
			// and advancing the positioning of the following characters
			if(codepoint == U'\n') // new line
			{
				advance.x = pos.x;
				advance.y += size.y;
				continue;
			}
			if(c == U'\t') // tab
			{
				advance.x = std::ceil(advance.x / size.x) * size.x;
				continue;
			}

			// get glyph, if possible
			glyph = font.getGlyph(codepoint);
			// get 0 glyph as the codepoint did not contain any glyphs
			if (!glyph) glyph = font.getGlyph(0);
			// unexpectedly no 0 glyph
			if (!glyph) throw std::runtime_error("Unexpected NULL glyph at slot 0");

			// insert new datapoint
			cache.datapoints.emplace_back();
			auto& back = cache.datapoints.back();
			back.codepoint = codepoint;
			back.color = color;
			back.position = (advance) + (glyph->top_left * size) + glm::vec2{ 0.0f, -font.m_topmost * size.y };
			back.size = glyph->size;
			back.sprite = glyph->sprite;

			// advancing
			advance += glm::vec2(glyph->advance.x, 0.0f) * size;
			// kerning if possible
			if(i+1 < text.string.size())
				advance.x += font.getKerning(text.string.at(i), text.string.at(i+1)) * size.x;
		}

		// alignment
		glm::vec2 min = origin_pos, max = origin_pos;
		for (auto& datapoint : cache.datapoints)
		{
			min = glm::min(min, datapoint.position);
			max = glm::max(max, datapoint.position + datapoint.size);
		}
		const glm::vec2 size = glm::abs(max - min);
		const glm::vec2 move = oe::alignmentOffset(size, align_to_origin);
		if(align_to_origin != oe::alignments::top_left || min != origin_pos)
			for (auto& datapoint : cache.datapoints)
				datapoint.position -= move;
		
		cache.top_left = min;
		cache.size = size;
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
				continue;

			advance += glm::vec2(glyph->advance.x, 0.0f) * size;
			if(i+1 < text.string.size())
				advance.x += font.getKerning(text.string.at(i), text.string.at(i+1)) * size.x;
		}

		return pos + advance;
	}

	template<typename char_type>
	void BasicText<char_type>::submit(Renderer& renderer, Font& font, const string_t& text, const glm::vec2& pos, const glm::vec2& size, const glm::vec2& align)
	{
		text_render_cache cache;
		create_text_render_cache<char_type>(cache, font, text, pos, size, colo);

		// get width
		glm::vec2 advance = (align == glm::vec2{ 0.0f, 0.0f } ? align : alignmentOffset(-calculate_final_size<char_type>(text, size, font), align));
		
		const oe::graphics::Font::Glyph* glyph;
		oe::color col = text.string_color_map.at(0);
		for (size_t i = 0; i < text.string.size(); i++) {
			char_type c;
			if (index_to_char(i, c, glyph, text, advance, size, font))
				continue;

			auto iter = text.string_color_map.find(i);
			if(iter != text.string_color_map.end())
				col = iter->second;

			auto quad = renderer.create();
			quad->setPosition(pos + advance + glyph->top_left * size + glm::vec2(0.0f, - font.m_topmost * size.y));
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