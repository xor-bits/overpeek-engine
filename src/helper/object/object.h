#pragma once

#include <unordered_map>
#include "modifier/modifier.h"



namespace oe::simple {

	typedef unsigned long id;

	class Object {
	private:
		std::unordered_map<id, Modifier> m_modifiers;

	public:
		Modifier& GetModifier(id object) { return m_modifiers.at(object); }

	};

}