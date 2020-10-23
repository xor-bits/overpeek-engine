#pragma once

#include <string>
#include <algorithm>
#include <numeric>

#include "engine/interfacegen.hpp"
#include "engine/internal_libs.hpp"
#include "engine/graphics/sprite.hpp"



namespace oe::assets { class DefaultShader; }
namespace oe::graphics { class Font; class Renderer; }

namespace oe::graphics
{
	template<typename char_type>
	struct text_render_input
	{
		std::basic_string<char_type> string;
		std::unordered_map<size_t, glm::vec4> string_color_map;

		using string_view_color = std::pair<std::basic_string_view<char_type>, glm::vec4>;
		using string_color = std::pair<std::basic_string<char_type>, glm::vec4>;
		
		using string_view_color_vec = std::vector<string_view_color>;
		using string_color_vec = std::vector<string_color>;

		text_render_input(const string_view_color_vec& _string_vec)
		{
			string.reserve(std::accumulate(_string_vec.begin(), _string_vec.end(), static_cast<size_t>(0), [](size_t count, const string_view_color& pair){ return count + pair.first.size(); }));
			for (const auto& pair : _string_vec)
			{
				string_color_map.insert({ string.size(), pair.second });
				string.insert(string.size(), pair.first);
			}
		}

		text_render_input(const string_color_vec& _string_vec)
		{
			string.reserve(std::accumulate(_string_vec.begin(), _string_vec.end(), static_cast<size_t>(0), [](size_t count, const string_color& pair){ return count + pair.first.size(); }));
			for (const auto& pair : _string_vec)
			{
				string_color_map.insert({ string.size(), pair.second });
				string.insert(string.size(), pair.first);
			}
		}

		text_render_input(const std::basic_string_view<char_type>& _string, const glm::vec4& _color)
			: string(_string)
			, string_color_map({ { 0, _color } })
		{}

		text_render_input(const std::basic_string<char_type>& _string, const glm::vec4& _color)
			: string(_string)
			, string_color_map({ { 0, _color } })
		{}

		text_render_input(const std::basic_string_view<char_type>& _string)
			: text_render_input(_string, oe::colors::white)
		{}

		text_render_input(const std::basic_string<char_type>& _string)
			: text_render_input(_string, oe::colors::white)
		{}

		text_render_input(const char_type* _string, const glm::vec4& _color)
			: text_render_input(std::basic_string<char_type>(_string), _color)
		{}

		text_render_input(const char_type* _string)
			: text_render_input(_string, oe::colors::white)
		{}

		text_render_input()
			: string()
			, string_color_map({ { 0, oe::colors::white } })
		{}

		bool operator==(const text_render_input& right)
		{
			return string == right.string && string_color_map == right.string_color_map;
		}
	};

	template<typename char_type>
	class BasicText {
	public:
		using string_t = text_render_input<char_type>;

		static glm::vec2 size(Font& font, const string_t& text, const glm::vec2& size);
		static glm::vec2 charpos(Font& font, const string_t& text, size_t first, size_t i, const glm::vec2& pos, const glm::vec2& size, const glm::vec2& align);
		static void submit(Renderer& renderer, Font& font, const string_t& text, const glm::vec2& pos, const glm::vec2& size, const glm::vec2& align = alignments::top_left);
		static void submit(Renderer& renderer, Font& font, const string_t& text, const glm::vec2& pos, float size, const glm::vec2& align = alignments::top_left) { submit(renderer, font, text, pos, glm::vec2(size, size), align); }
	};

	template<typename char_type>
	class BasicTextLabel
	{
		using string_t = text_render_input<char_type>;

		Font& m_font;
		FrameBuffer m_framebuffer;
		glm::vec2 m_fb_size;
		Sprite m_sprite;

		string_t m_text;
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
		void generate(const string_t& text, const Window& window, const glm::vec4& color = oe::colors::transparent);
		void regenerate(const string_t& text, const Window& window, const glm::vec4& color = oe::colors::transparent);

		inline const FrameBuffer* getFB() const { return &m_framebuffer; }
		inline const Sprite& getSprite() const { return m_sprite; }
		inline Font& getFont() const { return m_font; }
		inline glm::vec2 getSize() const { return m_size; }
		inline string_t getText() const { return m_text; }
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