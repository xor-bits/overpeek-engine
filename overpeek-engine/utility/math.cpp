#include "math.h"

namespace oe {

	void rotatePoint(glm::vec2 center, float xyangle, glm::vec2 &p) {
		float s = sin(xyangle);
		float c = cos(xyangle);

		// translate point back to origin:
		p.x -= center.x;
		p.y -= center.y;

		// rotate point
		float xnew = p.x * c - p.y * s;
		float ynew = p.x * s + p.y * c;

		// translate point back:
		p.x = xnew + center.x;
		p.y = ynew + center.y;
	}

	std::string boolToString(bool b)
	{
		return b ? "true" : "false";
	}

	bool isInRange(int input, int min, int max) {
		return (input >= min) && (input <= max);
	}

	int sign (float n) {
		if (n > 0) return 1;
		return -1;
	}

	glm::vec4 rgb2hsv(glm::vec4 in)
	{
		glm::vec4   out;
		out.w = in.w;
		double      min, max, delta;

		min = in.r < in.g ? in.r : in.g;
		min = min < in.b ? min : in.b;

		max = in.r > in.g ? in.r : in.g;
		max = max > in.b ? max : in.b;

		out.z = max;                                // v
		delta = max - min;
		if (delta < 0.00001)
		{
			out.y = 0;
			out.x = 0; // undefined, maybe nan?
			return out;
		}
		if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
			out.y = (delta / max);                  // s
		}
		else {
			// if max is 0, then r = g = b = 0              
			// s = 0, h is undefined
			out.s = 0.0;
			out.x = NAN;                            // its now undefined
			return out;
		}
		if (in.r >= max)                           // > is bogus, just keeps compilor happy
			out.x = (in.g - in.b) / delta;        // between yellow & magenta
		else
			if (in.g >= max)
				out.x = 2.0 + (in.b - in.r) / delta;  // between cyan & yellow
			else
				out.x = 4.0 + (in.r - in.g) / delta;  // between magenta & cyan

		out.x *= 60.0;                              // degrees

		if (out.x < 0.0)
			out.x += 360.0;

		return out;
	}

	glm::vec4 hsv2rgb(glm::vec4 in)
	{
		double      hh, p, q, t, ff;
		long        i;
		glm::vec4   out;
		out.w = in.w;

		if (in.y <= 0.0) {       // < is bogus, just shuts up warnings
			out.r = in.z;
			out.g = in.z;
			out.b = in.z;
			return out;
		}
		hh = in.x;
		if (hh >= 360.0) hh = 0.0;
		hh /= 60.0;
		i = (long)hh;
		ff = hh - i;
		p = in.y * (1.0 - in.y);
		q = in.y * (1.0 - (in.y * ff));
		t = in.y * (1.0 - (in.y * (1.0 - ff)));

		switch (i) {
		case 0:
			out.r = in.z;
			out.g = t;
			out.b = p;
			break;
		case 1:
			out.r = q;
			out.g = in.z;
			out.b = p;
			break;
		case 2:
			out.r = p;
			out.g = in.z;
			out.b = t;
			break;

		case 3:
			out.r = p;
			out.g = q;
			out.b = in.z;
			break;
		case 4:
			out.r = t;
			out.g = p;
			out.b = in.z;
			break;
		case 5:
		default:
			out.r = in.z;
			out.g = p;
			out.b = q;
			break;
		}
		return out;
	}

	void map(double& val, double a_low, double a_high, double b_low, double b_high) {
		val = (a_low * b_high - a_high * b_low + val * (b_low - b_high)) / (a_low - a_high);
	}

}