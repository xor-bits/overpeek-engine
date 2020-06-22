#pragma once

#include "widget.hpp"



namespace oe::graphics { class Quad; }
namespace oe::gui {

	struct FormInfo {
		glm::ivec2 size            = { 50, 50 };
		glm::ivec2 offset_position = { 0, 0 };
		glm::vec2 align_parent     = oe::alignments::top_left;
		glm::vec2 align_render     = oe::alignments::top_left;
	};

	class GUI;
	class Form : public Widget {
	public:
		FormInfo form_info;

	public:
		Form(const FormInfo& form_info);
		~Form();
	};

}