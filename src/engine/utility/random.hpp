#pragma once

#include <string>
#include <random>
#include "engine/internal_libs.hpp"



namespace oe::utils {

	class Random {
	private:
		static Random* singleton;
		std::mt19937 gen;

	public:
		Random() { seed(); } // not limited to only singleton
		Random(const Random&) = delete;
		static Random& getSingleton() {
			if (!singleton) singleton = new Random();
			return *singleton;
		}

	public:
		void seed(uint32_t value);
		template<typename T>
		void seed(const T& value) { seed(static_cast<uint32_t>(std::hash<T>{}(value))); }
		void seed() { seed(static_cast<uint32_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count())); }
		
		template<typename T = float>
		T randomf(T min = std::numeric_limits<T>().min(), T max = std::numeric_limits<T>().max())
		{
    		std::uniform_real_distribution<> dist(0, 10);
			return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
		}
		template<typename T = int>
		T randomi(T min = std::numeric_limits<T>().min(), T max = std::numeric_limits<T>().max())
		{
			return min + rand() % (max - min);
		}

		glm::vec2 randomVec2(float min, float max);
		glm::vec3 randomVec3(float min, float max);
		glm::vec4 randomVec4(float min, float max);
	};

}