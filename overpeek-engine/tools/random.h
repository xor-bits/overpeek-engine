#pragma once

namespace tools {

	class Random {
	public:
		static void seed();
		static float random(float min, float max);
	};

}