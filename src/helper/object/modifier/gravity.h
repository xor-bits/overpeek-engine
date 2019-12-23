#pragma once

#include "internal_libs.h"

#include "modifier.h"
#include "motion.h"



namespace oe::simple::modifier {

	class Gravity : public Modifier {
	private:
		glm::vec2 m_gravity_vector;
		Motion* m_motion_modifier;

	public:
		// constructor(s) and deconstructor
		Gravity(glm::vec2 gravity_vector = glm::vec2(0.0f, -9.80665f));

		// getters
		glm::vec2& GravityVector() { return m_gravity_vector; }

		// inherited via Modifier
		virtual void OnPreInit() override;
		virtual void OnUpdate() override;

	};

}