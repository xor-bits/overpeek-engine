#include "noise.h"

namespace oe {
	int Noise::p[512];
	double Noise::Fade(double t) {
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	double Noise::Lerp(double t, double a, double b) {
		return a + t * (b - a);
	}

	double Noise::Grad(int hash, double x, double y, double z) {
		int h = hash & 15;
		double u = h < 8 ? x : y;
		double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
		return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
	}

	void Noise::seed(int seed)
	{
		for (size_t i = 0; i < 256; ++i) p[i] = i;
		std::shuffle(std::begin(p), std::begin(p) + 256, std::default_random_engine(seed));
		for (size_t i = 0; i < 256; ++i) p[256 + i] = p[i];
	}

	double Noise::noise(double x, double y, double z)
	{
		int X = int(std::floor(x)) & 255;
		int Y = int(std::floor(y)) & 255;
		int Z = int(std::floor(z)) & 255;

		x -= std::floor(x);
		y -= std::floor(y);
		z -= std::floor(z);

		double u = Fade(x);
		double v = Fade(y);
		double w = Fade(z);

		int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
		int B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

		return Lerp(w,
			Lerp(v,
				Lerp(u, Grad(p[AA], x, y, z), Grad(p[BA], x - 1, y, z)), 
				Lerp(u, Grad(p[AB], x, y - 1, z), Grad(p[BB], x - 1, y - 1, z))),
			Lerp(v,
				Lerp(u, Grad(p[AA + 1], x, y, z - 1), Grad(p[BA + 1], x - 1, y, z - 1)), 
				Lerp(u, Grad(p[AB + 1], x, y - 1, z - 1), Grad(p[BB + 1], x - 1, y - 1, z - 1)))
		);
	}

	double Noise::octaveNoise(double x, double y, int octaves)
	{
		double result = 0.0;
		double amp = 1.0;

		for (int i = 0; i < octaves; ++i) {
			result += noise(x, y, 0.0) * amp;
			x *= 2.0;
			y *= 2.0;
			amp *= 0.5;
		}

		return result;
	}

}