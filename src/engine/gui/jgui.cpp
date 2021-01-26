#include "jgui.hpp"

#include "engine/utility/formatted_error.hpp"
#include "engine/asset/texture_set/texture_set.hpp"
#include "engine/graphics/sprite.hpp"
#include "widgets/checkbox.hpp"



namespace oe::gui
{
	JGUI::JGUI(const oe::graphics::Window& window)
		: gui_manager(window)
	{
		auto empty_img = oe::asset::TextureSet::sprite("empty");

		oe::TextureInfo ti{};
		ti.data = empty_img.data;
		ti.data_format = empty_img.format;
		ti.empty = false;
		ti.filter = oe::texture_filter::nearest;
		ti.generate_mipmaps = false;
		ti.size_offset = { { empty_img.width, 0 }, { empty_img.height, 0 } };
		ti.wrap = oe::texture_wrap::clamp_to_border;
		
		oe::graphics::Texture texture{ ti };
		sprite_empty.m_owner = texture;
		sprite_empty.opacity = false;
		sprite_empty.position = { 0.0f, 0.0f };
		sprite_empty.size = { 1.0f, 1.0f };
	}
	
	std::shared_ptr<Widget> JGUI::load_widget(const nlohmann::json& widget, std::string_view parent_path)
	{
		std::shared_ptr<Widget> parent{};
		if(!parent_path.empty())
			parent = widget_map.at(parent_path).second;

		// name
		const auto optional_name = widget.contains("name")
			? widget["name"].get<std::string>()
			: std::string{ "element_" } + std::to_string(widget_map.size());

		// type
		const auto w_type_str = widget["type"].get<std::string>();
		const auto w_type_iter = aliases.find(w_type_str);
		if(w_type_iter == aliases.end())
		{
			spdlog::warn("Ignoring invalid type: {}", w_type_str);
			return {};
		}
		const widgets w_type = (*w_type_iter).second;

		// base widget info
		const Widget::info_t base_info
		{
			widget.contains("size_px")
			? glm::ivec2{ widget["size_px"][0].get<int>(), widget["size_px"][1].get<int>() }
			: glm::ivec2{ 50, 50 },
			widget.contains("pos_px")
			? glm::ivec2{ widget["pos_px"][0].get<int>(), widget["pos_px"][1].get<int>() }
			: glm::ivec2{ 0, 0 },
			widget.contains("min_size_px")
			? glm::ivec2{ widget["min_size_px"][0].get<int>(), widget["min_size_px"][1].get<int>() }
			: glm::ivec2{ 0, 0 },
			widget.contains("max_size_px")
			? glm::ivec2{ widget["max_size_px"][0].get<int>(), widget["max_size_px"][1].get<int>() }
			: glm::ivec2{ std::numeric_limits<int>::max(), std::numeric_limits<int>::max() },
			widget.contains("size_rel")
			? glm::vec2{ widget["size_rel"][0].get<float>(), widget["size_rel"][1].get<float>() }
			: glm::vec2{ 0.0f, 0.0f },
			widget.contains("pos_rel")
			? glm::vec2{ widget["pos_rel"][0].get<float>(), widget["pos_rel"][1].get<float>() }
			: glm::vec2{ 0.0f, 0.0f },
			widget.contains("offset_rel")
			? glm::vec2{ widget["offset_rel"][0].get<float>(), widget["offset_rel"][1].get<float>() }
			: glm::vec2{ 0.0f, 0.0f },
			widget.contains("toggled")
			? widget["toggled"].get<bool>()
			: true,
		};

		auto create_lambda = [this, parent_path, w_type](const std::shared_ptr<Widget>& parent, const std::string& name, const auto& info){
			if(!parent)
			{
				const std::string& path = name;
				auto& entry = widget_map[name];
				entry.first = w_type;
				entry.second = gui_manager.create(info);
				return std::make_pair(std::string_view{ path }, entry.second);
			}
			else
			{
				const std::string path = std::string{parent_path} + "." + name;
				auto& entry = widget_map[path];
				entry.first = w_type;
				entry.second = parent->create(info);
				return std::make_pair(std::string_view{ path }, entry.second);
			}
		};

		// specifics
		std::pair<std::string_view, std::shared_ptr<Widget>> this_widget{};
		switch (w_type)
		{
		case widgets::button_decorated:
			/* code */
			break;
		
		case widgets::button:
			/* code */
			break;
		
		case widgets::checkbox:
			{
				const oe::graphics::Sprite* sprite = &sprite_empty;
				if(widget.contains("sprite") && !widget["sprite"].is_null())
				{
					const auto sprite_name = widget["sprite"].get<std::string>();
					const auto sprite_name_iter = sprite_map.find(sprite_name);
					if(sprite_name_iter == sprite_map.end())
					{
						spdlog::warn("asked sprite: '{}' not found, using empty sprite", sprite_name);
						sprite = &sprite_empty;
					}
					else
						sprite = &(*sprite_name_iter).second;
				}

				const Checkbox::info_t w_info
				{
					widget.contains("initial")
					? widget["initial"].get<bool>()
					: false,
					widget.contains("color_back")
					? oe::color{ widget["color_back"][0].get<float>(), widget["color_back"][1].get<float>(), widget["color_back"][2].get<float>(), widget["color_back"][3].get<float>() }
					: oe::colors::dark_grey,
					widget.contains("color_mark")
					? oe::color{ widget["color_mark"][0].get<float>(), widget["color_mark"][1].get<float>(), widget["color_mark"][2].get<float>(), widget["color_mark"][3].get<float>() }
					: oe::colors::lime,
					sprite,
					base_info,
				};

				this_widget = create_lambda(parent, optional_name, w_info);
			}
			break;

		default: break;
		}

		// subwidgets
		if(widget.contains("subwidgets"))
			load(widget["subwidgets"], this_widget.first);

		return this_widget.second;
	}

	std::vector<std::string> JGUI::parse_path(std::string_view path)
	{
		std::stringstream ss{ path.data() };
		std::string part{};
		std::vector<std::string> parsed;

		while(!std::getline(ss, part, '.').fail())
			parsed.push_back(part);

		return parsed;
	}
}