#pragma once

#include <string>
#include <algorithm>
#include <numeric>

#include "engine/interfacegen.hpp"
#include "engine/internal_libs.hpp"
#include "engine/graphics/sprite.hpp"
#include "engine/utility/color_string.hpp"



namespace oe::asset { class DefaultShader; }
namespace oe::graphics { class Font; class Renderer; }

namespace oe::graphics
{
	struct text_render_cache_datapoint
	{
		bool rendered      = false;

		glm::vec2 position = { 0.0f, 0.0f };
		glm::vec2 size     = { 1.0f, 1.0f };
		glm::vec4 color    = oe::colors::white;

		const oe::graphics::Sprite* sprite;
		char32_t codepoint = '\0';
	};
	struct text_render_cache
	{
		std::vector<text_render_cache_datapoint> datapoints{};
		glm::vec2 top_left = { 0.0f, 0.0f };
		glm::vec2 size = { 0.0f, 0.0f };
	};



	template<typename char_type>
	class BasicText {
	public:
		using string_t = oe::utils::color_string<char_type>;
		
		static void create_text_render_cache(
			      text_render_cache&     cache,
			const string_t&              text,
			      Font&                  font,
			const glm::vec2&             origin_pos      = { 0.0f, 0.0f },
			const glm::vec2&             size            = { 64.0f, 64.0f }, // more like scaling per axis
			const glm::vec2&             align_to_origin = oe::alignments::top_left);

		static void submit(const text_render_cache& cache, Renderer& renderer);
		static glm::vec2 offset_to_char(const text_render_cache& cache, size_t index);
	};

	template<typename char_type>
	class BasicTextLabel
	{
		using text_t = BasicText<char_type>;
		using string_t = oe::utils::color_string<char_type>;

		Font& m_font;
		FrameBuffer m_framebuffer{};
		glm::vec2 m_fb_size;
		Sprite m_sprite;

		string_t m_text;
		glm::vec2 m_size;
		const size_t m_resolution;
		
	public:
		BasicTextLabel(Font& font, const size_t resolution = 64)
			: m_font(font)
			, m_fb_size(0.0f, 0.0f)
			, m_text()
			, m_size(0.0f, 0.0f)
			, m_resolution(resolution)
		{}

		// Generate framebuffer and render text to it
		void generate(const string_t& text, const Window& window, const oe::color& bg_color = oe::colors::transparent, float width = 0.25f, float outline_width = 0.15f, const oe::color& outline_c = oe::colors::black);
		void regenerate(const string_t& text, const Window& window, const oe::color& bg_color = oe::colors::transparent, float width = 0.25f, float outline_width = 0.15f, const oe::color& outline_c = oe::colors::black);

		inline const FrameBuffer* getFB() const { return &m_framebuffer; }
		inline const Sprite& getSprite() const { return m_sprite; }
		inline Font& getFont() const { return m_font; }
		inline glm::vec2 getSize() const { return m_size; }
		inline float getAspect() const { return m_size.y == 0.0f ? 0.0f : m_size.x / m_size.y; }
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