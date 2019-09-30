#include "pch.h"
#include <iomanip>

#define JULIA_SET
#define TEXEL_WIDTH	100
#define TEXEL_COUNT	TEXEL_WIDTH * TEXEL_WIDTH * 3
GLubyte texels[TEXEL_COUNT];
unsigned int texture;

unsigned int iteration_count = 64;
NTL::RR scale = NTL::RR(1.0);//9.08663e-16);
NTL::RR xo = NTL::RR(0.0);//1.6873200000000003);
NTL::RR yo = NTL::RR(0.0);

void render(float corrector); void update();
oe::Window *m_window = new oe::Window(600, 600, "Mandelbrot", 0);
oe::Shader *m_shader = new oe::Shader("shaders/texture.vert.glsl", "shaders/texture.frag.glsl");
oe::Renderer *m_renderer = new oe::Renderer("resources/arial.ttf", m_window->getWidth(), m_window->getHeight());
oe::GameLoop *m_loop = new oe::GameLoop(render, update, 5, m_window);




int mandelbrot(NTL::RR &x, NTL::RR &y, int maximum) {
	int iteration = 0;

	NTL::RR zx = x;
	NTL::RR zy = y;
	NTL::RR xtemp;

	while (zx*zx + zy * zy <= NTL::RR(4.0) && iteration <= maximum) {
#ifdef JULIA_SET
		xtemp = zx * zx - zy * zy;
		zy = 2 * zx*zy + 0.156;
		zx = xtemp - 0.8;
#else
		xtemp = zx * zx - zy * zy;
		zy = 2 * zx*zy + y;
		zx = xtemp + x;
#endif // JULIA_SET

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
		//float val = (sin((n / 16.0 * glm::pi<float>())) + 1.0) / 2.0;
		//float val2 = cos((n / 16.0 * M_PI) + M_PI / 2);

		float r = oe::Random::hash(n / (double)maximum);
		float g = 0.0;
		float b = 0.0;

		return glm::vec3(r, g, b);
	}
	else return glm::vec3(0);
}

void draw() {
	int count = 0;
#pragma omp parallel for
	for (int x = 0; x < TEXEL_WIDTH; x++)
	{
		NTL::RR x_t;
		x_t = x;
		x_t /= TEXEL_WIDTH;
		x_t *= 4.0;
		x_t -= 2.0;
		x_t *= scale;
		x_t -= xo;
		for (int y = 0; y < TEXEL_WIDTH; y++)
		{
			/*
			Zoom: 9.08663e-16
			Offset real: -1.68732
			Offset imaginary: 0
			Iterations: 138
			*/

			NTL::RR y_t;
			y_t = y;
			y_t /= TEXEL_WIDTH;
			y_t *= 4.0;
			y_t -= 2.0;
			y_t *= scale;
			y_t -= yo;

			//boost::multiprecision::mpfr_float x_t = float(x) / float(TEXEL_WIDTH) * 4.0f - 3.0f;
			//boost::multiprecision::mpfr_float x_t = float(x) / float(TEXEL_WIDTH) * 4.0f - 3.0f;

			//long double y_t = (long double(y) / long double(TEXEL_WIDTH) * long double(4.0) - long double(2.0)) * scale - long double(0.00000);
			//
			int iter = mandelbrot(x_t, y_t, iteration_count);

			glm::vec3 c = getColor2(iter, iteration_count);

			texels[(x + y * TEXEL_WIDTH) * 3 + 0] = c.x * 255;
			texels[(x + y * TEXEL_WIDTH) * 3 + 1] = c.y * 255;
			texels[(x + y * TEXEL_WIDTH) * 3 + 2] = c.z * 255;
		}
	}

	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, TEXEL_WIDTH, TEXEL_WIDTH, 1, GL_RGB, GL_UNSIGNED_BYTE, texels);
}

void render(float corrector) {
	if (m_window->close()) m_loop->stop();


	//oe::Logger::info("Draw");
	draw();
	m_renderer->quadRenderer->submitQuad(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(2.0f), 0, glm::vec4(1.0f));
	m_renderer->draw(m_shader, m_shader, texture, true);

	//m_window->update();
	//m_window->input();
	//m_window->clear();
}

void update() {
	if (m_window->getKey(OE_KEY_A)) xo += scale;
	if (m_window->getKey(OE_KEY_D)) xo -= scale;
	if (m_window->getKey(OE_KEY_W)) yo += scale;
	if (m_window->getKey(OE_KEY_S)) yo -= scale;

	if (m_window->getKey(OE_KEY_E)) scale *= 0.75;
	if (m_window->getKey(OE_KEY_Q)) scale *= 1.5;

	if (m_window->getKey(OE_KEY_R)) iteration_count += 2;
	if (m_window->getKey(OE_KEY_F)) iteration_count -= 2;
	//oe::Logger::info("Input");
}

int main()
{
	m_window->setSwapInterval(72);
	NTL::RR::SetPrecision(2048);
	NTL::RR::SetOutputPrecision(2048);


	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGB8, TEXEL_WIDTH, TEXEL_WIDTH, 1);

	update();


	m_shader->enable();
	glm::mat4 orhto = glm::ortho(-1.0, 1.0, 1.0, -1.0);
	m_shader->SetUniformMat4("pr_matrix", orhto);
	m_loop->start();
}