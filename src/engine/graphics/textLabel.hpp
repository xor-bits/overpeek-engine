#pragma once

#include <string>

#include "engine/interfacegen.hpp"
#include "engine/internal_libs.hpp"
#include "engine/graphics/sprite.hpp"



namespace oe::assets { class DefaultShader; }
namespace oe::graphics { class Font; class Renderer; }

namespace oe::graphics
{
	template<typename char_type>
	class BasicText {
	public:
		typedef std::basic_string<char_type> string_type;

		static glm::vec2 size(Font& font, const string_type& text, const glm::vec2& size);
		static void submit(Renderer& renderer, Font& font, const string_type& text, const glm::vec2& pos, const glm::vec2& size, const glm::vec2& align = alignments::top_left, const glm::vec4& bg_color = glm::vec4(0.0f));
		static void submit(Renderer& renderer, Font& font, const string_type& text, const glm::vec2& pos, float size, const glm::vec2& align = alignments::top_left, const glm::vec4& bg_color = glm::vec4(0.0f)) { submit(renderer, font, text, pos, glm::vec2(size, size), align, bg_color); }

	};

	template<typename char_type>
	class BasicTextLabel
	{
		typedef std::basic_string<char_type> string_type;

		Font& m_font;
		FrameBuffer m_framebuffer;
		glm::vec2 m_fb_size;
		Sprite m_sprite;

		string_type m_text;
		glm::vec2 m_size;
		bool initial_generated = false;
		
	public:
		BasicTextLabel(Font& font)
			: m_font(font)
			, m_fb_size(0.0f, 0.0f)
			, m_text()
			, m_size(0.0f, 0.0f)
		{}

		// Generate framebuffer and render text to it
		void generate(const string_type& text, const Window& window, const glm::vec4& color = oe::colors::transparent);
		void regenerate(const string_type& text, const Window& window, const glm::vec4& color = oe::colors::transparent);

		inline const FrameBuffer* getFB() const { return &m_framebuffer; }
		inline const Sprite& getSprite() const { return m_sprite; }
		inline glm::vec2 getSize() const { return m_size; }
		inline string_type getText() const { return m_text; }
	};

	using TextLabel = BasicTextLabel<char>;
	using wTextLabel = BasicTextLabel<wchar_t>;
	using u16TextLabel = BasicTextLabel<char16_t>;
	using u32TextLabel = BasicTextLabel<char32_t>;

	using Text = BasicText<char>;
	using wText = BasicText<wchar_t>;
	using u16Text = BasicText<char16_t>;
	using u32Text = BasicText<char32_t>;
}