#include "form.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/graphics/interface/renderer.hpp"



namespace oe::gui {

	Form::Form(const FormInfo& _form_info) 
		: Widget::Widget(_form_info.widget_info)
		, form_info(_form_info)
	{
	}

	Form::~Form()
	{
	}

}