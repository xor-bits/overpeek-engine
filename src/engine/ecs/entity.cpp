#include "entity.hpp"
#include "world.hpp"

#include "components/quad.hpp"
#include <spdlog/spdlog.h>



namespace oe::ecs
{
	Entity::Entity(World* world)
		: m_entity(world->m_scene.create())
		, m_world(world)
	{}

	Entity::Entity(entt::entity ent, World* world)
		: m_entity(ent)
		, m_world(world)
	{}

	bool Entity::valid() const
	{
		return m_world->m_scene.valid(m_entity);
	}
}