#pragma once

#include "widget.hpp"
#include "engine/graphics/textLabel.hpp"



namespace oe::graphics { class Quad; }
namespace oe::gui {

#define TEXT_CALLBACK_WRAPPER(x) [&](const std::u32string & string) { x(string); }
	typedef std::function<void(const std::u32string & string)> textbox_callback;

	const static std::u32string filter_none          = U"";
	const static std::u32string filter_letters_upper = U"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const static std::u32string filter_letters_lower = U"abcdefghijklmnopqrstuvwxyz";
	const static std::u32string filter_numbers       = U"0123456789";
	const static std::u32string filter_newline       = U"\n";

	struct TextInputInfo {
		textbox_callback callback_changed  = nullptr;
		textbox_callback callback_newline  = nullptr;
		glm::ivec2 size                    = { 100, 100 };
		glm::ivec2 padding                 = { 3, 3 };
		glm::ivec2 offset_position         = { 0, 0 };
		glm::vec2 align_parent             = oe::alignments::center_center;
		glm::vec2 align_render             = oe::alignments::center_center;
		glm::vec2 align_text               = oe::alignments::center_center;
		std::u32string text                = U"";
		std::u32string filter              = filter_none;
		int font_size                      = 16;
		glm::vec4 color                    = oe::colors::dark_grey;
		const oe::graphics::Sprite* sprite = nullptr; // must be set
	};

	struct GUIRenderEvent;
	class GUI;
	class TextInput : public Widget {
	private:
		oe::graphics::u32TextLabel* label;
		oe::graphics::Quad* quad;
		oe::graphics::Quad* text_quad;

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

		virtual void managerAssigned(GUI* gui_manager) override;
		virtual void managerUnassigned(GUI* gui_manager) override;

		oe::graphics::u32TextLabel* debug() { return label; };

		// events
		void on_render(const GUIRenderEvent& event);
		void on_codepoint(const CodepointEvent& event);
		void on_key(const KeyboardEvent& event);
		void on_button(const MouseButtonEvent& event);
	};

}