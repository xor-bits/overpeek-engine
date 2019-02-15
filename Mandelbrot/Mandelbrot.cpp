#include "pch.h"
#include <iomanip>

#define TEXEL_WIDTH	600
#define TEXEL_COUNT	TEXEL_WIDTH * TEXEL_WIDTH * 3
GLubyte texels[TEXEL_COUNT];
unsigned int texture;


void render(); void update();
oe::Window *m_window = new oe::Window(600, 600, "Mandelbrot", false, 0, false);
oe::Shader *m_shader = new oe::Shader("shaders/texture.vert.glsl", "shaders/texture.frag.glsl");
oe::Renderer *m_renderer = new oe::Renderer("resources/arial.ttf", m_window);
oe::GameLoop *m_loop = new oe::GameLoop(render, update, nullptr, 60, 1);

int mandelbrot(long double x, long double y, int maximum) {
	int iteration = 0;

	long double zx = x;
	long double zy = y;
	long double xtemp;

	while (zx*zx + zy * zy <= long double(4.0) && iteration <= maximum) {
		//JULIA SET
		//xtemp = zx*zx - zy*zy;
		//zy = 2*zx*zy + COMPLEX_IMAGINARY; 
		//zx = xtemp - COMPLEX_REAL;
		//JULIA SET

		//MANDELBROT SET
		xtemp = zx * zx - zy * zy;
		zy = 2 * zx*zy + y;
		zx = xtemp + x;
		//MANDELBROT SET

		if (zx == y && zy == y) {
			return -1;
		}

		if (zx == 0 || zy == 0) {
			return -1;
		}

		if (zx == -1 || zy == -1) {
			return -1;
		}

		iteration += 1;
	}

	if (iteration >= maximum) {
		return -1;
	}
	else {
		return iteration;
	}
}

glm::vec3 getColor2(int n, float maximum) {
	if (n > 0) {
		int i = n % 16;
		float val = (sin((n / 16.0 * glm::pi<float>())) + 1.0) / 2.0;
		//float val2 = cos((n / 16.0 * M_PI) + M_PI / 2);
		return glm::vec3(val, 0.0, val);
	}
	else return glm::vec3(0);
}

void render() {
	m_renderer->renderBox(glm::vec2(-1.0f), glm::vec2(2.0f), 0, glm::vec4(1.0f));
	m_renderer->draw(m_shader, m_shader, "unif_text", texture);

	m_window->update();
	m_window->input();
	m_window->clear();
}

void update() {
}

int main()
{
	long double scale = long double(9.08663e-16);
	for (int x = 0; x < TEXEL_WIDTH; x++)
	{
		long double x_t = long double(long double(x) / long double(TEXEL_WIDTH) * long double(4.0) - long double(2.0)) * long double(scale) - long double(1.68732);
		std::cout << std::setprecision(128) << long double(long double(x) / long double(TEXEL_WIDTH) * long double(4.0) - long double(2.0) - (long double(1.68732) / long double(scale))) * long double(scale) << std::endl;
		for (int y = 0; y < TEXEL_WIDTH; y++)
		{
			/*	
			Zoom: 9.08663e-16
			Offset real: -1.68732
			Offset imaginary: 0
			Iterations: 138
			*/


			//boost::multiprecision::mpfr_float x_t = float(x) / float(TEXEL_WIDTH) * 4.0f - 3.0f;
			//boost::multiprecision::mpfr_float x_t = float(x) / float(TEXEL_WIDTH) * 4.0f - 3.0f;

			long double y_t = (long double(y) / long double(TEXEL_WIDTH) * long double(4.0) - long double(2.0)) * scale - long double(0.00000);

			int iter = mandelbrot(x_t, y_t, 128);
			glm::vec3 c = getColor2(iter, 128);

			texels[(x + y * TEXEL_WIDTH) * 3 + 0] = c.x * 255;
			texels[(x + y * TEXEL_WIDTH) * 3 + 1] = c.y * 255;
			texels[(x + y * TEXEL_WIDTH) * 3 + 2] = c.z * 255;
		}
	}


	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGB8, TEXEL_WIDTH, TEXEL_WIDTH, 1);
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, TEXEL_WIDTH, TEXEL_WIDTH, 1, GL_RGB, GL_UNSIGNED_BYTE, texels);


	m_shader->enable();
	glm::mat4 orhto = glm::ortho(-1.0, 1.0, 1.0, -1.0);
	m_shader->SetUniformMat4("pr_matrix", orhto);
	m_loop->start();
}