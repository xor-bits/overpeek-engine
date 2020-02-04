#include "textLabel.h"

#include <sstream>
#include <iostream>
#include <ostream>

#include "engine/engine.h"



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

	TextLabel::TextLabel(const Font& font) {
		m_font = &font;
		m_framebuffer = nullptr;
	}

	void TextLabel::submit(Renderer& renderer, std::string text, glm::vec2 pos, glm::vec2 size) {
		Text::submit(renderer, text, pos, size, oe::graphics::alignment::top_left, glm::vec4(1.0f), m_font);
	}

	/*void TextLabel::generate(const std::string& text) {
		m_text = text;
		textrenderData renderData = textTorenderData(text);

		//Clear framebuffer (background)
		//label.buffer.clear(1.0f, 0.0f, 0.5f, 1.0f); //pink
		label.buffer.clear(0.0f, 0.0f, 0.0f, 0.0f); //transparent

		//render renders
		float xDelta = 0.0f;
		label_bake.clear();
		label_bake.begin();
		for (TextrenderData.CharColorPair cp : renderData.renderData) {
			Glyph glyph = glyphs.getGlyphData(cp.character);
			//float yDelta = glyph.y;
			label_bake.render(new Quad(new vec2(xDelta, glyphs.getHeight()), new vec2(glyphs.getWidth(), -glyphs.getHeight()), glyph.textureId, cp.color, Alignment.TOP_LEFT));
			xDelta += glyph.advance;
		}
		label_bake.end();

		//rendering
		label_bake_shader.bind();
		Texture tex = glyphs.getTexture();
		tex.bind();
		label_bake_shader.setUniform1i("usetex", 1);
		label_bake.render();
		label.buffer.unbind();

		resetViewPort();
	}*/

	const Font* Text::s_font;

	void Text::setFont(const Font& font) {
		s_font = &font;
	}

	const Font* Text::getFont() {
		return s_font;
	}

	std::ostream& operator<<(std::ostream& os, const glm::vec2& vec) {
		os << "[" << vec.x << ',' << vec.y << ']';
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const glm::vec3& vec) {
		os << "[" << vec.x << ',' << vec.y << ',' << vec.z << ']';
		return os;
	}

	float calculate_final_size(const textrenderData& renderData, const glm::vec2& size, const Font* font) {
		float first = 0.0f;
		float last = 0.0f;

		glm::vec2 advance = { 0.0f, 0.0f };
		for (size_t i = 0; i < renderData.size(); i++) {
			unsigned char c = renderData.at(i).first;
			auto glyph = font->getGlyph(c);
			auto sprite = glyph->sprite;
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

	void Text::submit(Renderer& renderer, const std::string& text, const glm::vec2& pos, const glm::vec2& size, alignment align, const glm::vec4& bg_color, const Font* font) {
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
			auto sprite = glyph->sprite;
			if (!glyph) continue;

			renderer.submit(pos + advance + glyph->top_left * size + glm::vec2{ 0.0, line }, glyph->size * size, sprite, pd.second);
			advance += glm::vec2(glyph->advance.x, 0.0f) * size;
		}
	}

}