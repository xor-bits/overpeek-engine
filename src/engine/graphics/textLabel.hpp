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

	struct text_render_cache
	{
		struct datapoint
		{
			bool rendered      = false;

			glm::vec2 offset   = { 0.0f, 0.0f };
			glm::vec2 position = { 0.0f, 0.0f };
			glm::vec2 size     = { 1.0f, 1.0f };
			glm::vec4 color    = oe::colors::white;

			const oe::graphics::Sprite* sprite;
			char32_t codepoint = '\0';
		};
		
		// members
		glm::vec2                                  top_left{ 0.0f, 0.0f };
		glm::vec2                                   scaling{ 0.0f, 0.0f };
		glm::vec2                                      size{ 0.0f, 0.0f };
		std::vector<datapoint>                   datapoints{};
		bool                                            sdf{};
		TextOptions                                 options{};

		// methods
		text_render_cache() = default;
		template<typename char_type>
		void create(const oe::utils::color_string<char_type>& text, Font& font, const oe::TextOptions& options = {}, const glm::vec2& origin_pos = {});
		void submit(Renderer& renderer) const;
		glm::vec2 offset_to(size_t index) const;
		glm::vec2 offset_to(decltype(datapoints)::const_iterator datapoint) const;
	};



	class TextLabel
	{
		FrameBuffer m_framebuffer{};
		glm::vec2 m_fb_size{ 0.0f, 0.0f };
		Sprite m_sprite{};

		text_render_cache m_cache{};
		glm::vec2 m_size{ 0.0f, 0.0f };
		
	public:
		TextLabel() = default;

		// Generate framebuffer and render text to it
		bool generate(const text_render_cache& cache);
		void regenerate(const text_render_cache& cache);

		// getters
		[[nodiscard]] constexpr inline const FrameBuffer& framebuffer() const { return m_framebuffer; }
		[[nodiscard]] constexpr inline const Sprite& sprite() const { return m_sprite; }
		[[nodiscard]] constexpr inline const glm::vec2& size() const { return m_size; }
		[[nodiscard]] constexpr inline float aspect() const { return (m_size.y == 0.0f) ? (0.0f) : (m_size.x / m_size.y); }
	};
}