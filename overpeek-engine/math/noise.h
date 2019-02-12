#pragma once
#include <cstdint>
#include <numeric>
#include <algorithm>
#include <random>

namespace oe {
	class Noise {
	private:
		static int p[512];

		static double Fade(double t);
		static double Lerp(double t, double a, double b);
		static double Grad(int hash, double x, double y, double z);

	public:
		static void seed(int seed);
		static double noise(double x, double y, double z);
		static double octaveNoise(double x, double y, int octaves);
	};
}