#include "textLabel.hpp"

#include <sstream>
#include <iostream>
#include <ostream>

#include "font.hpp"
#include "engine/engine.hpp"
#include "engine/graphics/interface/window.hpp"
#include "engine/graphics/interface/framebuffer.hpp"
#include "engine/graphics/renderer.hpp"
#include "engine/asset/font_shader/font_shader.hpp"



namespace oe::graphics {
	
	class TextLabelRenderer
	{
	private:
		static TextLabelRenderer* singleton;
		TextLabelRenderer(const TextLabelRenderer& copy) = delete;
		TextLabelRenderer()
			: fb_shader()
			, fb_renderer(2048)
		{}

	public:
		static TextLabelRenderer& getSingleton() { if(!singleton) singleton = new TextLabelRenderer(); return *singleton; }

		oe::asset::FontShader fb_shader;
		oe::graphics::Renderer fb_renderer;
	};
	TextLabelRenderer* TextLabelRenderer::singleton = nullptr;



	bool TextLabel::generate(const text_render_cache& cache)
	{
		/* const bool generated = m_text == text && m_framebuffer;
		if(generated) // why render the same text again?
			return false; */

		regenerate(cache);
		return true;
	}
	
	void TextLabel::regenerate(const text_render_cache& cache)
	{
		m_cache = cache;
		m_size = m_cache.size;

		const glm::vec2 res_scaling = 1.0f / m_cache.scaling * static_cast<float>(m_cache.options.resolution);
		const auto old_fb_state = IFrameBuffer::save_state();
		if (!m_framebuffer || m_fb_size.x < m_size.x * res_scaling.x || m_fb_size.y < m_size.y * res_scaling.y)
		{
			// create new framebuffer
			oe::FrameBufferInfo fb_info = {};
			fb_info.size = static_cast<glm::ivec2>(m_size * res_scaling) * 2; // double the required size, to make room for future reuse
			m_framebuffer = FrameBuffer(fb_info);
			
			m_fb_size = fb_info.size;
		}

		// bind fb
		m_framebuffer->bind();
		m_framebuffer->clear(m_cache.options.background_color);
		const glm::mat4 pr_matrix = glm::ortho(0.0f, m_fb_size.x, m_fb_size.y, 0.0f);

		// submit to the renderer
		const glm::vec2 align_move = oe::alignmentOffset(m_cache.size, m_cache.options.align) / res_scaling;
		auto& tbr = TextLabelRenderer::getSingleton();
		tbr.fb_renderer.clear();
		m_cache.submit(tbr.fb_renderer, false, align_move, res_scaling);
		

		// render to the framebuffer
		tbr.fb_shader.setProjectionMatrix(pr_matrix);
		tbr.fb_shader.setSDF(m_cache.sdf);
		tbr.fb_shader.setWidth(m_cache.options.weight);
		tbr.fb_shader.setEdge(m_cache.options.anti_alias);
		tbr.fb_shader.setOutlineWidth(m_cache.options.outline_weight);
		tbr.fb_shader.setOutlineEdge(m_cache.options.anti_alias);
		tbr.fb_shader.setOutlineColor(m_cache.options.outline_color);
		tbr.fb_renderer.render();

		// bind back the old framebuffer
		IFrameBuffer::load_state(old_fb_state);

		// generate the sprite for user
		const glm::vec2 size_ratio = m_size * res_scaling / m_fb_size;
		/* m_size *= (m_font.isSDF() ? 0.25f : 1.0f); */

		m_sprite.m_owner = m_framebuffer->getTexture();
		m_sprite.position = { 0.0f, 1.0f };
		m_sprite.size = { size_ratio.x, -size_ratio.y };
		m_sprite.opacity = true;
	}
	


	template<typename char_type>
	void text_render_cache::create(const oe::utils::color_string<char_type>& text, Font& font, const oe::TextOptions& _options, const glm::vec2& origin_pos)
	{
		options = _options;
		sdf = font.isSDF();

		// at the very least the size of the text + 1 (for the null terminator)
		// \0, \n, etc... will be removed
		const size_t cache_size = std::accumulate(text.vec.cbegin(), text.vec.cend(), static_cast<size_t>(0), [](size_t since, const oe::utils::color_string_part<char_type>& part){ return since + std::get<0>(part).size(); });
		datapoints.clear();
		datapoints.reserve(cache_size + 1);

		glm::vec2 advance{ 0.0f, 0.0f };
		char32_t codepoint{ U'\0' };
		char32_t codepoint_next{ U'\0' };

		// const glm::vec2 advance_padding_vec = { advance_padding, 0.0f };

		for (size_t j = 0; j < text.vec.size(); j++)
		{
			const auto& string_color_part = text.vec.at(j);
			const auto& string_part = std::get<0>(string_color_part);
			const auto& color_part = std::get<1>(string_color_part);

			for (size_t i = 0; i < string_part.size() + 1; i++)
			{

				// get the codepoint from the input text at i
				codepoint = codepoint_next;
				// and the next codepoint or \0
				codepoint_next = U'\0';
				if(i != string_part.size())
					codepoint_next = static_cast<char32_t>(string_part.at(i));
					

				// skip the first
				if(i == 0)
					continue;
				
				auto& datapoint = datapoints.emplace_back();

				// test if the codepoint will be rendered
				// and advancing the positioning of the following characters
				datapoint.rendered = false;
				datapoint.position = advance;
				datapoint.size = { 0.0f, 0.0f };
				if(codepoint == U'\n') // new line
				{
					advance.x = 0.0f;
					advance.y += options.scale.y;
					continue;
				}
				else if(codepoint == U'\t') // tab
				{
					advance.x = std::ceil(advance.x / options.scale.x) * options.scale.x;
					continue;
				}
				else if(codepoint == U'\0') // null terminator
					continue;

				// get glyph, if possible
				const auto* glyph = font.getGlyph(codepoint);
				// get 0 glyph as the codepoint did not contain any glyphs
				if (!glyph) glyph = font.getGlyph(0);
				// unexpectedly no 0 glyph
				if (!glyph) { spdlog::error("Unexpected NULL glyph at slot 0"); continue; }

				// insert new datapoint
				datapoint.rendered = true;
				datapoint.codepoint = codepoint;
				datapoint.color = color_part;
				datapoint.size = glyph->size * options.scale;
				datapoint.sprite = glyph->sprite;
				datapoint.render_offset = glyph->top_left * options.scale;

				// advancing & kerning & padding
				advance.x += (glyph->advance + font.getKerning(codepoint, codepoint_next) + options.advance_padding) * options.scale.x;
			}
		}

		// NULL terminator
		auto& last_datapoint = datapoints.emplace_back();
		last_datapoint.rendered = false;
		last_datapoint.position = advance;
		last_datapoint.size = { 0.0f, 0.0f };

		// alignment
		// find topleft and bottomright
		glm::vec2 min = last_datapoint.position + last_datapoint.render_offset, max = last_datapoint.position + last_datapoint.render_offset + last_datapoint.size;
		for (const auto& datapoint : datapoints)
		{
			min = glm::min(min, datapoint.position + datapoint.render_offset);
			max = glm::max(max, datapoint.position + datapoint.render_offset + datapoint.size);
		}
		// move all datapoints
		const glm::vec2 total_size = glm::abs(max - min);
		const glm::vec2 align_move = oe::alignmentOffset(total_size, options.align);
		for (auto& datapoint : datapoints)
			datapoint.position -= min + align_move - origin_pos;
		
		// extra cache data
		top_left = origin_pos;
		scaling = options.scale;
		size = total_size;
	}

	glm::vec2 text_render_cache::offset_to(size_t index) const
	{
		if(index >= datapoints.size())
			return { 0.0f, 0.0f };
		return offset_to(datapoints.begin() + index);
	}
	
	glm::vec2 text_render_cache::offset_to(decltype(datapoints)::const_iterator datapoint) const
	{
		return datapoint->position - datapoints.front().position;
	}

	void text_render_cache::submit(Renderer& renderer, bool include_bg, const glm::vec2& offset, const glm::vec2& render_scaling) const
	{
		if (include_bg)
		{
			auto quad = renderer.create();
			quad->setPosition(top_left * render_scaling + offset);
			quad->setZ(-0.1f);
			quad->setSize(size * render_scaling);
			quad->setColor(options.background_color);
			quad->setOpacityMode(true);
			
			renderer.forget(std::move(quad));
		}

		for (const auto& datapoint : datapoints)
		{
			if(!datapoint.rendered)
				continue;

			auto quad = renderer.create();
			quad->setPosition((datapoint.position + datapoint.render_offset) * render_scaling + offset);
			quad->setSize(datapoint.size * render_scaling);
			quad->setSprite(datapoint.sprite);
			quad->setColor(datapoint.color);
			quad->setOpacityMode(true);
			
			renderer.forget(std::move(quad));
		}
	}



	template void text_render_cache::create(const oe::utils::color_string<char>&     text, Font& font, const oe::TextOptions& options, const glm::vec2& origin_pos);
	template void text_render_cache::create(const oe::utils::color_string<wchar_t>&  text, Font& font, const oe::TextOptions& options, const glm::vec2& origin_pos);
	template void text_render_cache::create(const oe::utils::color_string<char16_t>& text, Font& font, const oe::TextOptions& options, const glm::vec2& origin_pos);
	template void text_render_cache::create(const oe::utils::color_string<char32_t>& text, Font& font, const oe::TextOptions& options, const glm::vec2& origin_pos);
}