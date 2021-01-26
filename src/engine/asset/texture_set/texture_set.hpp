#pragma once

#include "engine/utility/fileio.hpp"

#include <iostream>
#include <string_view>



namespace oe::graphics { struct Sprite; }
namespace oe::asset
{
	class TextureSet
	{
	public:
		static std::unordered_map<std::string_view, oe::graphics::Sprite> sprites();
		static oe::utils::image_data sprite(std::string_view name);

		/* constexpr static inline auto oe_logo_img = []() constexpr -> oe::utils::image_datac<oe::formats::rgb, 5, 5> {
			constexpr uint8_t oe_logo[] =
			{
				0xff, 0xff, 0xff,    0xff, 0xff, 0xff,    0xff, 0xff, 0xff,    0xff, 0xff, 0xff,    0xff, 0xff, 0xff,
				0xff, 0xff, 0xff,    0x10, 0x20, 0xff,    0xff, 0xff, 0xff,    0x10, 0x20, 0xff,    0xff, 0xff, 0xff,
				0xff, 0xff, 0xff,    0x10, 0x20, 0xff,    0xff, 0xff, 0xff,    0xff, 0xff, 0xff,    0xff, 0xff, 0xff,
				0xff, 0xff, 0xff,    0x10, 0x20, 0xff,    0x10, 0x20, 0xff,    0x10, 0x20, 0xff,    0xff, 0xff, 0xff,
				0xff, 0xff, 0xff,    0xff, 0xff, 0xff,    0xff, 0xff, 0xff,    0xff, 0xff, 0xff,    0xff, 0xff, 0xff
			};
			return { &oe_logo[0] };
		}(); */

		constexpr static inline auto pixel_setter(oe::formats format)
		{
			switch (format)
			{
			case oe::formats::rgba:
				return +[](const oe::utils::image_data& img, size_t i, const glm::vec<4, uint8_t>& color){
					*reinterpret_cast<glm::vec<4, uint8_t>*>(&img.data[i]) = oe::utils::to_rgba(color);
				};
			case oe::formats::rgb:
				return +[](const oe::utils::image_data& img, size_t i, const glm::vec<4, uint8_t>& color){
					*reinterpret_cast<glm::vec<3, uint8_t>*>(&img.data[i]) = oe::utils::to_rgb(color);
				};
			case oe::formats::mono:
				return +[](const oe::utils::image_data& img, size_t i, const glm::vec<4, uint8_t>& color){
					*reinterpret_cast<glm::vec<1, uint8_t>*>(&img.data[i]) = oe::utils::to_mono(color);
				};
			case oe::formats::none:
				break;
			}
			return +[](const oe::utils::image_data& /* img */, size_t /* i */, const glm::vec<4, uint8_t>& /* color */){};
		}

		constexpr static void generate_ellipse(const oe::utils::image_data& img)
		{
			const double radius_pow_2 = static_cast<double>(img.width * img.width);
			const double aspect = img.height / img.width;
			constexpr glm::vec<4, uint8_t> u_color_a = static_cast<glm::vec<4, uint8_t>>(oe::colors::white * 255.0f);
			constexpr glm::vec<4, uint8_t> u_color_b = static_cast<glm::vec<4, uint8_t>>(oe::colors::transparent * 255.0f);

			auto set_pixel = pixel_setter(img.format);
			for	(int x = 0; x < img.width; x++)
				for	(int y = 0; y < img.height; y++)
				{
					const size_t pixel = (x + (y * img.width)) * oe::utils::stb_i_channels(img.format);
					const double x_mid = static_cast<double>(x - img.width / 2);
					const double y_mid = static_cast<double>(y - img.height / 2);
					set_pixel(img, pixel, x_mid * x_mid * aspect + y_mid * y_mid <= radius_pow_2 ? u_color_a : u_color_b);
				}
		}

		template<int diameter = 16, oe::formats format = oe::formats::rgba>
		constexpr inline static oe::utils::image_datac<format, diameter, diameter> generate_circle()
		{
			return generate_ellipse<diameter, diameter, format>();
		}

		template<int width = 16, int height = 16, oe::formats format = oe::formats::rgba>
		constexpr inline static oe::utils::image_datac<format, width, height> generate_ellipse()
		{
			oe::utils::image_datac<format, std::abs(width), std::abs(height)> img{};
			generate_ellipse(img);
			return img;
		}

		inline static oe::utils::image_data generate_circle(int diameter = 16, oe::formats format = oe::formats::rgba)
		{
			return generate_ellipse(diameter, diameter, format);
		}

		inline static oe::utils::image_data generate_ellipse(int width = 16, int height = 16, oe::formats format = oe::formats::rgba)
		{
			oe::utils::image_data img{ format, std::abs(width), std::abs(height) };
			generate_ellipse(img);
			return img;
		}



		constexpr static void generate_checkerboard(const oe::utils::image_data& img, const oe::color& color_a = oe::colors::white, const oe::color& color_b = oe::colors::light_grey)
		{
			const glm::vec<4, uint8_t> u_color_a = static_cast<glm::vec<4, uint8_t>>(color_a * 255.0f);
			const glm::vec<4, uint8_t> u_color_b = static_cast<glm::vec<4, uint8_t>>(color_b * 255.0f);

			auto set_pixel = pixel_setter(img.format);
			for	(int x = 0; x < img.width; x++)
				for	(int y = 0; y < img.height; y++)
				{
					const size_t pixel = (x + (y * img.width)) * oe::utils::stb_i_channels(img.format);
					set_pixel(img, pixel, pixel % 2 == 0 ? u_color_a : u_color_b);
				}
		}

		template<int width = 2, int height = 2, oe::formats format = oe::formats::rgba>
		constexpr inline static oe::utils::image_datac<format, width, height> generate_checkerboard(const oe::color& color_a = oe::colors::white, const oe::color& color_b = oe::colors::light_grey)
		{
			oe::utils::image_datac<format, std::abs(width), std::abs(height)> img{};
			generate_checkerboard(img, color_a, color_b);
			return img;
		}

		static inline oe::utils::image_data generate_checkerboard(int width = 2, int height = 2, const oe::color& color_a = oe::colors::white, const oe::color& color_b = oe::colors::light_grey, oe::formats format = oe::formats::rgba)
		{
			oe::utils::image_data img{ format, std::abs(width), std::abs(height) };
			generate_checkerboard(img, color_a, color_b);
			return img;
		}
	};
} // namespace oe::asset::TextureSet