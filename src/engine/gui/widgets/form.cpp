#include "form.hpp"



namespace oe::gui {

	Form::Form(const FormInfo& _form_info) 
		: Widget::Widget(_form_info.size, _form_info.align_parent, _form_info.align_render, _form_info.offset_position)
		, form_info(_form_info)
	{
	
	}

	void Form::render(oe::graphics::Renderer& renderer) {
		if (form_info.color != oe::colors::transparent) {
			renderer.submit(render_position, size, 0, form_info.color);
		}
	}

}