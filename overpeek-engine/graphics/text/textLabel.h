#pragma once

#include <string>

#include "font.h"
#include "../renderer.h"
#include "../frameBuffer.h"
#include "../../internal_libs.h"



namespace oe {

	class TextLabel {
	private:
		typedef std::pair<char, glm::vec4> pairData;
		typedef std::vector<pairData> textDrawData;
	public:
		static textDrawData textToDrawData(const std::string& text);

	private:
		const Font* m_font;
		FrameBuffer* m_framebuffer;

		std::string m_text;
		
	public:
		TextLabel(const Font& font);

		void submit(Renderer& renderer, std::string text, glm::vec2 pos, glm::vec2 size);
		// Generate framebuffer and draw text to it
		//void generate(const std::string& text);

		// Return usable oe::Texture object
		inline const Texture* getTexture() const { return m_framebuffer->getTexture(); }

		inline std::string getText() const { return m_text; }

	};

}