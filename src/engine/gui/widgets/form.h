#pragma once

#include "widget.h"



namespace oe::gui {

	struct FormInfo {
		glm::vec4 color           = oe::colors::transparent;
		glm::ivec2 size           = { 50, 50 };
		glm::vec2 offset_position = { 0, 0 };
		glm::vec2 align_parent    = oe::alignments::top_left;
		glm::vec2 align_render    = oe::alignments::top_left;
	};

	class Form : public Widget {
	public:
		FormInfo form_info;

	public:
		Form(const FormInfo& form_info);

		// Inherited via Widget
		virtual void render(oe::graphics::Renderer& renderer) override;
	};

}