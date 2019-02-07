#include "pch.h"
#include "particle.h"

#define PARTICLE_COUNT	512
#define PARTICLE_SPREAD	400

graphics::Window *m_window;
graphics::Renderer *m_renderer;
graphics::Shader *m_shader;

logic::GameLoop *m_loop;

Particle *m_particles[PARTICLE_COUNT];

bool m_paused = true;
bool m_strong = true;
bool m_electro = true;
bool m_grav = false;

double zoom = 1.0;
double xoff = 0.0;
double yoff = 0.0;

unsigned int speed = 1;


void step() {
	for (int i = 0; i < speed; i++)
	{
		#pragma omp parallel for
		for (int i = 0; i < PARTICLE_COUNT; i++)
		{
			m_particles[i]->update();
		}
		#pragma omp parallel for
		for (int i = 0; i < PARTICLE_COUNT; i++)
		{
			m_particles[i]->preupdate(m_particles, PARTICLE_COUNT, m_strong, m_electro, m_grav);
		}
	}
}

void render() {
	m_window->clear();

	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		m_particles[i]->render(m_renderer, zoom, xoff, yoff);
	}

	m_renderer->renderText(glm::vec2(-100.0, -100.0), glm::vec2(10, 10), logic::boolToString(m_strong ), glm::vec4(1.0, 1.0, 1.0, 1.0), TEXT_ORIGIN_LEFT, TEXT_ORIGIN_TOP);
	m_renderer->renderText(glm::vec2(-100.0, -090.0), glm::vec2(10, 10), logic::boolToString(m_electro), glm::vec4(1.0, 1.0, 1.0, 1.0), TEXT_ORIGIN_LEFT, TEXT_ORIGIN_TOP);
	m_renderer->renderText(glm::vec2(-100.0, -080.0), glm::vec2(10, 10), logic::boolToString(m_grav   ), glm::vec4(1.0, 1.0, 1.0, 1.0), TEXT_ORIGIN_LEFT, TEXT_ORIGIN_TOP);
	m_renderer->renderText(glm::vec2(-100.0, -070.0), glm::vec2(10, 10), std::to_string(zoom), glm::vec4(1.0, 1.0, 1.0, 1.0), TEXT_ORIGIN_LEFT, TEXT_ORIGIN_TOP);
	m_renderer->renderText(glm::vec2(-100.0, -060.0), glm::vec2(10, 10), std::to_string(speed), glm::vec4(1.0, 1.0, 1.0, 1.0), TEXT_ORIGIN_LEFT, TEXT_ORIGIN_TOP);
	m_renderer->renderText(glm::vec2(-100.0, -050.0), glm::vec2(10, 10), std::to_string(m_loop->getUMS()), glm::vec4(1.0, 1.0, 1.0, 1.0), TEXT_ORIGIN_LEFT, TEXT_ORIGIN_TOP);

	m_renderer->draw(m_shader, "unif_text", graphics::TextureManager::getTexture(0));

	m_window->update();
	m_window->input();
}

void update() {
	if (m_window->getKey(GLFW_KEY_D)) xoff -= 3.0 / zoom;
	if (m_window->getKey(GLFW_KEY_A)) xoff += 3.0 / zoom;
	if (m_window->getKey(GLFW_KEY_S)) yoff -= 3.0 / zoom;
	if (m_window->getKey(GLFW_KEY_W)) yoff += 3.0 / zoom;
	if (!m_paused) step();
}

void rapid() {
}

void setup() {
	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		m_particles[i] = new Particle(tools::Random::random(-PARTICLE_SPREAD, PARTICLE_SPREAD), tools::Random::random(-PARTICLE_SPREAD, PARTICLE_SPREAD));
	}
}

void keyPress(int key, int action) {
	if (action == GLFW_PRESS) {
		int n = int(tools::Random::random(0.0, PARTICLE_COUNT));
		float dir = 0.0;
		if (m_window->getKey(GLFW_KEY_LEFT_CONTROL) || m_window->getKey(GLFW_KEY_RIGHT_CONTROL)) dir = -1.0;
		if (m_window->getKey(GLFW_KEY_LEFT_SHIFT) || m_window->getKey(GLFW_KEY_RIGHT_SHIFT)) dir = 1.0;

		switch (key)
		{
		case GLFW_KEY_SPACE:
			m_paused = !m_paused;
			break;
			
		case GLFW_KEY_ENTER:
			step();
			break;

		case GLFW_KEY_R:
			for (int i = 0; i < PARTICLE_COUNT; i++) delete m_particles[i];
			setup();
			break;

		case GLFW_KEY_1:
			m_strong = !m_strong;
			break;

		case GLFW_KEY_2:
			m_electro = !m_electro;
			break;

		case GLFW_KEY_3:
			m_grav = !m_grav;
			break;

		case GLFW_KEY_UP:
			speed *= 2;

			if (speed == 0) speed = 1;
			break;

		case GLFW_KEY_DOWN:
			speed /= 2;
			break;

		case GLFW_KEY_F:
			tools::Logger::info(m_loop->getFPS());
			break;

		case GLFW_KEY_C:
			for (int i = 0; i < PARTICLE_COUNT; i++)
			{
				tools::Logger::info(m_particles[i]->getKineticEnergy());
			}
			break;

		case GLFW_KEY_P:
			m_particles[n] = new Particle(-xoff, -yoff, dir, 0.0, 1.0);
			break;

		case GLFW_KEY_E:
			m_particles[n] = new Particle(-xoff, -yoff, dir, 0.0, -1.0);
			break;

		case GLFW_KEY_N:
			m_particles[n] = new Particle(-xoff, -yoff, dir, 0.0, 0.0);
			break;

		case GLFW_KEY_M:
			m_particles[n] = new Particle(-xoff, -yoff, dir, 0.0, -10000.0, 100000000);
			break;

		default:
			break;
		}
	}
}

void buttonPress(int key, int action) {
	if (action == GLFW_PRESS) {
		switch (key)
		{
		default:
			break;
		}
	}
}

void scroll(double x) {
	if (x > 0) zoom *= 2;
	if (x < 0) zoom *= 0.5;
	//tools::Logger::info(zoom);
}

int main() {
	m_window = new graphics::Window(720, 720, "Chaos", false, 0);
	m_window->setKeyboardCallback(keyPress);
	m_window->setButtonCallback(buttonPress);
	m_window->setScrollCallback(scroll);
	m_renderer = new graphics::Renderer("recourses/arial.ttf", m_window);
	m_shader = new graphics::Shader("shaders/texture.vert.glsl", "shaders/texture.frag.glsl");
	glm::mat4 pr = glm::ortho(-100.0, 100.0, 100.0, -100.0);
	m_shader->enable(); m_shader->SetUniformMat4("pr_matrix", pr);

	graphics::TextureManager::loadTextureAtlas("recourses/atlas.png", GL_RGBA, 0);

	setup();

	m_loop = new logic::GameLoop(render, update, rapid, 60, 60);
	m_loop->start();
	return 0;
}