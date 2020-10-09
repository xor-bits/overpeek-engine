#include "world.hpp"

#include "entity.hpp"



namespace oe::ecs
{
	World::World()
	{
	}

	Entity World::create()
	{
		return Entity{ this };
	}

	Entity World::create(entt::entity entity)
	{
		return Entity{ entity, this };
	}

	void World::destroy(Entity entity)
	{
		destroy(entity.m_entity);
	}

	void World::destroy(entt::entity entity)
	{
		m_scene.destroy(entity);
	}

	void World::clear()
	{
		m_scene.clear();
		m_renderer.clear();
		m_debug_renderer.clear();
	}

	size_t World::count() const
	{
		return m_scene.alive();
	}
}