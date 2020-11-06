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

	private:
		static AssetLoader* singleton;
		AssetLoader();

		using res_variant = std::variant<std::string, oe::utils::image_data, oe::utils::audio_data, oe::utils::byte_string>;
		std::unordered_map< std::string, res_variant > m_resources;
	
	public:
		AssetLoader(const AssetLoader&) = delete;
		static AssetLoader& get() {
			if (!singleton) singleton = new AssetLoader();
			return *singleton;
		}
		
		[[nodiscard]] oe::utils::FileIO path() const noexcept;
		[[nodiscard]] const res_variant& resource(const std::string& resource_path, asset_type type);
		[[nodiscard]] const std::string& resource_string(const std::string& resource_path);
		[[nodiscard]] const oe::utils::image_data& resource_texture(const std::string& resource_path);
		[[nodiscard]] const oe::utils::audio_data& resource_audio(const std::string& resource_path);
		[[nodiscard]] const oe::utils::byte_string& resource_bytes(const std::string& resource_path);
	};
}