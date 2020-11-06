#pragma once

#include "engine/graphics/renderer.hpp"
#include "engine/asset/default_shader/default_shader.hpp"

#include <entt/entt.hpp>



namespace oe::ecs
{
	struct Entity;

	struct World
	{
		World();

		oe::asset::DefaultShader m_shader{ oe::polygon_mode::fill };
		oe::graphics::Renderer m_renderer{ 10000 };

		oe::asset::DefaultShader m_debug_shader{ oe::polygon_mode::lines };
		oe::graphics::Renderer m_debug_renderer{ 10000 };

		entt::registry m_scene;

		Entity create(); // create new entity
		Entity create(entt::entity entity); // find existing entity

		void destroy(Entity entity);
		void destroy(entt::entity entity);
		
		void clear();
		size_t count() const;
	};
}