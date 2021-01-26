#include "texture_set.hpp"

#include "engine/asset/asset_loader.hpp"
#include "engine/utility/formatted_error.hpp"
#include "engine/interfacegen.hpp"
#include "engine/graphics/sprite.hpp"
#include <nlohmann/json.hpp>



namespace oe::asset
{
	std::unordered_map<std::string_view, oe::graphics::Sprite> TextureSet::sprites()
	{
		auto image = AssetLoader::resource_image("texture/set.png");
		
		oe::TextureInfo ti{};
		ti.data = image.data;
		ti.data_format = image.format;
		ti.empty = false;
		ti.filter = oe::texture_filter::nearest;
		ti.generate_mipmaps = false;
		ti.size_offset = { { image.width, 0 }, { image.height, 0 } };
		ti.wrap = oe::texture_wrap::clamp_to_border;
		
		oe::graphics::Texture texture{ ti };
		std::unordered_map<std::string_view, oe::graphics::Sprite> map{};
		
		try{
			const nlohmann::json json = nlohmann::json::parse(AssetLoader::resource_string("texture/set.json"));
			const glm::vec2 size = {
				json["size"][0].get<float>(),
				json["size"][1].get<float>()
			};
			const nlohmann::json& sprites = json["sprites"];
			for (auto it = sprites.cbegin(); it != sprites.cend(); it++)
			{
				oe::graphics::Sprite& sprite = map[it.key()];
				sprite.m_owner = texture;
				sprite.opacity = false;
				sprite.position = glm::vec2{
					it.value()["pos"][0].get<float>(),
					it.value()["pos"][1].get<float>()
				} / size;
				sprite.size = glm::vec2{
					it.value()["size"][0].get<float>(),
					it.value()["size"][1].get<float>()
				} / size;
			}
		}catch(const std::exception& e){
			throw oe::utils::formatted_error("Asset data likely corrupted: {}", e.what());
		}

		return map;
	}
	
	oe::utils::image_data TextureSet::sprite(std::string_view name)
	{
		auto image = AssetLoader::resource_image("texture/set.png");
		int x = 0, y = 0, w = -1, h = -1;
		
		try{
			const nlohmann::json json = nlohmann::json::parse(AssetLoader::resource_string("texture/set.json"));
			const glm::vec2 size = {
				json["size"][0].get<float>(),
				json["size"][1].get<float>()
			};
			const nlohmann::json& sprite = json["sprites"][name.data()];
			x = sprite["pos"][0].get<int>();
			y = sprite["pos"][1].get<int>();
			w = sprite["size"][0].get<int>();
			h = sprite["size"][1].get<int>();
		}catch(const std::exception& e){
			throw oe::utils::formatted_error("Asset data likely corrupted: {}", e.what());
		}

		return image.crop(x, y, w, h);
	}
}