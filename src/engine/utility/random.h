#pragma once

#define PRIME_A 98909
#define PRIME_B 37993
#define PRIME_C 56671
#define PRIME_H 5903

#include <string>

namespace oe {

	class Random {
	public:
		//Seed = 0 is time
		static void seed(int seed = 0);
		static float random(float min, float max);

		static unsigned long hash(const char* str);
		static unsigned long hash(std::string str) { return hash(str.c_str()); }
		static unsigned long hash(long n) { return hash(std::to_string(n)); }
		static unsigned long hash(double n) { return hash(std::to_string(n)); }
		static unsigned long hash(long x, long y) { return hash(std::to_string(x) + std::string(".") + std::to_string(y)); }
		static unsigned long hash(double x, double y) { return hash(std::to_string(x) + std::string(".") + std::to_string(y)); }
		static unsigned long hash(long x, long y, long z) { return hash(std::to_string(x) + std::string(".") + std::to_string(y) + std::string(".") + std::to_string(z)); }
		static unsigned long hash(double x, double y, double z) { return hash(std::to_string(x) + std::string(".") + std::to_string(y) + std::string(".") + std::to_string(z)); }
	};

}