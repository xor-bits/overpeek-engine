#include "textLabel.h"

#include <sstream>
#include <iostream>



namespace oe {

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

	TextLabel::textDrawData TextLabel::textToDrawData(const std::string& text) {
		textDrawData drawData = textDrawData();

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
			
			drawData.push_back(std::pair(c, curColor));
		}

		return drawData;
	}

	TextLabel::TextLabel(const Font& font) {
		m_font = &font;
		m_framebuffer = nullptr;
	}

	void TextLabel::submit(Renderer& renderer, std::string text, glm::vec2 pos, glm::vec2 size) {
		textDrawData drawData = textToDrawData(text);
		float offset = 0.0f;
		for (pairData& pd : drawData) {
			unsigned char i = pd.first;

			//if (i == 32) offset += 0.5f * size.x; // white space
			if (!m_font->m_glyphs[i]) continue; // no such character

			glm::vec2 local_offset = m_font->m_glyphs[i]->top_left * size;
			local_offset.x -= offset;
			float advance = m_font->m_glyphs[i]->advance.x * size.x;
			renderer.submit(pos - local_offset, size, pd.first, pd.second, oe::topLeft, 0.0f);

			offset += advance;
		}
	}

	/*void TextLabel::generate(const std::string& text) {
		m_text = text;
		textDrawData drawData = textToDrawData(text);

		//Clear framebuffer (background)
		//label.buffer.clear(1.0f, 0.0f, 0.5f, 1.0f); //pink
		label.buffer.clear(0.0f, 0.0f, 0.0f, 0.0f); //transparent

		//Draw submits
		float xDelta = 0.0f;
		label_bake.clear();
		label_bake.begin();
		for (TextDrawData.CharColorPair cp : drawData.drawData) {
			Glyph glyph = glyphs.getGlyphData(cp.character);
			//float yDelta = glyph.y;
			label_bake.submit(new Quad(new vec2(xDelta, glyphs.getHeight()), new vec2(glyphs.getWidth(), -glyphs.getHeight()), glyph.textureId, cp.color, Alignment.TOP_LEFT));
			xDelta += glyph.advance;
		}
		label_bake.end();

		//Drawing
		label_bake_shader.bind();
		Texture tex = glyphs.getTexture();
		tex.bind();
		label_bake_shader.setUniform1i("usetex", 1);
		label_bake.draw();
		label.buffer.unbind();

		resetViewPort();
	}*/

}