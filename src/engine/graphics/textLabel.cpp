#include "textLabel.hpp"

#include <sstream>
#include <iostream>
#include <ostream>

#include "engine/engine.hpp"



namespace oe::graphics {

	textrenderData textTorenderData(const std::string& text) {
		textrenderData renderData = textrenderData();

		bool nextIsColor = false;
		glm::vec4 curColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		std::stringstream ss;
		for (int i = 0; i < text.size(); i++) {
			char c = text[i];

			// "blablabla<#00ff00>"
			// check if there's upcoming colorcode
			if (oe::utils::checkChar(text, '<', i) && oe::utils::checkChar(text, '>', i + 8)) {
				long hex = oe::utils::stringToHex(text.substr(i + 1));
				if (hex != -1)
				{
					glm::vec3 rgb = oe::utils::hexToRGB(hex);
					curColor = glm::vec4(rgb / 255.0f, 1.0f); // alpha 1.0

					i += 8;
					continue;
				}
				else
				{
					// Invalid hex code
				}
			}
			
			renderData.push_back(std::pair(c, curColor));
		}
		return renderData;
	}


	
	oe::assets::DefaultShader* TextLabel::fb_shader = nullptr;
	oe::assets::DefaultShader* TextLabel::getFBShader()
	{
		if (!fb_shader)
		{
			fb_shader = new oe::assets::DefaultShader();
		}
		return fb_shader; 
	}
	
	oe::graphics::Renderer* TextLabel::fb_renderer = nullptr;
	oe::graphics::Renderer* TextLabel::getFBRenderer() 
	{ 
		if (!fb_renderer)
		{
			oe::RendererInfo r_info = {};
			r_info.max_primitive_count = 2048;
			fb_renderer = oe::Engine::getSingleton().createRenderer(r_info); 
		}
		return fb_renderer; 
	}

	TextLabel::TextLabel(const Font* font)
		: m_sprite(nullptr)
		, m_text("")
		, m_font(font)
		, m_framebuffer(nullptr)
		, m_fb_size(0.0f, 0.0f)
	{}

	TextLabel::TextLabel()
		: m_sprite(nullptr)
		, m_text("")
		, m_font(Text::getFont())
		, m_framebuffer(nullptr)
		, m_fb_size(0.0f, 0.0f)
	{}

	void TextLabel::generate(const std::string& text, Window* window, const glm::vec4& color)
	{
		if (m_text == text && initial_generated) return; // why render the same image again?
		regenerate(text, window, color);
	}
	
	void TextLabel::regenerate(const std::string& text, Window* window, const glm::vec4& color)
	{
		m_text = text;
		initial_generated = true;

		float multisamples = static_cast<float>(window->m_window_info.multisamples); // should I make this function arg or keep like this

		// size of the framebuffer
		m_size = Text::size(text, glm::vec2(m_font->m_resolution * multisamples), m_font);
		m_size.x = std::max(m_size.x, 1.0f);
		m_size.y = std::max(m_size.y, 1.0f);

		// create the framebuffer
		oe::FrameBufferInfo fb_info = {};
		fb_info.width = static_cast<size_t>(m_size.x) * 2; // double the required size, to make room for future reuse
		fb_info.height = static_cast<size_t>(m_size.y) * 2; // ^^
		if (m_framebuffer && m_fb_size.x >= m_size.x && m_fb_size.y >= m_size.y)
		{
			// reuse the old framebuffer
		}
		else if (m_framebuffer)
		{
			// regen framebuffer
			oe::Engine::getSingleton().destroyFrameBuffer(m_framebuffer);
			m_framebuffer = oe::Engine::getSingleton().createFrameBuffer(fb_info, window);
		}
		else
		{
			// create new framebuffer
			m_framebuffer = oe::Engine::getSingleton().createFrameBuffer(fb_info, window);
		}
		m_fb_size = m_size * 2.0f;

		m_framebuffer->bind();
		m_framebuffer->clear(color);

		// render to the framebuffer
		auto fb_renderer = getFBRenderer();
		auto fb_shader = getFBShader();
		Text::submit(fb_renderer, text, { 0.0f, 0.0f }, m_font->m_resolution * multisamples, alignments::top_left, glm::vec4(1.0f), m_font);
		glm::mat4 pr_matrix = glm::ortho(0.0f, m_size.x, m_size.y, 0.0f);
		fb_shader->setProjectionMatrix(pr_matrix);
		fb_shader->bind();
		fb_renderer->render();
		fb_renderer->clear();
		
		fb_shader->unbind(); // just so the user couldnt accidentally modify this shader

		// bind window framebuffer
		window->bind();

		// generate the sprite for user
		m_sprite = Sprite(m_framebuffer->getTexture());
		m_sprite.position = { 0.0f, 1.0f };
		m_sprite.size = { 1.0f, -1.0f };
		m_size /= m_font->m_resolution * multisamples;
	}
	


	const Font* Text::s_font;

	void Text::setFont(const Font& font)
	{
		s_font = &font;
	}

	const Font* Text::getFont()
	{
		return s_font;
	}

	bool index_to_char(size_t i, unsigned char& c, oe::graphics::Font::Glyph const *& glyph, const textrenderData& renderData, glm::vec2& advance, const glm::vec2& size, const Font* font)
	{
		c = renderData.at(i).first;
		glyph = font->getGlyph(c);
		if (!glyph) glyph = font->getGlyph(0);

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
		return false;
	}

	glm::vec2 calculate_final_size(const textrenderData& renderData, const glm::vec2& size, const Font* font)
	{
		float left(0.0f);
		float right(0.0f);

		glm::vec2 advance = { 0.0f, 0.0f };
		const oe::graphics::Font::Glyph* glyph;
		for (size_t i = 0; i < renderData.size(); i++) {
			unsigned char c;
			if (index_to_char(i, c, glyph, renderData, advance, size, font))
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

	glm::vec2 Text::size(const std::string& text, const glm::vec2& size, const Font* font)
	{
		if (!font) font = s_font;
		if (!font) oe_error_terminate("No font!");

		textrenderData renderData = textTorenderData(text);
		return calculate_final_size(renderData, size, font);
	}

	void Text::submit(Renderer* renderer, const std::string& text, const glm::vec2& pos, const glm::vec2& size, const glm::vec2& align, const glm::vec4& bg_color, const Font* font)
	{
		if (!font) font = s_font;
		if (!font) oe_error_terminate("No font!");

		textrenderData renderData = textTorenderData(text);
		float line = font->bb_max_height * size.y;
		const glm::vec2 final_size = calculate_final_size(renderData, size, font);
		
		// get width
		glm::vec2 advance = alignmentOffset(-final_size, align);
		const oe::graphics::Font::Glyph* glyph;
		for (size_t i = 0; i < renderData.size(); i++) {
			unsigned char c;
			if (index_to_char(i, c, glyph, renderData, advance, size, font))
			{
				continue;
			}

			auto quad = renderer->createQuad();
			quad->setPosition(pos + advance + glyph->top_left * size + glm::vec2{ 0.0, line });
			quad->setSize(glyph->size * size);
			quad->setSprite(glyph->sprite);
			quad->setColor(renderData[i].second);
			quad->update();
			advance += glm::vec2(glyph->advance.x, 0.0f) * size;
		}
	}

}