#pragma once

#include <string>

#include "font.h"
#include "engine/graphics/renderer.h"
#include "engine/graphics/framebuffer.h"
#include "engine/internal_libs.h"



namespace oe::graphics {

	typedef std::pair<char, glm::vec4> pairData;
	typedef std::vector<pairData> textrenderData;
	static textrenderData textTorenderData(const std::string& text);

	class TextLabel {
	private:
		const Font* m_font;
		FrameBuffer* m_framebuffer;

		std::string m_text;
		
	public:
		TextLabel(const Font& font);

		void submit(Renderer& renderer, std::string text, glm::vec2 pos, glm::vec2 size);
		// Generate framebuffer and render text to it
		//void generate(const std::string& text);

		// Return usable oe::Texture object
		inline const Texture* getTexture() const { return m_framebuffer->getTexture(); }

		inline std::string getText() const { return m_text; }

	};

	class Text {
	private:
		static const Font* s_font;

	public:
		static float width(const std::string& text, const glm::vec2& size, const Font* font = nullptr);
		static void setFont(const Font& font);
		static const Font* getFont();
		static void submit(Renderer& renderer, const std::string& text, const glm::vec2& pos, const glm::vec2& size, const glm::vec2& align = alignments::top_left, const glm::vec4& bg_color = glm::vec4(0.0f), const Font* font = nullptr);
		static void submit(Renderer& renderer, const std::string& text, const glm::vec2& pos, float size, const glm::vec2& align = alignments::top_left, const glm::vec4& bg_color = glm::vec4(0.0f), const Font* font = nullptr) { submit(renderer, text, pos, glm::vec2(size, size), align, bg_color, font); }

	};

}
