#pragma once

#include "dependencies/spdlog/spdlog.h"
#include "dependencies/glm/glm.hpp"
#include "dependencies/glm/matrix.hpp"
#include "dependencies/glm/gtc/matrix_transform.hpp"
#include "dependencies/glm/gtc/type_ptr.hpp"

namespace oe {

	static void init() {
		spdlog::set_pattern("%^[%T] [%l]:%$ %v");
		spdlog::set_level(spdlog::level::level_enum::trace);

		spdlog::info("Engine initialized");
	}

}