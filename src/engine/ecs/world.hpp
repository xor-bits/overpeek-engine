#pragma once

#include "engine/graphics/interface/renderer.hpp"
#include "engine/assets/default_shader/default_shader.hpp"

#include <entt/entt.hpp>



namespace oe::ecs
{
	struct Entity;

	struct World
	{
		World();

		oe::assets::DefaultShader m_shader{ oe::polygon_mode::fill };
		oe::graphics::Renderer m_renderer{ { 10000, oe::types::dynamic_type, oe::types::static_type, nullptr } };

		oe::assets::DefaultShader m_debug_shader{ oe::polygon_mode::lines };
		oe::graphics::Renderer m_debug_renderer{ { 10000, oe::types::dynamic_type, oe::types::static_type, nullptr } };

		entt::registry m_scene;

		Entity create(); // create new entity
		Entity create(entt::entity entity); // find existing entity
		void clear();
		size_t count() const;
	};
}