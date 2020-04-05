#pragma once

#include <string>
#include "engine/internal_libs.hpp"



namespace oe::utils {

	class Random {
	private:
		static Random* singleton;
		Random() { seed(0);  }

	public:
		Random(const Random&) = delete;
		static Random& getSingleton() {
			if (!singleton) singleton = new Random();
			return *singleton;
		}

	public:
		void seed(uint32_t seed = 0);
		
		float randomf(float min, float max);
		int randomi(int min, int max);

		glm::vec2 randomVec2(float min, float max);
		glm::vec3 randomVec3(float min, float max);
		glm::vec4 randomVec4(float min, float max);
	};

}