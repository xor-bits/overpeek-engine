#include "spritePacker.h"
#include "engine/engine.h"

#include <finders_interface.h>



namespace oe::graphics {

	const static bool allow_flip = false;
	const rectpack2D::flipping_option runtime_flipping_mode = rectpack2D::flipping_option::DISABLED;
	
	using spaces_type = rectpack2D::empty_spaces<allow_flip, rectpack2D::default_empty_spaces>;
	using rect_type = rectpack2D::output_rect_t<spaces_type>;

	struct __usr_data {
		std::vector<rect_type> m_rectangles;
		std::vector<oe::utils::image_data> m_images;
	};



	SpritePack::SpritePack(int border) {
		m_usr_data = new __usr_data();
		auto usr_data = static_cast<__usr_data*>(m_usr_data);

		usr_data->m_rectangles = std::vector<rect_type>();
		usr_data->m_images = std::vector<oe::utils::image_data>(); 
		m_border = border;

		// load empty sprite
		size_t img_size = 4ul * (size_t)1 * (size_t)1;
		unsigned char* data = new unsigned char[img_size];
		std::memset(data, (unsigned char)255, img_size);
		oe::utils::image_data img(data, 1, 1, 4);
		addSprite(img);
	}

	SpritePack::~SpritePack() {
		for (size_t i = 0; i < m_sprites.size(); i++) {
			for (size_t j = 0; j < m_sprites.at(i).size(); j++) {
				delete m_sprites.at(i).at(j);
			}
		}
		delete m_texture;
	}

	const Sprite* SpritePack::addSprite(oe::utils::image_data sprite_texture) {
		auto usr_data = static_cast<__usr_data*>(m_usr_data);

		std::vector<Sprite*> vect;
		Sprite* sprite = new Sprite(this);
		sprite->position = { 0, 0 };
		sprite->size = { 1, 1 };
		vect.push_back(sprite);
		
		usr_data->m_rectangles.push_back(rectpack2D::rect_xywh(0, 0, sprite_texture.width + m_border, sprite_texture.height + m_border));
		usr_data->m_images.push_back(sprite_texture);
		m_sprites.push_back(vect);

		return vect.at(0);
	}

	const Sprite* SpritePack::addSprite(fs::path sprite_texture) {
		auto img = oe::utils::loadImage(sprite_texture);
		return addSprite(img);
	}

	std::vector<std::vector<const Sprite*>> SpritePack::addMultiSprite(oe::utils::image_data sprite_texture, const glm::vec2& sprite_count) {
		auto usr_data = static_cast<__usr_data*>(m_usr_data);

		std::vector<Sprite*> vect;
		for (size_t x = 0; x < sprite_count.x; x++) {
			for (size_t y = 0; y < sprite_count.y; y++) {
				Sprite* sprite = new Sprite(this);
				sprite->position = { x, y };
				sprite->size = sprite_count;
				vect.push_back(sprite);
			}
		}
		
		usr_data->m_rectangles.push_back(rectpack2D::rect_xywh(0, 0, sprite_texture.width + m_border, sprite_texture.height + m_border));
		usr_data->m_images.push_back(sprite_texture);
		m_sprites.push_back(vect);

		std::vector<std::vector<const Sprite*>> returned;
		for (size_t x = 0; x < sprite_count.x; x++) {
			std::vector<const Sprite*> row;
			for (size_t y = 0; y < sprite_count.y; y++) {
				row.push_back(vect.at(y + x * sprite_count.y));
			}
			returned.push_back(row);
		}

		return returned;
	}

	std::vector<std::vector<const Sprite*>> SpritePack::addMultiSprite(fs::path sprite_texture, const glm::vec2& sprite_count) {
		auto img = oe::utils::loadImage(sprite_texture);
		return addMultiSprite(img, sprite_count);
	}

	size_t coordsToIndex(size_t x, size_t y, size_t c, size_t width, size_t channels) {
		return channels * (x + y * width) + c;
	}

	void SpritePack::constructRepeat() {
		auto usr_data = static_cast<__usr_data*>(m_usr_data);

		// pack sprites
		const auto max_side = 1000;
		const auto discard_step = 1;

		auto report_successful = [](rect_type&) { return rectpack2D::callback_result::CONTINUE_PACKING; };
		auto report_unsuccessful = [](rect_type&) { return rectpack2D::callback_result::ABORT_PACKING; };

		const auto result_size = rectpack2D::find_best_packing<spaces_type>(
			usr_data->m_rectangles,
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
		for (size_t i = 0; i < usr_data->m_rectangles.size(); i++) {
			auto& rectangle = usr_data->m_rectangles.at(i);
			auto& image = usr_data->m_images.at(i);
			auto& sprite = m_sprites.at(i);
			for (size_t i = 0; i < sprite.size(); i++) {
				sprite.at(i)->size = glm::vec2(image.width / (float)pack_width, image.height / (float)pack_height) / sprite.at(i)->size;
				sprite.at(i)->position = glm::vec2(rectangle.x / (float)pack_width, rectangle.y / (float)pack_height) + sprite.at(i)->position * sprite.at(i)->size;
			}

			// print sprite img to final texture
			for (size_t y = 0; y < image.height; y++) {
				for (size_t x = 0; x < image.width; x++) {
					data[coordsToIndex(rectangle.x + x, rectangle.y + y, 0, pack_width, 4)] = image.data[coordsToIndex(x, y, 0, image.width, 4)];
					data[coordsToIndex(rectangle.x + x, rectangle.y + y, 1, pack_width, 4)] = image.data[coordsToIndex(x, y, 1, image.width, 4)];
					data[coordsToIndex(rectangle.x + x, rectangle.y + y, 2, pack_width, 4)] = image.data[coordsToIndex(x, y, 2, image.width, 4)];
					data[coordsToIndex(rectangle.x + x, rectangle.y + y, 3, pack_width, 4)] = image.data[coordsToIndex(x, y, 3, image.width, 4)];
				}
			}
		}


		auto img = oe::utils::image_data(data, pack_width, pack_height, 4);
		oe::utils::saveImage("pack.png", img);
		m_texture = new Texture();
		m_texture->load2D(img);
		delete[] data;
	}

	void SpritePack::construct() {
		auto usr_data = static_cast<__usr_data*>(m_usr_data);

		constructRepeat();

		for (size_t i = 0; i < usr_data->m_images.size(); i++) {
			delete[] usr_data->m_images.at(i).data;
		}
	}

	void SpritePack::bind() const {
		if (!m_texture) { oe_error_terminate("SpritePack was not constructed"); }
		m_texture->bind();
	}

}