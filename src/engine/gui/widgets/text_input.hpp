#pragma once

#include "widget.hpp"
#include "engine/graphics/textLabel.hpp"



namespace oe::graphics { struct Quad; }

namespace oe::gui
{
	static constexpr char32_t filter_none[]          = U"";
	static constexpr char32_t filter_letters_upper[] = U"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static constexpr char32_t filter_letters_lower[] = U"abcdefghijklmnopqrstuvwxyz";
	static constexpr char32_t filter_numbers[]       = U"0123456789";
	static constexpr char32_t filter_newline[]       = U"\n";

	struct TextInputInfo
	{
		glm::ivec2 offset_position         = { 0, 0 };
		glm::vec2 align_text               = oe::alignments::center_center;
		std::u32string text                = U"";
		std::u32string filter              = filter_none;
		size_t font_size                   = 16;
		std::string font_path              = ""; // empty for gui default
		glm::vec4 color                    = oe::colors::dark_grey;
		const oe::graphics::Sprite* sprite = nullptr; // must be set
		
		WidgetInfo widget_info             = { { 100, 100 }, { 3, 3 }, oe::alignments::center_center, oe::alignments::center_center };
	};

	struct TextInputHoverEvent
	{};

	struct TextInputUseEvent
	{
		oe::actions action;
		oe::mouse_buttons button;
		oe::modifiers modifier;
	};

	struct TextInputChangedEvent
	{
		char32_t character;
		std::u32string_view value;
	};

	class TextInput : public Widget
	{
	private:
		oe::graphics::u32TextLabel* label;
		std::shared_ptr<oe::graphics::Quad> quad;
		std::shared_ptr<oe::graphics::Quad> text_quad;

	public:
		TextInputInfo text_input_info;
	
	private:
		void* m_state;
		bool m_selected;
		bool m_filtering;

	public:
		// window_handle is used for clipboard
		TextInput(const TextInputInfo& text_input_info);
		~TextInput();

		virtual void managerAssigned() override;
		virtual void managerUnassigned() override;

		oe::graphics::u32TextLabel* debug() { return label; };

	private:
		// events
		void on_render(const GUIRenderEvent& event);
		void on_codepoint(const CodepointEvent& event);
		void on_key(const KeyboardEvent& event);
		void on_button(const MouseButtonEvent& event);
	};
}