#include "motion.h"



namespace oe::simple::modifier {
	
	Motion::Motion(glm::vec2 position, glm::vec2 velocity) 
		: m_position(position), m_velocity(velocity) 
	{
		//
	}

	void Motion::OnUpdate()
	{
		m_position += m_velocity * timeframe;
	}

}
