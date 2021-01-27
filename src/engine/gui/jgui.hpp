#include "gui_manager.hpp"
#include "engine/utility/color_string.hpp"
#include "engine/graphics/sprite.hpp"

#include <nlohmann/json.hpp>
#include <any>



namespace oe::gui
{
	enum class widgets {
		none,
		button_decorated,
		button,
		checkbox,
		color_input,
		color_picker,
		graph,
		list,
		number_input,
		slider_input,
		sprite_panel,
		text_input,
		text_panel,
		vec_slider,
	};

	const static std::unordered_map<std::string_view, widgets> aliases {
		{ "button_decorated", 	widgets::button_decorated },
		{ "decorated_button", 	widgets::button_decorated },
		{ "btn_d", 				widgets::button_decorated },
		{ "d_btn", 				widgets::button_decorated },
		{ "btnd", 				widgets::button_decorated },

		{ "button", 			widgets::button },
		{ "button_hidden", 		widgets::button },
		{ "hidden_button", 		widgets::button },
		{ "btn", 				widgets::button },
		{ "btn_h", 				widgets::button },
		{ "h_btn", 				widgets::button },
		{ "btnh", 				widgets::button },

		{ "checkbox", 			widgets::checkbox },
		{ "cb", 				widgets::checkbox },
		{ "toggle", 			widgets::checkbox },
		{ "tl", 				widgets::checkbox },

		{ "color_input", 		widgets::color_input },
		{ "color_slider", 		widgets::color_input },
		{ "c_input", 			widgets::color_input },
		{ "c_slider", 			widgets::color_input },
		{ "color_i", 			widgets::color_input },
		{ "cs", 				widgets::color_input },

		{ "color_picker", 		widgets::color_picker },
		{ "color_wheel", 		widgets::color_picker },
		{ "c_picker", 			widgets::color_picker },
		{ "c_wheel", 			widgets::color_picker },
		{ "color_p", 			widgets::color_picker },
		{ "color_w", 			widgets::color_picker },
		{ "cp", 				widgets::color_picker },
		{ "cw", 				widgets::color_picker },

		{ "graph", 				widgets::graph },
		{ "graphic",			widgets::graph },
		{ "plot",				widgets::graph },
		{ "g", 					widgets::graph },

		{ "list", 				widgets::list },
		{ "ls", 				widgets::list },

		{ "number_input", 		widgets::number_input },
		{ "number_box",			widgets::number_input },
		{ "ni",					widgets::number_input },
		{ "nb",					widgets::number_input },

		{ "slider_input", 		widgets::slider_input },
		{ "slider",				widgets::slider_input },
		{ "number_slider",		widgets::slider_input },
		{ "si",					widgets::slider_input },

		{ "sprite_panel",		widgets::sprite_panel },
		{ "s_panel",			widgets::sprite_panel },
		{ "sp",					widgets::sprite_panel },

		{ "text_input",			widgets::text_input },
		{ "text_box",			widgets::text_input },
		{ "input",				widgets::text_input },
		{ "ti",					widgets::text_input },

		{ "text_panel",			widgets::text_panel },
		{ "t_panel",			widgets::text_panel },
		{ "tp",					widgets::text_panel },

		{ "vec_slider",			widgets::vec_slider },
		{ "vector_slider",		widgets::vec_slider },
		{ "v_slider",			widgets::vec_slider },
		{ "vs",					widgets::vec_slider },
	};

	const static std::unordered_map<widgets, std::string_view> true_aliases {
		{ widgets::button_decorated, "button_decorated" },
		{ widgets::button, "button" },
		{ widgets::checkbox, "checkbox" },
		{ widgets::color_input, "color_input" },
		{ widgets::color_picker, "color_picker" },
		{ widgets::graph, "graph" },
		{ widgets::list, "list" },
		{ widgets::number_input, "number_input" },
		{ widgets::slider_input, "slider_input" },
		{ widgets::sprite_panel, "sprite_panel" },
		{ widgets::text_input, "text_input" },
		{ widgets::text_panel, "text_panel" },
		{ widgets::vec_slider, "vec_slider" },
	};


	/* load example: [ // widget array
		{ // widget
			// drawn position = pos_px + parent.pos_px + parent.size_px * pos_rel - size_px * offset_rel
			// drawn size = clamp(size_px + parent.size_px * size_rel, min_size_px, max_size_px)
			
			// optional name
			// - default = element_<n>, where <n> = element index
			"name": "element_0",

			// widget modified value refrence index
			// - default = null (no refrence, widget owns its value)
			"ref": null,

			// multiple types with multiple aliases corresponding to gui/widgets/ *
			// - no default, missing are ignored with a warn
			"type": "slider",

			// alignment point pixel offset
			// - default = [ 0, 0 ]
			"pos_px": [ 5, 5 ],

			// alignment point offset relative to the parent widget
			// - default = [ 0.0, 0.0 ]
			"pos_rel": [ 0.0, 0.0 ],

			// relative offset from the alignment point
			// - default = [ 0.0, 0.0 ]
			"offset_rel": [ 0.0, 0.0 ],

			// size in pixels
			// - default = [ 50, 50 ]
			"size_px": [ 200, 80 ],

			// size relative to the parent widget
			// - default = [ 0.0, 0.0 ]
			"size_rel": [ 0.0, 0.0 ],
			
			// maximum possible size in pixels
			// - default = [ <int max>, <int max> ]
			"max_size_px": [ 200, 80 ],
			
			// minimum possible size in pixels
			// - default = [ 0, 0 ]
			"min_size_px": [ 200, 80 ],

			// optional visibility
			// - default = true
			"toggled": true,

			// specifics
			// defaults are the set values

			// button_decorated type specifics
			"text": { "Button": [ 1.0, 1.0, 1.0, 1.0 ] },
			"text_options": {
				"enabled": true,
				"font": null, // null font for default
				"align": [ 0.log_checkbox, 1.0, 1.0, 1.0 ],
				"weight": 0.25, // font "boldness"
				"outline_color": [ 0.0, 0.0, 0.0, 1.0 ],
				"outline_weight": 0.3,
				"anti_alias": 0.2,
				"advance_padding": 0.1
			},
			"color": [ 0.25, 0.25, 0.25, 1.0 ],
			"sprite": null, // null or name of the sprite

			// button type specifics
			// none

			// checkbox type specifics
			"initial": false,
			"color": [ 0.25, 0.25, 0.25, 1.0 ], // background color
			"color_mark": [ 0.5, 1.0, 0.0, 1.0 ],
			"sprite": null,

			// color_input type specifics
			"initial": [ 1.0, 0.0, 0.0, 1.0 ],
			// fmt format_string ex. "{}:{}" where
			// - the first is the short name of the color channel
			// - the second is the float value of the channel
			// null = "{}:{:.1f}"
			"text_format": null,
			"text_options": {}, // ... button_decorated has an example of this ...
			"color": [ 0.25, 0.25, 0.25, 1.0 ],
			"sprite": null,
			"popup_picker": true,
			"popup_close": "leave_bbox",
			"popup_open": "click_in",
			"input": "slider",

			// sprite_panel type specifics
			"color": [ 1.0, 1.0, 1.0, 1.0 ],
			"sprite": null,
			"rotation": 0.0, // radians
			"align": [ 0.5, 0.5 ], // rotation point

			// subwidgets
			"subwidgets": [
				{ // widget
					...
				}
			]
		}
	] */
	class JGUI
	{
	private:
		GUI gui_manager;
		nlohmann::json base_json{};
		std::unordered_map<std::string, std::pair<widgets, std::shared_ptr<Widget>>> widget_map{};
		std::unordered_map<std::string, oe::graphics::Sprite> sprite_map{};
		std::unordered_map<std::string, oe::utils::FontFile> font_map{};
		oe::graphics::Sprite sprite_empty{};

		Widget::info_t deserialize_base(const nlohmann::json& obj);
		const oe::graphics::Sprite* deserialize_sprite(const nlohmann::json& obj);
		oe::utils::FontFile deserialize_font(const nlohmann::json& obj);
		oe::utils::color_string<char32_t> deserialize_text(const nlohmann::json& obj);
		TextOptions deserialize_text_options(const nlohmann::json& obj);

		bool debugmode = false;

	public:
		JGUI(const oe::graphics::Window& window, bool debugmode = false);

		// loaders
		
		std::shared_ptr<Widget> load_widget(const nlohmann::json& widget, const std::vector<std::any>& ref_list = {}, std::string_view parent = {});

		inline void load(const nlohmann::json& json_data, const std::vector<std::any>& ref_list = {}, std::string_view parent = {})
		{
			base_json = json_data;

			// iterate widget array
			for (const auto& widget : json_data)
				load_widget(widget, ref_list, parent);
		}

		template<typename T>
		std::shared_ptr<T> load_widget_T(const nlohmann::json& widget, const std::vector<std::any>& ref_list = {}, std::string_view parent = {})
		{
			return std::dynamic_pointer_cast<T>(load_widget(widget, ref_list, parent));
		}

		inline void load_sprite(std::string_view name, const oe::graphics::Sprite& sp)
		{
			sprite_map[std::string{ name }] = sp;
		}

		inline void load_font(std::string_view name, oe::utils::FontFile font)
		{
			font_map[std::string{ name }] = font;
		}

		// dumpers

		[[nodiscard]] inline nlohmann::json dump_widget(std::string_view widget) const
		{
			return base_json[widget.data()];
		}

		[[nodiscard]] inline nlohmann::json dump() const
		{
			return base_json;
		}

		inline void list() const
		{
			for (const auto& widget : widget_map)
				spdlog::debug("Widget: {}", widget.first);
		}

		// base/getters

		[[nodiscard]] inline std::shared_ptr<oe::gui::Widget> at(std::string_view path)
		{
			return widget_map.at(std::string{ path }).second;
		}

		template<typename T>
		[[nodiscard]] inline std::shared_ptr<T> at_T(std::string_view path)
		{
			auto widget = std::dynamic_pointer_cast<T>(at(path));
			if(!widget)
				throw std::runtime_error("Widget type not what asked");
			return widget;
		}
		
		[[nodiscard]] inline GUI& gui()
		{
			return gui_manager;
		}

		inline void reload()
		{
			load(base_json);
		}

		inline void clear()
		{
			base_json = {};
			widget_map.clear();
			gui_manager.clear(); 
		}

		inline void render()
		{
			gui_manager.render();
		}

		// non-methods

		static std::vector<std::string> parse_path(std::string_view path);
	};
}