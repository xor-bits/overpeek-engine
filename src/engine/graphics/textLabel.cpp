#include "textLabel.hpp"

#include <sstream>
#include <iostream>
#include <ostream>

#include "engine/engine.hpp"



namespace oe::graphics {

	glm::ivec3 hexToRGB(unsigned long hex) {
		return glm::vec3(
			(hex & 0xff0000) >> 16,
			(hex & 0x00ff00) >> 8,
			(hex & 0x0000ff) >> 0
		);
	}

	unsigned long RGBtoHex(glm::ivec3 rgb) {
		return rgb.x << 16 + rgb.y << 8 + rgb.z << 0;
	}

	bool checkChar(const std::string& text, char character, int position) {
		if (position >= text.size() || position < 0) {
			return false;
		}
		
		if (text[position] == character) {
			return true;
		}

		return false;
	}

	bool is_number(const char* input) {
		if (strchr(input, 'x')) return false; // no
		if (strchr(input, 'b')) return false; // hex
		if (strchr(input, 'o')) return false; // or oct or bin

		char* p = nullptr;
		long hex = strtol(input, &p, 16);
		return !*p;
	}

	textrenderData textTorenderData(const std::string& text) {
		textrenderData renderData = textrenderData();

		bool nextIsColor = false;
		glm::vec4 curColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		std::stringstream ss;
		for (int i = 0; i < text.size(); i++) {
			char c = text[i];

			// "blablabla<#00ff00>"
			// check if there's upcoming colorcode
			if (checkChar(text, '<', i) && checkChar(text, '#', i + 1) && checkChar(text, '>', i + 8)) {
				std::stringstream hex_str;
				hex_str << std::hex << text.substr(i + 2, 6);

				long hex = std::stol(hex_str.str().c_str(), nullptr, 16);
				if (is_number(hex_str.str().c_str()) && hex >= 0) {
					glm::vec3 rgb = hexToRGB(hex);
					curColor = glm::vec4(rgb / 255.0f, 1.0f); // alpha 1.0

					i += 8;
					continue;
				}
				// Invalid hex code
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
	{
		m_font = font;
		m_framebuffer = nullptr;
	}

	void TextLabel::generate(const std::string& text)
	{
		if (m_text == text) return; // why render the same image again?
		regenerate(text);
	}
	
	void TextLabel::regenerate(const std::string& text)
	{
		m_text = text;

		float width = Text::width(text, glm::vec2(m_font->m_resolution), m_font);
		glm::vec2 fb_size = { width, m_font->bb_height * m_font->m_resolution };
		spdlog::info("size of {} framebuffer is {}", text, fb_size);

		oe::FrameBufferInfo fb_info = {};
		fb_info.width = static_cast<size_t>(ceil(fb_size.x));
		fb_info.height = static_cast<size_t>(ceil(fb_size.y));
		m_framebuffer = oe::Engine::getSingleton().createFrameBuffer(fb_info);

		m_framebuffer->bind();
		m_framebuffer->clear({ 0.0f, 0.0f, 0.0f, 0.0f });

		Text::submit(getFBRenderer(), text, { 0.0f, 0.0f }, m_font->m_resolution, alignments::top_left, glm::vec4(0.0f), m_font);

		glm::mat4 pr_matrix = glm::ortho(0.0f, fb_size.x, fb_size.y, 0.0f);
		fb_shader->setProjectionMatrix(pr_matrix);
		getFBShader()->bind();
		getFBRenderer()->render();
		getFBRenderer()->destroyQuads();
		
		getFBShader()->unbind(); // just so the user couldnt accidentally modify this shader

		m_framebuffer->unbind();

		m_sprite = Sprite(m_framebuffer->getTexture());
		m_sprite.position = { 0.0f, 0.0f };
		m_sprite.size = { 1.0f, 1.0f };
	}
	


	const Font* Text::s_font;

	void Text::setFont(const Font& font) {
		s_font = &font;
	}

	const Font* Text::getFont() {
		return s_font;
	}

	float calculate_final_size(const textrenderData& renderData, const glm::vec2& size, const Font* font) {
		float first = 0.0f;
		float last = 0.0f;

		glm::vec2 advance = { 0.0f, 0.0f };
		for (size_t i = 0; i < renderData.size(); i++) {
			unsigned char c = renderData.at(i).first;
			auto glyph = font->getGlyph(c);
			if (!glyph) continue;

			// first
			if (i == 0) {
				first = advance.x + glyph->top_left.x * size.x;
			}
			if (i == renderData.size() - 1) {
				last = advance.x + glyph->top_left.x * size.x + glyph->size.x * size.x;
			}

			advance += glm::vec2(glyph->advance.x, 0.0f) * size;
		}

		return last - first;
	}

	float Text::width(const std::string& text, const glm::vec2& size, const Font* font) {
		if (!font) font = s_font;
		if (!font) oe_error_terminate("No font!");

		textrenderData renderData = textTorenderData(text);
		return calculate_final_size(renderData, size, font);
	}

	void Text::submit(Renderer* renderer, const std::string& text, const glm::vec2& pos, const glm::vec2& size, const glm::vec2& align, const glm::vec4& bg_color, const Font* font) {
		if (!font) font = s_font;
		if (!font) oe_error_terminate("No font!");

		textrenderData renderData = textTorenderData(text);
		float line = font->bb_max_height * size.y;
		float final_size = calculate_final_size(renderData, size, font);
		
		// get width
		glm::vec2 advance = alignmentOffset({ -final_size, -size.y }, align);
		for (pairData& pd : renderData) {
			unsigned char i = pd.first;
			auto glyph = font->getGlyph(i);
			if (!glyph) continue;
			auto sprite = glyph->sprite;

			auto quad = renderer->createQuad();
			quad->setPosition(pos + advance + glyph->top_left * size + glm::vec2{ 0.0, line });
			quad->setSize(glyph->size * size);
			quad->setSprite(sprite);
			quad->setColor(pd.second);
			quad->update();
			advance += glm::vec2(glyph->advance.x, 0.0f) * size;
		}
	}

}