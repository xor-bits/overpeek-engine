#include "form.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/graphics/interface/renderer.hpp"



namespace oe::gui {

	Form::Form(const FormInfo& _form_info) 
		: Widget::Widget(_form_info.size, _form_info.align_parent, _form_info.align_render, _form_info.offset_position)
		, form_info(_form_info)
	{
	}

	Form::~Form()
	{
	}

}