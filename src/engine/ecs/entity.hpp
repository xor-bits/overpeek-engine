#pragma once

#include "world.hpp"

#include <entt/entt.hpp>



namespace oe::ecs
{
	struct World;

/*#define OE_HAS_TEST(fn, name) \
	template <typename T> \
	class name \
	{ \
		using a = uint8_t; \
		using b = uint16_t; \
		template <typename T> static a test(decltype(&T::fn)); \
		template <typename T> static b test(...); \
	public: \
		enum { value = sizeof(test<T>(0)) == sizeof(a) }; \
	}

	OE_HAS_TEST(on_init, has_on_init);
	OE_HAS_TEST(m_entity, has_entity_ptr);
	OE_HAS_TEST(on_cleanup, has_on_cleanup);*/

	struct Behaviour;

	struct Entity
	{
		Entity() = default;
		Entity(World* world); // create new entity
		Entity(entt::entity ent, World* world); // find existing entity

		// get component
		// returned value might get invalidated after next Entity::setComponent()
		template<typename T>
		T& getComponent()
		{
			return m_world->m_scene.get<T>(m_entity);
		}

		// get component
		// returned value might get invalidated after next Entity::setComponent()
		template<typename T>
		T& getScriptComponent() const
		{
			return *m_world->m_scene.get<std::unique_ptr<T>>(m_entity).get();
		}

		// set or reset component
		// returned value might get invalidated after next Entity::setComponent() or Entity::setScriptComponent()
		template<typename T, typename... Args>
		T& setComponent(Args&& ... args)
		{
			return m_world->m_scene.emplace_or_replace<T>(m_entity, args...);
		}

		template<typename T>
		static void on_destroy(entt::registry& r, entt::entity e)
		{
			r.get<T>(e).on_cleanup();
		}

		// set or reset script component
		// only invalidated after another Entity::setScriptComponent<T>() or Entity::removeComponent<T>()
		template<typename T, typename ... Args>
		T& setScriptComponent(Args&& ... args)
		{
			auto& c = *m_world->m_scene.emplace_or_replace<std::unique_ptr<T>>(m_entity, std::make_unique<T>()).get();
			c.m_world = m_world;
			c.m_entity = m_entity;
			c.on_init(args...);
			
			static bool connect_once = false;
			
			if(!connect_once) connect_once = [&] { // will be initialized exactly once for every component type
				m_world->m_scene.on_destroy<T>().template connect<&Entity::on_destroy<T>>();
				return true;
			}();
			
			return c;
		}

		template<typename T>
		void removeComponent()
		{
			m_world->m_scene.remove_if_exists<T>(m_entity);
		}
		
		template<typename T>
		bool hasComponent() const
		{
			return m_world->m_scene.has<T>(m_entity);
		}

		bool valid() const;

	public:
		entt::entity m_entity{};
		World* m_world{};
	};

	struct Behaviour : public Entity
	{
		Behaviour() = default;

		virtual void on_cleanup() {}
	};
}