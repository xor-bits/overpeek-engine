#pragma once

#include "engine/enum.hpp"
#include "engine/internal_libs.hpp"



namespace oe::gui
{
	struct BaseHoverEvent
	{
		glm::fvec2 relf_pos;
	};

	struct BaseUseEvent
	{
		oe::actions action;
		oe::mouse_buttons button;
		oe::modifiers modifier;
	};
} // namespace oe::gui
