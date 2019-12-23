#pragma once

#include <unordered_map>
#include "object/object.h"



namespace oe::simple {

	class World {
	private:
		static std::unordered_map<id, Object> m_gameobjects;

	public:
		static Object& GetObject(id object) { return m_gameobjects.at(object); }

	};

}