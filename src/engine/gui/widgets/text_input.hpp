#pragma once

#include "widget.hpp"



namespace oe::gui {

#define TEXT_CALLBACK_WRAPPER(x) [&](const std::string & string) { x(string); }
	typedef std::function<void(const std::string & string)> text_callback;

	const static std::string filter_none = "";
	const static std::string filter_letters_upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const static std::string filter_letters_lower = "abcdefghijklmnopqrstuvwxyz";
	const static std::string filter_numbers       = "0123456789";
	const static std::string filter_symbols       = "-_.:,;<>|^¨~'*´`?\\/()[]{}&%¤#\"!@£$§=";
	const static std::string filter_newline       = "\n";

	struct TextInputInfo {
		text_callback callback_changed        = nullptr;
		text_callback callback_newline        = nullptr;
		glm::ivec2 size                       = { 100, 100 };
		glm::vec2 offset_position             = { 0, 0 };
		glm::vec2 align_parent                = oe::alignments::center_center;
		glm::vec2 align_render                = oe::alignments::center_center;
		glm::vec2 align_text                  = oe::alignments::center_center;
		std::string text                      = "";
		std::string filter                    = filter_none;
		int font_size                         = 16;
		glm::vec4 color                       = oe::colors::dark_grey;
		const oe::graphics::Sprite* sprite    = nullptr; // must be set
		oe::graphics::Window* window_handle = nullptr; // must be set
	};

	class TextInput : public Widget {
	public:
		TextInputInfo text_input_info;
	
	private:
		void* m_state;
		bool m_selected;
		bool m_filtering;

	public:
		// window_handle is used for clipboard
		TextInput(const TextInputInfo& text_input_info);
		virtual ~TextInput() override;

		// Inherited via Widget
		virtual void render(oe::graphics::Renderer& renderer) override;
		virtual void text(uint32_t codepoint, oe::modifiers mods) override;
		virtual void key(oe::keys key, oe::actions action, oe::modifiers mods) override;
		virtual void cursor(oe::mouse_buttons button, oe::actions action, const glm::vec2& cursor_window) override;
	};

}