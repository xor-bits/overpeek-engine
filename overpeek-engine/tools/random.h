#pragma once

namespace tools {

	class Random {
	public:
		static void seed();
		static void seed(int seed);
		static float random(float min, float max);
	};

}