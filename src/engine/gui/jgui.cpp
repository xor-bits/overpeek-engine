#include "jgui.hpp"

#include "engine/utility/formatted_error.hpp"
#include "engine/asset/texture_set/texture_set.hpp"
#include "engine/graphics/sprite.hpp"

#include "widgets/button_decorated.hpp"
#include "widgets/button.hpp"
#include "widgets/checkbox.hpp"
#include "widgets/color_input.hpp"
#include "widgets/color_picker.hpp"
#include "widgets/sprite_panel.hpp"
#include "widgets/text_panel.hpp"

#include <utf8.h>
#include <optional>



namespace oe::gui
{
	const static std::unordered_map<std::string_view, close_fn> close_fn_aliases {
		{ "never", 			close_fn::never },
		{ "immediately", 	close_fn::immediately },
		{ "leave_bbox", 	close_fn::leave_bbox },
		{ "click_out", 		close_fn::click_out },
	};

	const static std::unordered_map<std::string_view, open_fn> open_fn_aliases {
		{ "never", 			open_fn::never },
		{ "immediately", 	open_fn::immediately },
		{ "in_bbox", 		open_fn::in_bbox },
		{ "click_in", 		open_fn::click_in },
	};

	const static std::unordered_map<std::string_view, input_type> input_type_aliases {
		{ "dragger", 		input_type::dragger },
		{ "slider", 		input_type::slider },
		{ "none", 			input_type::none },
	};



	JGUI::JGUI(const oe::graphics::Window& window, bool _debugmode)
		: gui_manager(window)
		, debugmode(_debugmode)
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

	Widget::info_t JGUI::deserialize_base(const nlohmann::json& obj)
	{
		return {
			obj.contains("size_px")
				? glm::ivec2{
					obj["size_px"][0].get<int>(),
					obj["size_px"][1].get<int>() }
				: glm::ivec2{ 50, 50 },
			obj.contains("pos_px")
				? glm::ivec2{
					obj["pos_px"][0].get<int>(),
					obj["pos_px"][1].get<int>() }
				: glm::ivec2{ 0, 0 },
			obj.contains("min_size_px")
				? glm::ivec2{
					obj["min_size_px"][0].get<int>(),
					obj["min_size_px"][1].get<int>() }
				: glm::ivec2{ 0, 0 },
			obj.contains("max_size_px")
				? glm::ivec2{
					obj["max_size_px"][0].get<int>(),
					obj["max_size_px"][1].get<int>() }
				: glm::ivec2{ std::numeric_limits<int>::max(), std::numeric_limits<int>::max() },
			obj.contains("size_rel")
				? glm::vec2{
					obj["size_rel"][0].get<float>(),
					obj["size_rel"][1].get<float>() }
				: glm::vec2{ 0.0f, 0.0f },
			obj.contains("pos_rel")
				? glm::vec2{
					obj["pos_rel"][0].get<float>(),
					obj["pos_rel"][1].get<float>() }
				: glm::vec2{ 0.0f, 0.0f },
			obj.contains("offset_rel")
				? glm::vec2{
					obj["offset_rel"][0].get<float>(),
					obj["offset_rel"][1].get<float>() }
				: glm::vec2{ 0.0f, 0.0f },
			obj.contains("toggled")
				? obj["toggled"].get<bool>()
				: true,
		};
	}

	const oe::graphics::Sprite* JGUI::deserialize_sprite(const nlohmann::json& obj)
	{
		const oe::graphics::Sprite* sprite = &sprite_empty;
		if(obj.contains("sprite") && !obj["sprite"].is_null())
		{
			const auto sprite_name = obj["sprite"].get<std::string>();
			const auto sprite_name_iter = sprite_map.find(sprite_name);
			if(sprite_name_iter == sprite_map.end())
				spdlog::warn("asked sprite: '{}' not found, using empty sprite", sprite_name);
			else
				sprite = &(*sprite_name_iter).second;
		}

		return sprite;
	}

	oe::utils::FontFile JGUI::deserialize_font(const nlohmann::json& obj)
	{
		oe::utils::FontFile font{};
		if(obj.contains("font") && !obj["font"].is_null())
		{
			const auto font_name = obj["font"].get<std::string>();
			const auto font_name_iter = font_map.find(font_name);
			if(font_name_iter == font_map.end())
				spdlog::warn("asked font: '{}' not found, using the default", font_name);
			else
				font = (*font_name_iter).second;
		}

		return font;
	}

	oe::utils::color_string<char32_t> JGUI::deserialize_text(const nlohmann::json& obj)
	{
		std::vector<oe::utils::color_string_part<char32_t>> csv{};
		std::u32string csv_owner{};

		if(obj.contains("text"))
		{
			const nlohmann::json& text_obj = obj["text"];
			for (auto it = text_obj.cbegin(); it != text_obj.cend(); it++)
			{
				const auto end = csv_owner.cend();
				csv_owner += utf8::utf8to32(it.key());
				const auto d = std::distance(end, csv_owner.cend());
				csv.push_back(std::make_tuple(
					std::basic_string_view<char32_t>{
						&*end,
						static_cast<size_t>(std::max(static_cast<decltype(d)>(0), d)) },
					oe::color{ 
						it.value()[0].get<float>(),
						it.value()[1].get<float>(),
						it.value()[2].get<float>(),
						it.value()[3].get<float>() }
				));
			}
		}
		return { csv.begin(), csv.end() };
	}

	TextOptions JGUI::deserialize_text_options(const nlohmann::json& obj_holder)
	{
		const nlohmann::json& obj = obj_holder["text_options"];
		return {
			obj.contains("enabled")
				? obj["enabled"].get<bool>()
				: true,
			deserialize_font(obj),
			obj.contains("align")
				? glm::vec2{ obj["align"][0].get<float>(), obj["align"][1].get<float>() }
				: glm::vec2{ 0.0f, 0.0f },
			obj.contains("color")
				? oe::color{
					obj["color"][0].get<float>(),
					obj["color"][1].get<float>(),
					obj["color"][2].get<float>(),
					obj["color"][3].get<float>() }
				: oe::colors::white,
			obj.contains("background_color")
				? oe::color{
					obj["background_color"][0].get<float>(),
					obj["background_color"][1].get<float>(),
					obj["background_color"][2].get<float>(),
					obj["background_color"][3].get<float>() }
				: oe::colors::transparent,
			obj.contains("resolution")
				? obj["resolution"].get<uint16_t>()
				: (uint16_t)16,
			obj.contains("scale")
				? glm::vec2{ obj["scale"][0].get<float>(), obj["scale"][1].get<float>() }
				: glm::vec2{ 16.0f, 16.0f },
			obj.contains("weight")
				? obj["weight"].get<float>()
				: 0.25f,
			obj.contains("outline_color")
				? oe::color{
					obj["outline_color"][0].get<float>(),
					obj["outline_color"][1].get<float>(),
					obj["outline_color"][2].get<float>(),
					obj["outline_color"][3].get<float>() }
				: oe::colors::black,
			obj.contains("outline_weight")
				? obj["outline_weight"].get<float>()
				: 0.3f,
			obj.contains("anti_alias")
				? obj["anti_alias"].get<float>()
				: 0.2f,
			obj.contains("advance_padding")
				? obj["advance_padding"].get<float>()
				: 0.1f,
		};
	}
	
	std::shared_ptr<Widget> JGUI::load_widget(const nlohmann::json& widget, const std::vector<std::any>& ref_list, std::string_view parent_path)
	{
		std::shared_ptr<Widget> parent{};
		if(!parent_path.empty())
			parent = widget_map.at(std::string{ parent_path }).second;
		const std::string parent_path_opt = !parent_path.empty()
			? std::string{parent_path} + "."
			: "";

		// name
		const auto path_name = widget.contains("name")
			? parent_path_opt + widget["name"].get<std::string>()
			: parent_path_opt + "element_" + std::to_string(widget_map.size());

		// type
		const auto w_type_str = widget["type"].get<std::string>();
		const auto w_type_iter = aliases.find(w_type_str);
		if(w_type_iter == aliases.end())
		{
			spdlog::warn("Ignoring invalid type: {}", w_type_str);
			return {};
		}
		const widgets w_type = (*w_type_iter).second;

		// ref
		const std::optional<size_t> ref = widget.contains("ref")
			? std::optional<size_t>{ widget["ref"].get<size_t>() }
			: std::optional<size_t>{};

		// base widget info
		const Widget::info_t base_info = deserialize_base(widget);

		// debug log
		if(debugmode)
			spdlog::debug(
				"\nLoading widget named: {}, of type: {}, with {} refs.\nWidget::info_t: {}",
				path_name,
				true_aliases.at(w_type),
				ref.has_value() ? 1 : 0,
				base_info);

		auto create_lambda = [this, w_type, &ref, &ref_list](const std::shared_ptr<Widget>& parent, std::string_view name, const auto& info){
			
			auto& entry = widget_map[std::string{ name }];
			entry.first = w_type;

			using widget_t = typename std::remove_reference_t<std::remove_cv_t<decltype(info)>>::widget_t;
			using value_t = typename widget_t::value_t;
			if(debugmode)
				spdlog::debug("Widget create_lambda: {}", typeid(widget_t).name());
			if constexpr(std::is_same_v<value_t, void>)
			{
				if (parent)							entry.second = parent->create(info);
				else								entry.second = gui_manager.create(info);
			}
			else
			{
				std::optional<value_t*> ref_val{};
				if(ref.has_value() && ref.value() < ref_list.size())
					ref_val = std::any_cast<value_t*>(ref_list.at(ref.value()));
				else if(ref.has_value())
					spdlog::warn("Ignoring out of bounds ref");

				if(debugmode && ref_val.has_value())
					spdlog::debug("Widget has ref: {}", *ref_val.value());

				if(parent && ref_val.has_value())	entry.second = parent->create(info, *ref_val.value());
				else if (parent)					entry.second = parent->create(info);
				else if (ref_val.has_value())		entry.second = gui_manager.create(info, *ref_val.value());
				else								entry.second = gui_manager.create(info);
			}
			
			return std::make_pair(name, entry.second);
		};

		// specifics
		std::pair<std::string_view, std::shared_ptr<Widget>> this_widget{};
		switch (w_type)
		{
		case widgets::button_decorated:
			this_widget = create_lambda(parent, path_name, DecoratedButton::info_t {
				deserialize_text(widget),
				deserialize_text_options(widget),
				widget.contains("color")
					? oe::color{
						widget["color"][0].get<float>(),
						widget["color"][1].get<float>(),
						widget["color"][2].get<float>(),
						widget["color"][3].get<float>() }
					: oe::colors::dark_grey,
				deserialize_sprite(widget),
				{ base_info },
			});
			break;
		
		case widgets::button:
			this_widget = create_lambda(parent, path_name, Button::info_t { base_info });
			break;
		
		case widgets::checkbox:
			this_widget = create_lambda(parent, path_name, Checkbox::info_t {
				widget.contains("initial")
					? widget["initial"].get<bool>()
					: false,
				widget.contains("color_back")
					? oe::color{
						widget["color_back"][0].get<float>(),
						widget["color_back"][1].get<float>(),
						widget["color_back"][2].get<float>(),
						widget["color_back"][3].get<float>() }
					: oe::colors::dark_grey,
				widget.contains("color_mark")
					? oe::color{
						widget["color_mark"][0].get<float>(),
						widget["color_mark"][1].get<float>(),
						widget["color_mark"][2].get<float>(),
						widget["color_mark"][3].get<float>() }
					: oe::colors::lime,
				deserialize_sprite(widget),
				base_info,
			});
			break;

		case widgets::color_input:
			this_widget = create_lambda(parent, path_name, ColorInput::info_t {
				widget.contains("initial")
					? oe::color{
						widget["initial"][0].get<float>(),
						widget["initial"][1].get<float>(),
						widget["initial"][2].get<float>(),
						widget["initial"][3].get<float>() }
					: oe::colors::dark_grey,
				&ColorInput::info_t::default_formatter, /* widget.contains("text_format")
					? widget["text_format"].get<std::string>()
					: "{}:{:.1f}", c, c_val), */
				deserialize_text_options(widget),
				widget.contains("color")
					? oe::color{
						widget["color"][0].get<float>(),
						widget["color"][1].get<float>(),
						widget["color"][2].get<float>(),
						widget["color"][3].get<float>() }
					: oe::colors::dark_grey,
				deserialize_sprite(widget),
				widget.contains("popup_picker")
					? widget["popup_picker"].get<bool>()
					: true,
				widget.contains("popup_close")
					? [](const std::string& name)
					{
						if(name == "never")
							return close_fn::never;
						else if(name == "immediately")
							return close_fn::immediately;
						else if(name == "leave_bbox")
							return close_fn::leave_bbox;
						else if(name == "click_out")
							return close_fn::click_out;
						else
							spdlog::warn("Invalid popup_close: '{}', using default", name);
						return close_fn::leave_bbox;
					}(widget["popup_close"].get<std::string>())
					: close_fn::leave_bbox,
				widget.contains("popup_open")
					? [](const std::string& name)
					{
						if(name == "never")
							return open_fn::never;
						else if(name == "immediately")
							return open_fn::immediately;
						else if(name == "in_bbox")
							return open_fn::in_bbox;
						else if(name == "click_in")
							return open_fn::click_in;
						else
							spdlog::warn("Invalid popup_open: '{}', using default", name);
						return open_fn::click_in;
					}(widget["popup_open"].get<std::string>())
					: open_fn::click_in,
				widget.contains("input")
					? [](const std::string& name)
					{
						if(name == "dragger")
							return input_type::dragger;
						else if(name == "slider")
							return input_type::slider;
						else if(name == "none")
							return input_type::none;
						else
							spdlog::warn("Invalid input: '{}', using default", name);
						return input_type::slider;
					}(widget["input"].get<std::string>())
					: input_type::slider,
				base_info,
			});
			break;

		case widgets::sprite_panel:
			this_widget = create_lambda(parent, path_name, SpritePanel::info_t {
				widget.contains("color")
					? oe::color{
						widget["color"][0].get<float>(),
						widget["color"][1].get<float>(),
						widget["color"][2].get<float>(),
						widget["color"][3].get<float>() }
					: oe::colors::white,
				deserialize_sprite(widget),
				widget.contains("rotation")
					? widget["rotation"].get<float>()
					: 0.0f,
				widget.contains("align")
					? glm::vec2{
						widget["align"][0].get<float>(),
						widget["align"][1].get<float>() }
					: oe::alignments::center_center,
				base_info,
			});
			break;

		case widgets::text_panel:
			this_widget = create_lambda(parent, path_name, TextPanel::info_t {
				deserialize_text(widget),
				deserialize_text_options(widget),
				base_info,
			});
			break;

		default:
			spdlog::warn("Invalid widget type, not yet implemented");
			break; // TODO: remove when all widget handlers are implemented here
		}

		// subwidgets
		if(widget.contains("subwidgets"))
			load(widget["subwidgets"], {}, this_widget.first);

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