#pragma once

#include <string>

#include "engine/interfacegen.hpp"
#include "engine/internal_libs.hpp"
#include "engine/graphics/sprite.hpp"



namespace oe::assets { class DefaultShader; }
namespace oe::graphics { class Font; }

namespace oe::graphics {

	class TextLabelRenderer
	{
	private:
		static TextLabelRenderer* singleton;
		TextLabelRenderer(const TextLabelRenderer& copy) = delete;
		TextLabelRenderer();

	public:
		static TextLabelRenderer& getSingleton() { if(!singleton) singleton = new TextLabelRenderer(); return *singleton; }

		oe::assets::DefaultShader* fb_shader;
		oe::graphics::Renderer* fb_renderer;
	};

	template<typename char_type>
	class BasicTextLabel
	{
		typedef std::basic_string<char_type> string_type;

		Font* m_font;
		FrameBuffer m_framebuffer;
		glm::vec2 m_fb_size;
		Sprite m_sprite;

		string_type m_text;
		glm::vec2 m_size;
		bool initial_generated = false;
		
	public:
		BasicTextLabel(Font* font)
			: m_text(str_array_cast<char_type>("").c_str())
			, m_font(font)
			, m_fb_size(0.0f, 0.0f)
			, m_size(0.0f, 0.0f)
		{}

		BasicTextLabel()
			: BasicTextLabel(Text::getFont())
		{}

		// Generate framebuffer and render text to it
		void generate(const string_type& text, Window& window, const glm::vec4& color = oe::colors::transparent);
		void regenerate(const string_type& text, Window& window, const glm::vec4& color = oe::colors::transparent);

		inline const FrameBuffer* getFB() const { return &m_framebuffer; }
		inline const Sprite* getSprite() const { return &m_sprite; }
		inline glm::vec2 getSize() const { return m_size; }
		inline string_type getText() const { return m_text; }
	};

	using TextLabel = BasicTextLabel<char>;
	using wTextLabel = BasicTextLabel<wchar_t>;
	using u16TextLabel = BasicTextLabel<char16_t>;
	using u32TextLabel = BasicTextLabel<char32_t>;

	
	template<typename char_type>
	class BasicText {
	private:
		static Font* s_font;

	public:
		typedef std::basic_string<char_type> string_type;

		static glm::vec2 size(const string_type& text, const glm::vec2& size, Font* font = nullptr);
		static void setFont(Font* font);
		static Font* getFont();
		static void submit(Renderer* renderer, const string_type& text, const glm::vec2& pos, const glm::vec2& size, const glm::vec2& align = alignments::top_left, const glm::vec4& bg_color = glm::vec4(0.0f), Font* font = nullptr);
		static void submit(Renderer* renderer, const string_type& text, const glm::vec2& pos, float size, const glm::vec2& align = alignments::top_left, const glm::vec4& bg_color = glm::vec4(0.0f), Font* font = nullptr) { submit(renderer, text, pos, glm::vec2(size, size), align, bg_color, font); }

	};

	using Text = BasicText<char>;
	using wText = BasicText<wchar_t>;
	using u16Text = BasicText<char16_t>;
	using u32Text = BasicText<char32_t>;

}
