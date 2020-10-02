#pragma once

#include "widget.hpp"



namespace oe::gui
{
	class GUI;
	class Form : public Widget
	{
	public:
		Form(Widget* parent, GUI& gui_manager, const WidgetInfo& widget_info = {}) : Widget(parent, gui_manager, widget_info) {}
		friend class GUI;
	};

}