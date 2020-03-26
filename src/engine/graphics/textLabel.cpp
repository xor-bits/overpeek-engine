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

	TextLabel::TextLabel(const Font& font) {
		m_font = &font;
		m_framebuffer = nullptr;
	}

	void TextLabel::submit(Renderer& renderer, std::string text, glm::vec2 pos, glm::vec2 size) {
		Text::submit(renderer, text, pos, size, oe::alignments::top_left, glm::vec4(1.0f), m_font);
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

	glm::vec2 calculate_final_size(const textrenderData& renderData, const glm::vec2& size, const Font* font) {
		glm::vec2 top_left_corner = { 0.0f, 0.0f };
		glm::vec2 bottom_right_corner = { 0.0f, 0.0f };

		glm::vec2 advance = { 0.0f, 0.0f };
		for (size_t i = 0; i < renderData.size(); i++) {
			unsigned char c = renderData.at(i).first;
			auto glyph = font->getGlyph(c);
			if (!glyph) continue;
			auto sprite = glyph->sprite;

			if (c == '\n') {
				advance.x = 0.0f;
				advance.y += size.y;
			}
			else {
				top_left_corner.x = std::min(top_left_corner.x, advance.x);
				top_left_corner.y = std::min(top_left_corner.y, advance.y + size.y);

				bottom_right_corner.x = std::max(bottom_right_corner.x, advance.x + glyph->aspect * size.x);
				bottom_right_corner.y = std::max(bottom_right_corner.y, advance.y);

				advance.x += glyph->advance * size.x;
			}
		}

		return glm::abs(top_left_corner - bottom_right_corner);
	}

	glm::vec2 Text::size(const std::string& text, const glm::vec2& size, const Font* font) {
		if (!font) font = s_font;
		if (!font) oe_error_terminate("No font!");

		textrenderData renderData = textTorenderData(text);
		return calculate_final_size(renderData, size, font);
	}

	void Text::submit(Renderer& renderer, const std::string& text, const glm::vec2& pos, const glm::vec2& size, const glm::vec2& align, const glm::vec4& bg_color, const Font* font) {
		if (!font) font = s_font;
		if (!font) oe_error_terminate("No font!");

		textrenderData renderData = textTorenderData(text);
		glm::vec2 final_size = calculate_final_size(renderData, size, font);
		const glm::vec2 advance_initial = alignmentOffset(-final_size, align);
		glm::vec2 advance = advance_initial;

		if (bg_color.a > 0.0f) {
			renderer.submit(advance_initial, final_size, font->getGlyph(0)->sprite, bg_color);
		}

		for (pairData& pd : renderData) {
			unsigned char i = pd.first;
			auto glyph = font->getGlyph(i);
			if (!glyph) continue;
			auto sprite = glyph->sprite;

			if (i == '\n') {
				advance.x = advance_initial.x;
				advance.y += size.y;
			}
			else {
				renderer.submit(pos + advance, { size.x * glyph->aspect, size.y }, sprite, pd.second, oe::alignments::bottom_left);
				advance.x += glyph->advance * size.x;
			}
		}
	}

}