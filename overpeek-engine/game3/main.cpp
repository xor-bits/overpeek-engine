#include "../engine.h"

#include <omp.h>


int mandelbrot(double x, double y, int maximum) {
	int iteration = 0; double zx = x; double zy = y;
	while (zx*zx + zy * zy <= 4.0 && iteration <= maximum) {
		double ytemp = 2 * zx*zy + y;
		zx = zx * zx - zy * zy + x;
		zy = ytemp;
		if (zx == y && zy == y) return -1;
		iteration += 1;
	}

	if (iteration >= maximum) return -1;
	else return iteration;
}

glm::vec3 getColorLine0(int n, float maximum) {
	if (n > 0) {
		float val = (sin(n / 20.0) + 1) / 2.0;
		return glm::vec3(0, val * 255, 0);
	}
	else return glm::vec3(0, 0, 0);
}

glm::vec3 getColorLine1(int n, float maximum) {
	if (n > 0) {
		float val = (sin(n / 20.0) + 1) / 2.0;
		return glm::vec3(0, 0, val * 255);
	}
	else return glm::vec3(0, 0, 0);
}

glm::vec3 getColorLine2(int n, float maximum) {
	if (n > 0) {
		float val = (sin(n / 20.0) + 1) / 2.0;
		return glm::vec3(val * 255, 0, 0);
	}
	else return glm::vec3(0, 0, 0);
}

glm::vec3 getColorLine3(int n, float maximum) {
	if (n > 0) {
		float val = (sin(n / 20.0) + 1) / 2.0;
		return glm::vec3(val * 255, val * 255, val * 255);
	}
	else return glm::vec3(0, 0, 0);
}

glm::vec3 getColorLine4(int n, float maximum) {
	if (n > 0) {
		float val = (sin(n / 20.0) + 1) / 2.0;
		float val2 = (cos(n / 20.0) + 1) / 2.0;
		return glm::vec3(0, val * 255, val2 * 255);
	}
	else return glm::vec3(0, 0, 0);
}

glm::vec3 getColorFromAlgorithm(int n, float max, unsigned int id) {
	switch (id)
	{
	case 0:
		return getColorLine0(n, max);
		break;
	case 1:
		return getColorLine1(n, max);
		break;
	case 2:
		return getColorLine2(n, max);
		break;
	case 3:
		return getColorLine3(n, max);
		break;
	case 4:
		return getColorLine4(n, max);
		break;
	default:
		break;
	}
	return glm::vec3(0.0, 0.0, 0.0);
}

int main() {
	tools::Logger::setup();
	double zoom = 1;
	double xOff = 0;
	double yOff = 0;
	int height = 1080, width = 1920, iterations = pow(2, 7);
	unsigned int colorAlg = 0;
	bool preview = false;

	std::cout << "Enter zoom (between 0.0-2.0 but not 0.0, default 1.0): ";
	std::cin >> zoom;

	std::cout << "Enter real number offset (default 0.0): ";
	std::cin >> xOff;

	std::cout << "Enter imaginary number offset (default 0.0): ";
	std::cin >> yOff;

	std::cout << "Enter image height (default 1080): ";
	std::cin >> height;

	std::cout << "Enter image width (default 1920): ";
	std::cin >> width;

	std::cout << "Enter iterations (default 128): ";
	std::cin >> iterations;

	std::cout << "Enter color algorithm id (between 0-4, default 0): ";
	std::cin >> colorAlg;

	tools::Logger::info(std::string("Resolution: ") +
		std::to_string(width) + std::string("x") +
		std::to_string(height));
	tools::Logger::info(std::string("Iterations: ") +
		std::to_string(iterations));
	tools::Logger::info(std::string("Zoom: ") + 
		std::to_string(zoom) + 
		std::string(", Real offset: ") +
		std::to_string(xOff) + 
		std::string(", Imaginary offset: ") + 
		std::to_string(yOff));
	tools::Logger::info(std::string("Color algorithm: ") +
		std::to_string(colorAlg));
	//tools::Logger::info(std::string("Preview: ") +
	//	std::to_string(preview));

	std::vector<GLubyte> data;
	double aspect = (float)width / (float)height;

	long long microS = tools::Clock::getMicroseconds();
	for (int i = 0; i < height * width * 3; i++) data.push_back(0);
	tools::Logger::info(std::string("Time took to allocate memory: ") + std::to_string((tools::Clock::getMicroseconds() - microS) / 1000.0) + std::string("ms"));

	microS = tools::Clock::getMicroseconds();
#pragma omp parallel
	for (int x = 0; x < width; x++)
	{
#pragma omp for
		for (int y = 0; y < height; y++)
		{
			double real = (float)x / (float)width * 4 - 2;
			double imag = (float)y / (float)height * 4 - 2;
			real *= zoom;
			imag *= zoom / aspect;
			real += xOff;
			imag += yOff;
			glm::vec3 color = getColorFromAlgorithm(mandelbrot(real, imag, iterations), iterations, colorAlg);

			data[(x + y * width) * 3 + 0] = color.x;
			data[(x + y * width) * 3 + 1] = color.y;
			data[(x + y * width) * 3 + 2] = color.z;
		}
	}
	tools::Logger::info(std::string("Time took to render mandelbrot: ") + std::to_string((tools::Clock::getMicroseconds() - microS) / 1000.0) + std::string("ms"));
	graphics::TextureManager::saveTexture("recourses/atlas.png", &data[0], width, height, preview);
	
	system("pause");

	main();
}