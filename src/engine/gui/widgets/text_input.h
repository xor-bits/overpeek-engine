#pragma once

#include "widget.h"



namespace oe::gui {

	class TextInput : public Widget {
	private:
		std::string m_string;
		// baked text label
		glm::vec4 m_color;
		void* m_state;
		bool m_selected;
		oe::graphics::Window* m_window_handle;
		const oe::graphics::Sprite* m_sprite;

		void(*m_callback_changed)(std::string& string);
		void(*m_callback_newline)(std::string& string);

		int m_font_size;

	public:
		// window_handle is used for clipboard
		TextInput(oe::graphics::Window* window_handle, const glm::vec2& bounding_box_size);

		// Inherited via Widget
		virtual void render(oe::graphics::Renderer& renderer) override;
		virtual void text(uint32_t codepoint, oe::modifiers mods) override;
		virtual void key(oe::keys key, oe::actions action, oe::modifiers mods) override;
		virtual void cursor(oe::mouse_buttons button, oe::actions action, const glm::vec2& cursor_window) override;

		inline std::string& string() { return m_string; };
		inline glm::vec4& color() { return m_color; };
		inline int& font_size() { return m_font_size; };
		inline void setSprite(const oe::graphics::Sprite* sprite) { m_sprite = sprite; }

		inline void setChangeCallback(void(*callback_changed)(std::string& string)) { m_callback_changed = callback_changed; }
		inline void setNewlineCallback(void(*callback_newline)(std::string& string)) { m_callback_newline = callback_newline; }
	};

}