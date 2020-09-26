#include "spritePacker.hpp"
#include "engine/engine.hpp"
#include "engine/graphics/interface/window.hpp"

#include <finders_interface.h>



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
	{
		oe_debug_call("spritepack");

		m_constructed = false;
		m_usr_data = new __usr_data();

		m_usr_data->m_rectangles = std::vector<rect_type>();
		m_usr_data->m_images = std::vector<oe::utils::image_data>(); 
		m_border = border;
		
		clear();
	}

	SpritePack::~SpritePack()
	{
		clear();
	}

	const Sprite* SpritePack::create(const oe::utils::image_data& sprite_texture)
	{
		auto sprite = new Sprite();
		
		m_usr_data->m_rectangles.push_back(rectpack2D::rect_xywh(0, 0, sprite_texture.width + m_border, sprite_texture.height + m_border));
		m_usr_data->m_images.push_back(sprite_texture);
		m_sprites.push_back(sprite);

		return sprite;
	}

	const Sprite* SpritePack::create(fs::path sprite_texture)
	{
		return create(oe::utils::image_data(sprite_texture));
	}

	void SpritePack::clear()
	{
		for(auto iter : m_sprites) delete iter;
		m_usr_data->m_images.clear();
		m_usr_data->m_rectangles.clear();
		m_sprites.clear();

		// load empty sprite
		size_t img_size = 4ul * (size_t)1 * (size_t)1;
		unsigned char* data = new unsigned char[img_size];
		std::memset(data, (unsigned char)255, img_size);
		oe::utils::image_data empty_img(data, oe::formats::rgba, 1, 1);
		create(empty_img);
		delete[] data;
	}

	size_t coordsToIndex(size_t x, size_t y, size_t c, size_t width, size_t channels)
	{
		return channels * (x + y * width) + c;
	}

	void SpritePack::constructRepeat()
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
		size_t pack_width = result_size.w;
		size_t pack_height = result_size.h;
		unsigned char* data = new unsigned char[pack_width * pack_height * 4]();
		for (size_t i = 0; i < m_usr_data->m_rectangles.size(); i++) {
			auto& rectangle = m_usr_data->m_rectangles.at(i);
			auto& image = m_usr_data->m_images.at(i);
			auto& sprite = m_sprites.at(i);
			
			sprite->size = glm::vec2(image.width / (float)pack_width, image.height / (float)pack_height);
			sprite->position = glm::vec2(rectangle.x / (float)pack_width, rectangle.y / (float)pack_height);
			sprite->opacity = false;

			// conversions
			switch (image.format)
			{
			case oe::formats::mono:
				// mono format to rgba
				for (size_t y = 0; y < image.height; y++) {
					for (size_t x = 0; x < image.width; x++) {
						data[coordsToIndex(rectangle.x + x, rectangle.y + y, 0, pack_width, 4)] = image.data[coordsToIndex(x, y, 0, image.width, 1)];
						data[coordsToIndex(rectangle.x + x, rectangle.y + y, 1, pack_width, 4)] = image.data[coordsToIndex(x, y, 0, image.width, 1)];
						data[coordsToIndex(rectangle.x + x, rectangle.y + y, 2, pack_width, 4)] = image.data[coordsToIndex(x, y, 0, image.width, 1)];
						data[coordsToIndex(rectangle.x + x, rectangle.y + y, 3, pack_width, 4)] = 255;
					}
				}
				break;
			case oe::formats::rgb:
				// rgb format to rgba
				for (size_t y = 0; y < image.height; y++) {
					for (size_t x = 0; x < image.width; x++) {
						data[coordsToIndex(rectangle.x + x, rectangle.y + y, 0, pack_width, 4)] = image.data[coordsToIndex(x, y, 0, image.width, 3)];
						data[coordsToIndex(rectangle.x + x, rectangle.y + y, 1, pack_width, 4)] = image.data[coordsToIndex(x, y, 1, image.width, 3)];
						data[coordsToIndex(rectangle.x + x, rectangle.y + y, 2, pack_width, 4)] = image.data[coordsToIndex(x, y, 2, image.width, 3)];
						data[coordsToIndex(rectangle.x + x, rectangle.y + y, 3, pack_width, 4)] = 255;
					}
				}
				break;
			case oe::formats::rgba:
				// no conversion
				for (size_t y = 0; y < image.height; y++) {
					for (size_t x = 0; x < image.width; x++) {
						data[coordsToIndex(rectangle.x + x, rectangle.y + y, 0, pack_width, 4)] = image.data[coordsToIndex(x, y, 0, image.width, 4)];
						data[coordsToIndex(rectangle.x + x, rectangle.y + y, 1, pack_width, 4)] = image.data[coordsToIndex(x, y, 1, image.width, 4)];
						data[coordsToIndex(rectangle.x + x, rectangle.y + y, 2, pack_width, 4)] = image.data[coordsToIndex(x, y, 2, image.width, 4)];
						data[coordsToIndex(rectangle.x + x, rectangle.y + y, 3, pack_width, 4)] = image.data[coordsToIndex(x, y, 3, image.width, 4)];
						sprite->opacity = 
							sprite->opacity || 
							data[coordsToIndex(rectangle.x + x, rectangle.y + y, 3, pack_width, 4)] != 255;
					}
				}
				break;
			}
		}


		oe::TextureInfo texture_info = {};
		texture_info.generate_mipmaps = true;
		texture_info.data = data;
		texture_info.size_offset = { { pack_width, 0 }, { pack_height, 0} };

		m_texture = Texture(texture_info);
		for (auto sprite : m_sprites)
		{
			sprite->m_owner = m_texture;
		}
		m_constructed = true;
		
		delete[] data;
	}

	void SpritePack::construct()
	{
		constructRepeat();

		m_usr_data->m_images.clear();
	}

	void SpritePack::bind()
	{
		if (!m_constructed) { spdlog::warn("SpritePack was not constructed"); constructRepeat(); }
		m_texture->bind();
	}

}