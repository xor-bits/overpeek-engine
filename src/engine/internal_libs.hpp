#pragma once

#include <spdlog/spdlog.h>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define MINIZ_NO_ZLIB_COMPATIBLE_NAMES
#include <miniz.h>

#include <entt/entt.hpp>

#ifdef BUILD_VULKAN
#include <shaderc/shaderc.hpp>
#endif