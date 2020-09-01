#pragma once

#include <memory>

#include "../entity.hpp"



namespace oe::graphics { struct Quad; }

namespace oe::ecs
{
	struct QuadComponent : public Entity
	{
		std::shared_ptr<oe::graphics::Quad> quad_holder;
	};
}