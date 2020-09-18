#include "form.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/graphics/renderer.hpp"



namespace oe::gui {

	Form::Form(Widget* parent, GUI& gui_manager, const FormInfo& _form_info) 
		: Widget::Widget(parent, gui_manager, _form_info.widget_info)
		, form_info(_form_info)
	{
	}

	Form::~Form()
	{
	}

}