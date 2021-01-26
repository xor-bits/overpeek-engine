#include "asset_loader.hpp"
#include "engine/utility/formatted_error.hpp"

#include <cmrc/cmrc.hpp>
CMRC_DECLARE(asset);



namespace oe::asset
{
	[[nodiscard]] AssetLoader::res_variant AssetLoader::resource(const std::string& resource_path, asset_type type)
	{
		auto fs = cmrc::asset::get_filesystem();
		auto file = fs.open("asset/" + resource_path);

		switch (type)
		{
		case asset_type::string:
			return std::string{ file.begin(), file.end() };
		case asset_type::texture:
			return oe::utils::image_data{ reinterpret_cast<const uint8_t*>(&*file.begin()), file.size() };
		case asset_type::audio:
			return oe::utils::audio_data{ reinterpret_cast<const uint8_t*>(&*file.begin()), file.size() };
		case asset_type::bytes:
			return oe::utils::byte_string{ file.begin(), file.end() };
		}

		throw oe::utils::formatted_error("Unknown asset type: {}", static_cast<size_t>(type));
	}

	constexpr std::string_view unexpected_variant_type = "Variant type not what expected";

	[[nodiscard]] std::string AssetLoader::resource_string(const std::string& resource_path)
	{
		using T = std::string;
		auto variant = resource(resource_path, asset_type::string);
		if(!std::holds_alternative<T>(variant))
			throw oe::utils::formatted_error(unexpected_variant_type);
		return std::get<T>(variant);
	}

	[[nodiscard]] oe::utils::image_data AssetLoader::resource_image(const std::string& resource_path)
	{
		using T = oe::utils::image_data;
		auto variant = resource(resource_path, asset_type::texture);
		if(!std::holds_alternative<T>(variant))
			throw oe::utils::formatted_error(unexpected_variant_type);
		return std::get<T>(variant);
	}

	[[nodiscard]] oe::utils::audio_data AssetLoader::resource_audio(const std::string& resource_path)
	{
		using T = oe::utils::audio_data;
		auto variant = resource(resource_path, asset_type::audio);
		if(!std::holds_alternative<T>(variant))
			throw oe::utils::formatted_error(unexpected_variant_type);
		return std::get<T>(variant);
	}

	[[nodiscard]] oe::utils::byte_string AssetLoader::resource_bytes(const std::string& resource_path)
	{
		using T = oe::utils::byte_string;
		auto variant = resource(resource_path, asset_type::bytes);
		if(!std::holds_alternative<T>(variant))
			throw oe::utils::formatted_error(unexpected_variant_type);
		return std::get<T>(variant);
	}

}