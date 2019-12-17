#include "textLabel.h"

#include <sstream>
#include <iostream>
#include <ostream>



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

	textrenderData textTorenderData(const std::string& text) {
		textrenderData renderData = textrenderData();

		bool nextIsColor = false;
		glm::vec4 curColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		std::stringstream ss;
		for (int i = 0; i < text.size(); i++) {
			char c = text[i];

			// "blablabla~#00ff00~"
			// check if there's upcoming colorcode
			if (c == '#') {
				// chech if there are enough characters remaining
				if (i + 6 < text.size()) {
					std::stringstream hex_str;
					hex_str << std::hex << text.substr(i + 1, 6);
					unsigned long hex = std::stol(hex_str.str(), nullptr, 16);

					glm::vec3 rgb = hexToRGB(hex);
					curColor = glm::vec4(rgb / 255.0f, 1.0f); // alpha 1.0

					i += 6;
					continue;
				}
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
		Text::submit(renderer, text, pos, size, oe::alignment::top_left, glm::vec4(1.0f), m_font);
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

	void Text::submit(Renderer& renderer, const std::string& text, const glm::vec2& pos, const glm::vec2& size, oe::alignment align, const glm::vec4& bg_color, const Font* font) {
		if (!font) font = s_font;
		if (!font) { spdlog::error("No font!"); return; }

		textrenderData renderData = textTorenderData(text);
		float size_x_accumulator = 0.0f;
		float offset = 0.0f;
		float line = font->bb_max_height * size.y;

		// get width
		for (pairData& pd : renderData) {
			unsigned char i = pd.first;

			// i == 32 // white space
			if (i == 9) // horizontal tab
			{
				float char_offset = offset / size.x;
				float next_min_tab = ceil(char_offset / 4.0f + 0.001f) * 4.0f;

				offset = next_min_tab * size.x;
				continue;
			}
			if (!font->m_glyphs[i]) continue; // no such character

			glm::vec2 local_offset = font->m_glyphs[i]->top_left * size;
			local_offset.x += offset;
			local_offset.y += line;
			offset += font->m_glyphs[i]->advance.x * size.x;

			size_x_accumulator = local_offset.x + font->m_glyphs[i]->size.x * size.x;
		}

		glm::vec2 local_size = glm::vec2(size_x_accumulator, size.y);
		glm::vec2 align_offset = -oe::alignmentOffset(local_size, align);

		// Text itself
		offset = 0.0f;
		for (pairData& pd : renderData) {
			unsigned char i = pd.first;

			// i == 32 // white space
			if (i == 9) // horizontal tab
			{
				float char_offset = offset / size.x;
				float next_min_tab = ceil(char_offset / 4.0f + 0.001f) * 4.0f;

				offset = next_min_tab * size.x;
				continue;
			}
			if (!font->m_glyphs[i]) continue; // no such character

			glm::vec2 local_offset = font->m_glyphs[i]->top_left * size;
			local_offset.x += offset;
			local_offset.y += line;
			float advance = font->m_glyphs[i]->advance.x * size.x;
			renderer.submit(pos + local_offset + align_offset, size, pd.first, pd.second);

			offset += advance;
		}
		
		// Text background
		if (bg_color.w > 0.0f) {
			renderer.submit(pos + align_offset, local_size, 1, bg_color);
			//												 font texture at index 1 is just white texture
		}
	}

}