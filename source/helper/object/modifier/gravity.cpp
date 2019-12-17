#include "gravity.h"

#include "../../world.h"



namespace oe::simple::modifier {

	Gravity::Gravity(glm::vec2 gravity_vector) 
		: m_gravity_vector(gravity_vector)
	{

	}

	void Gravity::OnPreInit()
	{
		m_motion_modifier = m_gameobject->GetModifier();
	}

	void Gravity::OnUpdate()
	{
	}

}