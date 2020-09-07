#pragma once

#include "widget.hpp"



namespace oe::graphics { class Quad; }
namespace oe::gui
{
	struct FormInfo
	{
		WidgetInfo widget_info = { { 50, 50 }, { 0, 0 }, oe::alignments::top_left, oe::alignments::top_left };
	};

	class GUI;
	class Form : public Widget
	{
	public:
		FormInfo form_info;

	public:
		Form(const FormInfo& form_info);
		~Form();
	};

}