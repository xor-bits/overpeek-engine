#include "asset_loader.hpp"
#include "engine/utility/formatted_error.hpp"



namespace oe::asset
{
	AssetLoader* AssetLoader::singleton = nullptr;



	AssetLoader::AssetLoader()
	{}

	[[nodiscard]] oe::utils::FileIO AssetLoader::path() const noexcept
	{
		auto& engine = oe::Engine::getSingleton();
		return engine.engine_info.asset_path;
	}

	[[nodiscard]] const AssetLoader::res_variant& AssetLoader::resource(const std::string& resource_path, asset_type type)
	{
		auto iter = m_resources.find(resource_path);
		if(iter != m_resources.end())
			return iter->second;

		auto& engine = oe::Engine::getSingleton();

		oe::utils::FileIO path(engine.engine_info.asset_path);
		path.open(resource_path);

		switch (type)
		{
		case asset_type::string:
			return m_resources[resource_path] = path.read<std::string>();
		case asset_type::texture:
			return m_resources[resource_path] = path.read<oe::utils::image_data>();
		case asset_type::audio:
			return m_resources[resource_path] = path.read<oe::utils::audio_data>();
		case asset_type::bytes:
			return m_resources[resource_path] = path.read<oe::utils::byte_string>();
		}

		throw oe::utils::formatted_error("Unknown asset type: {}", static_cast<size_t>(type));
	}

	constexpr std::string_view unexpected_variant_type = "Variant type not what expected";

	[[nodiscard]] const std::string& AssetLoader::resource_string(const std::string& resource_path)
	{
		using T = std::string;
		const auto& variant = resource(resource_path, asset_type::string);
		if(!std::holds_alternative<T>(variant))
			throw oe::utils::formatted_error(unexpected_variant_type);
		return std::get<T>(variant);
	}

	[[nodiscard]] const oe::utils::image_data& AssetLoader::resource_texture(const std::string& resource_path)
	{
		using T = oe::utils::image_data;
		const auto& variant = resource(resource_path, asset_type::texture);
		if(!std::holds_alternative<T>(variant))
			throw oe::utils::formatted_error(unexpected_variant_type);
		return std::get<T>(variant);
	}

	[[nodiscard]] const oe::utils::audio_data& AssetLoader::resource_audio(const std::string& resource_path)
	{
		using T = oe::utils::audio_data;
		const auto& variant = resource(resource_path, asset_type::audio);
		if(!std::holds_alternative<T>(variant))
			throw oe::utils::formatted_error(unexpected_variant_type);
		return std::get<T>(variant);
	}

	[[nodiscard]] const oe::utils::byte_string& AssetLoader::resource_bytes(const std::string& resource_path)
	{
		using T = oe::utils::byte_string;
		const auto& variant = resource(resource_path, asset_type::bytes);
		if(!std::holds_alternative<T>(variant))
			throw oe::utils::formatted_error(unexpected_variant_type);
		return std::get<T>(variant);
	}

}