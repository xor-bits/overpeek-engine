#pragma once

#include "widget.hpp"



namespace oe::graphics { class Quad; class TextLabel; }
namespace oe::gui {

#define TEXT_CALLBACK_WRAPPER(x) [&](const std::string & string) { x(string); }
	typedef std::function<void(const std::wstring & string)> textbox_callback;

	const static std::wstring filter_none          = L"";
	const static std::wstring filter_letters_upper = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const static std::wstring filter_letters_lower = L"abcdefghijklmnopqrstuvwxyz";
	const static std::wstring filter_numbers       = L"0123456789";
	const static std::wstring filter_newline       = L"\n";

	struct TextInputInfo {
		textbox_callback callback_changed  = nullptr;
		textbox_callback callback_newline  = nullptr;
		glm::ivec2 size                    = { 100, 100 };
		glm::ivec2 padding                 = { 3, 3 };
		glm::ivec2 offset_position         = { 0, 0 };
		glm::vec2 align_parent             = oe::alignments::center_center;
		glm::vec2 align_render             = oe::alignments::center_center;
		glm::vec2 align_text               = oe::alignments::center_center;
		std::wstring text                  = L"";
		std::wstring filter                = filter_none;
		int font_size                      = 16;
		glm::vec4 color                    = oe::colors::dark_grey;
		const oe::graphics::Sprite* sprite = nullptr; // must be set
	};

	struct GUIRenderEvent;
	class GUI;
	class TextInput : public Widget {
	private:
		oe::graphics::TextLabel* label;
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

		oe::graphics::TextLabel* debug() { return label; };

		// events
		void on_render(const GUIRenderEvent& event);
		void on_codepoint(const CodepointEvent& event);
		void on_key(const KeyboardEvent& event);
		void on_button(const MouseButtonEvent& event);
	};

}