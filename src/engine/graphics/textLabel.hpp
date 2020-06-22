#pragma once

#include <string>

#include "font.hpp"
#include "engine/graphics/interface/renderer.hpp"
#include "engine/graphics/interface/framebuffer.hpp"
#include "engine/internal_libs.hpp"
#include "engine/assets/default_shader/default_shader.hpp"
#include "engine/interfacegen.hpp"



namespace oe::graphics {

	typedef std::pair<char, glm::vec4> pairData;
	typedef std::vector<pairData> textrenderData;
	static textrenderData textTorenderData(const std::wstring& text);

	class TextLabel {
	private:
		static oe::assets::DefaultShader* fb_shader;
		static oe::assets::DefaultShader* getFBShader();
		static oe::graphics::Renderer* fb_renderer;
		static oe::graphics::Renderer* getFBRenderer();

		Font* m_font;
		FrameBuffer m_framebuffer;
		glm::vec2 m_fb_size;
		Sprite m_sprite;

		std::wstring m_text;
		glm::vec2 m_size;
		bool initial_generated = false;
		
	public:
		TextLabel(Font* font);
		TextLabel();

		// Generate framebuffer and render text to it
		void generate(const std::wstring& text, Window& window, const glm::vec4& color = oe::colors::transparent);
		void regenerate(const std::wstring& text, Window& window, const glm::vec4& color = oe::colors::transparent);

		inline const FrameBuffer* getFB() const { return &m_framebuffer; }
		inline const Sprite* getSprite() const { return &m_sprite; }
		inline glm::vec2 getSize() const { return m_size; }
		inline std::wstring getText() const { return m_text; }

	};

	class Text {
	private:
		static Font* s_font;

	public:
		static glm::vec2 size(const std::wstring& text, const glm::vec2& size, Font* font = nullptr);
		static void setFont(Font* font);
		static Font* getFont();
		static void submit(Renderer* renderer, const std::wstring& text, const glm::vec2& pos, const glm::vec2& size, const glm::vec2& align = alignments::top_left, const glm::vec4& bg_color = glm::vec4(0.0f), Font* font = nullptr);
		static void submit(Renderer* renderer, const std::wstring& text, const glm::vec2& pos, float size, const glm::vec2& align = alignments::top_left, const glm::vec4& bg_color = glm::vec4(0.0f), Font* font = nullptr) { submit(renderer, text, pos, glm::vec2(size, size), align, bg_color, font); }

	};

}
