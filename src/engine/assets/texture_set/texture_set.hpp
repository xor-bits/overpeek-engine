#pragma once

#include "engine/utility/fileio.hpp"



namespace oe::assets
{
	class TextureSet // this 'fake' class exists just for looks and consistency, pls don't bully me
	{
	public:
		static constexpr uint8_t oe_logo[] =
		{
			0xff, 0xff, 0xff,    0xff, 0xff, 0xff,    0xff, 0xff, 0xff,    0xff, 0xff, 0xff,    0xff, 0xff, 0xff,
			0xff, 0xff, 0xff,    0x10, 0x20, 0xff,    0xff, 0xff, 0xff,    0x10, 0x20, 0xff,    0xff, 0xff, 0xff,
			0xff, 0xff, 0xff,    0x10, 0x20, 0xff,    0xff, 0xff, 0xff,    0xff, 0xff, 0xff,    0xff, 0xff, 0xff,
			0xff, 0xff, 0xff,    0x10, 0x20, 0xff,    0x10, 0x20, 0xff,    0x10, 0x20, 0xff,    0xff, 0xff, 0xff,
			0xff, 0xff, 0xff,    0xff, 0xff, 0xff,    0xff, 0xff, 0xff,    0xff, 0xff, 0xff,    0xff, 0xff, 0xff,
		};

		static inline const oe::utils::image_data oe_logo_img = { &oe_logo[0], oe::formats::rgb, 5, 5 };

		// resolution = 2*radius
		static inline const oe::utils::image_data generate_circle(const size_t radius = 16)
		{
			size_t edge_l = 2 * radius;
			oe::utils::image_data img(oe::formats::rgba, edge_l, edge_l);

			for	(size_t i = 0; i < edge_l * edge_l * 4; i += 4)
			{
				const size_t pixel_i = i / 4;
				const size_t x_mid = (pixel_i % edge_l) - radius;
				const size_t y_mid = (pixel_i / edge_l) - radius;

				img.data[i + 0] = 0xff;
				img.data[i + 1] = 0xff;
				img.data[i + 2] = 0xff;
				img.data[i + 3] = x_mid*x_mid + y_mid*y_mid <= radius*radius ? 0xff : 0x00;
			}

			return img;
		}

		// resolution = 2*radius
		static inline const oe::utils::image_data generate_circle_bordered(const size_t radius = 16)
		{
			size_t edge_l = 2 * radius;
			oe::utils::image_data img(oe::formats::rgba, edge_l, edge_l);

			for (size_t i = 0; i < edge_l * edge_l * 4; i += 4)
			{
				const size_t pixel_i = i / 4;
				const size_t x_mid = (pixel_i % edge_l) - radius;
				const size_t y_mid = (pixel_i / edge_l) - radius;

				img.data[i + 0] = x_mid * x_mid + y_mid * y_mid <= radius * radius * 0.65 ? 0xff : 0x00;
				img.data[i + 1] = img.data[i + 0];
				img.data[i + 2] = img.data[i + 0];
				img.data[i + 3] = x_mid * x_mid + y_mid * y_mid <= radius * radius ? 0xff : 0x00;
			}

			return img;
		}

		// resolution = 2*radius
		static inline const oe::utils::image_data generate_checkerboard(const glm::vec4& color_a = oe::colors::white, const glm::vec4& color_b = oe::colors::light_grey)
		{
			constexpr size_t edge_l = 2;
			oe::utils::image_data img(oe::formats::rgba, edge_l, edge_l);

			reinterpret_cast<glm::vec<4, uint8_t>*>(img.data)[0] = color_a;
			reinterpret_cast<glm::vec<4, uint8_t>*>(img.data)[2] = color_a;
			reinterpret_cast<glm::vec<4, uint8_t>*>(img.data)[1] = color_b;
			reinterpret_cast<glm::vec<4, uint8_t>*>(img.data)[3] = color_b;

			return img;
		}
	};
} // namespace oe::assets::TextureSet