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
		
		template<typename T = float, typename = std::enable_if_t<std::is_floating_point_v<T>>>
		T randomf(T min = std::numeric_limits<T>().min(), T max = std::numeric_limits<T>().max())
		{
			std::uniform_real_distribution<T> dist(min, max);
			return dist(gen);
		}
		template<typename T = float, typename = std::enable_if_t<std::is_floating_point_v<T>>>
		T normalf(T mean = 0.0, T deviation = 1.0)
		{
			std::normal_distribution<T> dist(mean, deviation);
			return dist(gen);
		}
		template<typename T = int, typename = std::enable_if_t<std::is_integral_v<T>>>
		T randomi(T min = std::numeric_limits<T>().min(), T max = std::numeric_limits<T>().max())
		{
			std::uniform_int_distribution<T> dist(min, max);
			return dist(gen);
		}

		glm::vec2 randomVec2(float min, float max);
		glm::vec3 randomVec3(float min, float max);
		glm::vec4 randomVec4(float min, float max);
	};

}