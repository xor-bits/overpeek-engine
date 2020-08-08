#pragma once

#include "engine/enum.hpp"

#include <unordered_set>

#ifndef DISABLE_NULL_SPRITE_CHECK
#define NULL_SPRITE_CHECK(x) if (!x) { spdlog::warn("No sprite for {}", typeid(*this).name()); return; }
#endif /* NULL_SPRITE_CHECK */



namespace oe::gui { struct GUIRenderEvent; class GUI; }

namespace oe::gui
{
	class Widget
	{
	private:
		Widget* m_parent;
		std::unordered_set<Widget*> m_nodes;
		void setParent(Widget* parent);
		static float z_acc;

	protected:
		glm::ivec2 topleft_position = { 0, 0 };
		glm::ivec2 render_position = { 0, 0 };

		GUI* m_gui_manager;
		float z;
		entt::dispatcher dispatcher;

	public:
		glm::ivec2 size = { 0, 0 };
		glm::ivec2 offset_position = { 0, 0 };
		glm::vec2 align_parent = ::oe::alignments::center_center;
		glm::vec2 align_render = ::oe::alignments::center_center;
		bool toggled = true;

	public:
		Widget(const glm::ivec2& size, const glm::vec2& align_parent, const glm::vec2& align_render, const glm::ivec2& offset_position);
		virtual ~Widget();

		// connect event
		template<typename Event, auto Listener, typename Instance>
		void connect_listener(const Instance& instance)
		{
			dispatcher.sink<Event>().template connect<Listener>(instance);
		}
		template<typename Event, auto Listener>
		void connect_listener()
		{
			dispatcher.sink<Event>().template connect<Listener>();
		}
		// connect event
		template<typename Event, auto Listener, typename Instance>
		void disconnect_listener(const Instance& instance)
		{
			dispatcher.sink<Event>().template disconnect<Listener>(instance);
		}
		template<typename Event, auto Listener>
		void disconnect_listener()
		{
			dispatcher.sink<Event>().template disconnect<Listener>();
		}

		// unassign before reassigning
		virtual void managerAssigned(GUI* gui_manager);
		virtual void managerUnassigned(GUI* gui_manager);

		// this class will take ownership of this pointer
		virtual void addSubWidget(Widget* widget);
		virtual void removeSubWidget(Widget* widget);

	private:
		// events
		void on_render(const GUIRenderEvent& event);

	friend class GUI;
	};
}