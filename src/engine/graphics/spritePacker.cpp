#include "spritePacker.hpp"
#include "engine/engine.hpp"
#include "engine/graphics/interface/window.hpp"
#include "sprite.hpp"



// ignore external warnings
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpedantic"
#elif __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

#include <finders_interface.h>

// ignore external warnings
#ifdef __clang__
#pragma clang diagnostic pop
#elif __GNUC__
#pragma GCC diagnostic pop
#endif



namespace oe::graphics {

	const static bool allow_flip = false;
	const rectpack2D::flipping_option runtime_flipping_mode = rectpack2D::flipping_option::DISABLED;
	
	using spaces_type = rectpack2D::empty_spaces<allow_flip, rectpack2D::default_empty_spaces>;
	using rect_type = rectpack2D::output_rect_t<spaces_type>;

	struct __usr_data
	{
		std::vector<rect_type> m_rectangles;
		std::vector<oe::utils::image_data> m_images;
	};



	SpritePack::SpritePack(int border)
		: m_usr_data(nullptr)
		, m_border(border)
		, m_constructed(false)
	{
		m_usr_data = new __usr_data{};
		clear();
	}

	SpritePack::~SpritePack()
	{
		m_usr_data->m_images.clear();
		m_usr_data->m_rectangles.clear();
		delete m_usr_data;
		m_sprites.clear();
	}

	const Sprite* SpritePack::create(oe::utils::image_data&& sprite_texture)
	{
		m_usr_data->m_rectangles.push_back(rectpack2D::rect_xywh(0, 0, sprite_texture.width + m_border, sprite_texture.height + m_border));
		m_sprites.push_back(std::make_unique<Sprite>());

		m_usr_data->m_images.emplace_back(std::move(sprite_texture));

		return m_sprites.back().get();
	}

	const Sprite* SpritePack::create(const oe::utils::image_data& sprite_texture)
	{
		return create(oe::utils::image_data(sprite_texture));
	}

	const Sprite* SpritePack::create(const oe::utils::FileIO& sprite_texture)
	{
		return create(oe::utils::image_data(sprite_texture));
	}

	void SpritePack::clear()
	{
		m_usr_data->m_images.clear();
		m_usr_data->m_rectangles.clear();
		m_sprites.clear();

		// load empty sprite
		uint8_t data[1] = { std::numeric_limits<uint8_t>::max() }; // one white pixel
		create(oe::utils::image_data(data, oe::formats::mono, 1, 1));
	}

	constexpr inline size_t coordsToIndex(size_t x, size_t y, size_t c, size_t width, size_t channels) noexcept
	{
		return channels * (x + y * width) + c;
	}

	void SpritePack::constructRepeat(const oe::TextureInfo& texture_settings)
	{
		// pack sprites
		const auto max_side = 10000;
		const auto discard_step = 1;

		auto report_successful = [](rect_type&) { return rectpack2D::callback_result::CONTINUE_PACKING; };
		auto report_unsuccessful = [](rect_type&) { return rectpack2D::callback_result::ABORT_PACKING; };

		const auto result_size = rectpack2D::find_best_packing<spaces_type>(
			m_usr_data->m_rectangles,
			rectpack2D::make_finder_input(
				max_side,
				discard_step,
				report_successful,
				report_unsuccessful,
				runtime_flipping_mode
			)
		);

		// create texture and add sprites
		const size_t pack_width = static_cast<size_t>(std::abs(result_size.w));
		const size_t pack_height = static_cast<size_t>(std::abs(result_size.h));
		unsigned char* data = new unsigned char[pack_width * pack_height * 4]();
		for (size_t i = 0; i < m_usr_data->m_rectangles.size(); i++) {
			const auto& rectangle = m_usr_data->m_rectangles.at(i);
			const auto& image = m_usr_data->m_images.at(i);
			const size_t rect_x = static_cast<size_t>(std::abs(rectangle.x));
			const size_t rect_y = static_cast<size_t>(std::abs(rectangle.y));
			const size_t rect_w = static_cast<size_t>(std::abs(image.width));
			const size_t rect_h = static_cast<size_t>(std::abs(image.height));
			auto& sprite = m_sprites.at(i);
			
			
			sprite->size = glm::vec2(rect_w / (float)pack_width, rect_h / (float)pack_height);
			sprite->position = glm::vec2(rect_x / (float)pack_width, rect_y / (float)pack_height);
			sprite->opacity = false;

			// conversions
			switch (image.format)
			{
			case oe::formats::mono:
				// mono format to rgba
				for (size_t y = 0; y < rect_h; y++) {
					for (size_t x = 0; x < rect_w; x++) {
						data[coordsToIndex(rect_x + x, rect_y + y, 0, pack_width, 4)] = image.data[coordsToIndex(x, y, 0, rect_w, 1)];
						data[coordsToIndex(rect_x + x, rect_y + y, 1, pack_width, 4)] = image.data[coordsToIndex(x, y, 0, rect_w, 1)];
						data[coordsToIndex(rect_x + x, rect_y + y, 2, pack_width, 4)] = image.data[coordsToIndex(x, y, 0, rect_w, 1)];
						data[coordsToIndex(rect_x + x, rect_y + y, 3, pack_width, 4)] = 255;
					}
				}
				break;
			case oe::formats::rgb:
				// rgb format to rgba
				for (size_t y = 0; y < rect_h; y++) {
					for (size_t x = 0; x < rect_w; x++) {
						data[coordsToIndex(rect_x + x, rect_y + y, 0, pack_width, 4)] = image.data[coordsToIndex(x, y, 0, rect_w, 3)];
						data[coordsToIndex(rect_x + x, rect_y + y, 1, pack_width, 4)] = image.data[coordsToIndex(x, y, 1, rect_w, 3)];
						data[coordsToIndex(rect_x + x, rect_y + y, 2, pack_width, 4)] = image.data[coordsToIndex(x, y, 2, rect_w, 3)];
						data[coordsToIndex(rect_x + x, rect_y + y, 3, pack_width, 4)] = 255;
					}
				}
				break;
			case oe::formats::rgba:
				// no conversion
				for (size_t y = 0; y < rect_h; y++) {
					for (size_t x = 0; x < rect_w; x++) {
						data[coordsToIndex(rect_x + x, rect_y + y, 0, pack_width, 4)] = image.data[coordsToIndex(x, y, 0, rect_w, 4)];
						data[coordsToIndex(rect_x + x, rect_y + y, 1, pack_width, 4)] = image.data[coordsToIndex(x, y, 1, rect_w, 4)];
						data[coordsToIndex(rect_x + x, rect_y + y, 2, pack_width, 4)] = image.data[coordsToIndex(x, y, 2, rect_w, 4)];
						data[coordsToIndex(rect_x + x, rect_y + y, 3, pack_width, 4)] = image.data[coordsToIndex(x, y, 3, rect_w, 4)];
						
						// automatic translucency detector
						sprite->opacity = 
							sprite->opacity || 
							data[coordsToIndex(rect_x, rect_y + y, 3, pack_width, 4)] != 255;
					}
				}
				break;
			case oe::formats::none:
				break;
			}
		}

		// the main texture
		oe::TextureInfo texture_info = texture_settings;
		texture_info.data = data;
		texture_info.size_offset = { { pack_width, 0 }, { pack_height, 0} };
		m_texture = Texture(texture_info);

		// sprite owners
		for (auto& sprite : m_sprites)
			sprite->m_owner = m_texture;
		
		// complete
		m_constructed = true;
		delete[] data;
	}

	void SpritePack::construct(const oe::TextureInfo& texture_settings)
	{
		constructRepeat(texture_settings);

		m_usr_data->m_images.clear();
		m_usr_data->m_rectangles.clear();
	}

	void SpritePack::bind()
	{
		if (!m_constructed) { spdlog::warn("SpritePack was not constructed"); constructRepeat(); }
		m_texture->bind();
	}

}
