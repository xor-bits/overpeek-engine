#pragma once

#include <memory>

#include "../entity.hpp"



namespace oe::graphics { class Quad; }

namespace oe::ecs
{
	struct QuadComponent : public Entity
	{
		std::shared_ptr<oe::graphics::Quad> quad_holder;
	};
}