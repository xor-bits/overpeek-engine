#pragma once

// #define SPDLOG_HEADER_ONLY
// #define SPDLOG_FMT_EXTERNAL
#ifndef FMT_HEADER_ONLY
#  define FMT_HEADER_ONLY 1
#endif
#include <spdlog/spdlog.h>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>

#include <gcem.hpp>
