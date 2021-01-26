#pragma once

#include <variant>
#include "engine/utility/fileio.hpp"



namespace oe::asset
{
	class AssetLoader
	{
	public:
		enum class asset_type
		{
			string, texture, audio, bytes
		};
		using res_variant = std::variant<std::string, oe::utils::image_data, oe::utils::audio_data, oe::utils::byte_string>;
	
	public:
		[[nodiscard]] static res_variant resource(const std::string& resource_path, asset_type type);
		[[nodiscard]] static std::string resource_string(const std::string& resource_path);
		[[nodiscard]] static oe::utils::image_data resource_image(const std::string& resource_path);
		[[nodiscard]] static oe::utils::audio_data resource_audio(const std::string& resource_path);
		[[nodiscard]] static oe::utils::byte_string resource_bytes(const std::string& resource_path);
	};
}