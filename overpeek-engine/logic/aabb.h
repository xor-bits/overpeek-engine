#pragma once

#include <glm/glm.hpp>

namespace logic {

	bool AABB(glm::fvec2 aPos, glm::fvec2 aSize, glm::fvec2 bPos, glm::fvec2 bSize) {
		return	bPos.x < aPos.x + aSize.x && aPos.x < bPos.x + bSize.x
			&&  bPos.y < aPos.y + aSize.y && aPos.y < bPos.y + bSize.y;
	}

}