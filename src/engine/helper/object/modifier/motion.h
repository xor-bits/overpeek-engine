#pragma once

#include "internal_libs.h"

#include "modifier.h"



namespace oe::simple::modifier {

	class Motion : public Modifier {
	private:
		glm::vec2 m_position;
		glm::vec2 m_velocity;

	public:
		// constructor(s) and deconstructor
		Motion(glm::vec2 position, glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

		// getters
		inline glm::vec2& Position() { return m_position; }
		inline glm::vec2& Velocity() { return m_velocity; }

		// inherited via Modifier
		virtual void OnUpdate() override;
	};

}