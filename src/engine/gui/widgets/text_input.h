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
		const oe::graphics::Sprite* m_sprite;

		void(*m_callback_changed)(std::string& string);
		void(*m_callback_newline)(std::string& string);

		int m_font_size;

	public:
		TextInput(const glm::vec2& bounding_box_size);

		// Inherited via Widget
		virtual void render(oe::graphics::Renderer& renderer) override;
		virtual void text(unsigned int character, int mods) override;
		virtual void key(int key, int action, int mods) override;
		virtual void cursor(int button, int action, const glm::vec2& cursor_window) override;

		inline std::string& string() { return m_string; };
		inline glm::vec4& color() { return m_color; };
		inline int& font_size() { return m_font_size; };
		inline void setSprite(const oe::graphics::Sprite* sprite) { m_sprite = sprite; }

		inline void setChangeCallback(void(*callback_changed)(std::string& string)) { m_callback_changed = callback_changed; }
		inline void setNewlineCallback(void(*callback_newline)(std::string& string)) { m_callback_newline = callback_newline; }
	};

}