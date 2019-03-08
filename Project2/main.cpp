#include "pch.h"
#include "particle.h"

#define PARTICLE_COUNT	512
#define PARTICLE_SPREAD	400

oe::Window *m_window;
oe::Renderer *m_renderer;
oe::Shader *m_shader;

oe::GameLoop *m_loop;

Particle *m_particles[PARTICLE_COUNT];

bool m_paused = true;
bool m_strong = true;
bool m_electro = true;
bool m_grav = false;

double force_mult = 1;
double zoom = 1.0;
double xoff = 0.0;
double yoff = 0.0;
double xoff_drag = 0.0;
double yoff_drag = 0.0;

unsigned int speed = 1;


double before_mouse_x = 0;
double before_mouse_y = 0;

double last_mouse_x = 0;
double last_mouse_y = 0;
glm::vec2 last_vector_dir = glm::vec2(0.0);
Particle *selector;


void step() {
	for (int i = 0; i < speed; i++)
	{
		#pragma omp parallel for
		for (int i = 0; i < PARTICLE_COUNT; i++)
		{
			if (m_particles[i]) m_particles[i]->update();
		}
		#pragma omp parallel for
		for (int i = 0; i < PARTICLE_COUNT; i++)
		{
			if (m_particles[i]) m_particles[i]->preupdate(m_particles, PARTICLE_COUNT, force_mult, m_strong, m_electro, m_grav);
		}
	}
}

void render() {
	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		if (m_particles[i]) m_particles[i]->render(m_renderer, zoom, xoff + xoff_drag, yoff + yoff_drag);
	}

	m_renderer->renderText(glm::vec2(-100.0, -100.0), glm::vec2(6), ("Strong force: " + oe::boolToString(m_strong )).c_str(), oe::topLeft);
	m_renderer->renderText(glm::vec2(-100.0, -094.0), glm::vec2(6), ("Electromagnetic force: " + oe::boolToString(m_electro)).c_str(), oe::topLeft);
	m_renderer->renderText(glm::vec2(-100.0, -088.0), glm::vec2(6), ("Gravitational force: " + oe::boolToString(m_grav   )).c_str(), oe::topLeft);
	m_renderer->renderText(glm::vec2(-100.0, -082.0), glm::vec2(6), ("Zoom: " + std::to_string(zoom)).c_str(), oe::topLeft);
	m_renderer->renderText(glm::vec2(-100.0, -076.0), glm::vec2(6), ("Sim speed: " + std::to_string(speed)).c_str(), oe::topLeft);
	m_renderer->renderText(glm::vec2(-100.0, -070.0), glm::vec2(6), ("Tick time: " + std::to_string(m_loop->getUMS())).c_str(), oe::topLeft);
	m_renderer->renderText(glm::vec2(-100.0, -064.0), glm::vec2(6), ("Forces: x" + std::to_string(force_mult)).c_str(), oe::topLeft);

	m_renderer->draw(m_shader, m_shader, oe::TextureManager::getTexture(0), true);


	//Selector

	double mouse_x, mouse_y;
	m_window->getMousePos(mouse_x, mouse_y);
	glm::vec2 mouse = glm::vec2(mouse_x * 100, mouse_y * 100);
	glm::vec2 arrow = glm::normalize(last_vector_dir) * glm::vec2(20.0);

	m_renderer->renderLine(mouse, mouse + arrow, 24, glm::vec4(1.0, 0.5, 0.0, 1.0));
	m_renderer->renderText(glm::vec2(mouse.x, mouse.y - 5.0), glm::vec2(5, 5), (std::to_string(last_vector_dir.x) + ", " + std::to_string(last_vector_dir.y)).c_str(), oe::center);

	selector->render(m_renderer, zoom, xoff + xoff_drag, yoff + yoff_drag);

	if (selector->proton()) m_renderer->renderBox(glm::vec2(90), glm::vec2(10), 24, glm::vec4(1.0, 0.0, 0.0, 1.0));
	else if (selector->neutron()) m_renderer->renderBox(glm::vec2(90), glm::vec2(10), 24, glm::vec4(0.7, 0.7, 0.7, 1.0));
	else m_renderer->renderBox(glm::vec2(90), glm::vec2(10), 24, glm::vec4(0.0, 0.0, 1.0, 1.0));
}

void update() {
	if (m_window->close()) m_loop->stop();

	double mouse_x, mouse_y;
	m_window->getMousePos(mouse_x, mouse_y);

	if (m_window->getButton(OE_MOUSE_BUTTON_RIGHT)) {
		xoff_drag = (mouse_x - before_mouse_x) * 100.0 / zoom;
		yoff_drag = (mouse_y - before_mouse_y) * 100.0 / zoom;
	}

	selector->setX((mouse_x * 100.0) / zoom - xoff - xoff_drag);
	selector->setY((mouse_y * 100.0) / zoom - yoff - yoff_drag);
	last_vector_dir = Particle::calcForces(selector, m_particles, force_mult, PARTICLE_COUNT, m_strong, m_electro, m_grav);


	if (m_window->getKey(OE_KEY_D)) xoff -= 3.0 / zoom;
	if (m_window->getKey(OE_KEY_A)) xoff += 3.0 / zoom;
	if (m_window->getKey(OE_KEY_S)) yoff -= 3.0 / zoom;
	if (m_window->getKey(OE_KEY_W)) yoff += 3.0 / zoom;
	if (!m_paused) step();
}

void rapid() {
}

void setup() {
	selector = new Particle(0.0, 0.0, 0.0, 0.0, 1, 1.0);
	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		m_particles[i] = nullptr;
	}
}

void keyPress(int key, int action) {
	if (action == OE_PRESS) {
		int n = int(oe::Random::random(0.0, PARTICLE_COUNT));
		float dir = 0.0;
		if (m_window->getKey(OE_KEY_LEFT_CONTROL) || m_window->getKey(OE_KEY_RIGHT_CONTROL)) dir = -0.5;
		if (m_window->getKey(OE_KEY_LEFT_SHIFT) || m_window->getKey(OE_KEY_RIGHT_SHIFT)) dir = 0.5;

		switch (key)
		{
		case OE_KEY_ESCAPE:
			m_loop->stop();
			break;
		case OE_KEY_SPACE:
			m_paused = !m_paused;
			break;
			
		case OE_KEY_ENTER:
			step();
			break;

		case OE_KEY_R:
			for (int i = 0; i < PARTICLE_COUNT; i++) delete m_particles[i];
			setup();
			break;

		case OE_KEY_1:
			delete selector;
			selector = new Particle(-xoff - xoff_drag, -yoff - yoff_drag, 0, 0.0, 1.0);
			break;

		case OE_KEY_2:
			delete selector;
			selector = new Particle(-xoff - xoff_drag, -yoff - yoff_drag, 0, 0.0, 0.0);
			break;

		case OE_KEY_3:
			delete selector;
			selector = new Particle(-xoff - xoff_drag, -yoff - yoff_drag, 0, 0.0, -1.0);
			break;

		case OE_KEY_4:
			delete selector;
			selector = new Particle(-xoff - xoff_drag, -yoff - yoff_drag, 0, -10000.0, 100000000);
			break;

		case OE_KEY_F1:
			m_strong = !m_strong;
			break;

		case OE_KEY_F2:
			m_electro = !m_electro;
			break;

		case OE_KEY_F3:
			m_grav = !m_grav;
			break;

		case OE_KEY_UP:
			speed *= 2;

			if (speed == 0) speed = 1;
			break;

		case OE_KEY_DOWN:
			speed /= 2;
			break;

		case OE_KEY_PAGE_UP:
			force_mult *= 2;

			if (force_mult == 0) force_mult = 1;
			break;

		case OE_KEY_PAGE_DOWN:
			force_mult /= 2;
			break;

		case OE_KEY_LEFT:
			zoom *= 0.5;
			break;
		
		case OE_KEY_RIGHT:
			zoom *= 2;
			break;

		case OE_KEY_F:
			oe::Logger::out(oe::info, m_loop->getFPS());
			break;

		default:
			break;
		}
	}
}

void buttonPress(int button, int action) {
	if (action == OE_PRESS) {
		int n = int(oe::Random::random(0.0, PARTICLE_COUNT));
		float dir = 0.0;
		if (m_window->getKey(OE_KEY_LEFT_CONTROL) || m_window->getKey(OE_KEY_RIGHT_CONTROL)) dir = -0.1;
		if (m_window->getKey(OE_KEY_LEFT_SHIFT) || m_window->getKey(OE_KEY_RIGHT_SHIFT)) dir = 0.1;

		switch (button)
		{
		case OE_MOUSE_BUTTON_LEFT:
			m_particles[n] = new Particle(selector->getX(), selector->getY(), dir, 0.0, selector->getCharge());
			break;
		case OE_MOUSE_BUTTON_RIGHT:
			double mouse_x, mouse_y;
			m_window->getMousePos(mouse_x, mouse_y);

			before_mouse_x = mouse_x;
			before_mouse_y = mouse_y;

			break;

		default:
			break;
		}
	}
	else if (action == OE_RELEASE) {
		oe::Logger::out(oe::info, "Release");
		if (button == OE_MOUSE_BUTTON_RIGHT) {
			xoff += xoff_drag;
			yoff += yoff_drag;

			xoff_drag = 0;
			yoff_drag = 0;
		}
	}

}

void scroll(double x) {
	double mouse_x, mouse_y;
	m_window->getMousePos(mouse_x, mouse_y);

	if (x > 0) {
		xoff = ((zoom * zoom * 2 * xoff + 100 * mouse_x * (zoom - zoom * 2)) / (zoom * zoom * 2));
		yoff = ((zoom * zoom * 2 * yoff + 100 * mouse_y * (zoom - zoom * 2)) / (zoom * zoom * 2));

		zoom *= 2;
	}

	if (x < 0) {
		xoff = ((zoom * zoom * 0.5 * xoff + 100 * mouse_x * (zoom - zoom * 0.5)) / (zoom * zoom * 0.5));
		yoff = ((zoom * zoom * 0.5 * yoff + 100 * mouse_y * (zoom - zoom * 0.5)) / (zoom * zoom * 0.5));

		zoom *= 0.5;
	}

	oe::Logger::out(oe::info, mouse_x);
}

int main() {
	m_window = new oe::Window(720, 720, "Chaos", false, 0, false);
	m_window->setKeyboardCallback(keyPress);
	m_window->setButtonCallback(buttonPress);
	m_window->setScrollCallback(scroll);
	m_window->setSwapInterval(1);
	m_renderer = new oe::Renderer("recourses/arial.ttf", m_window);
	m_shader = new oe::Shader("shaders/texture.vert.glsl", "shaders/texture.frag.glsl");
	glm::mat4 pr = glm::ortho(-100.0, 100.0, 100.0, -100.0);
	m_shader->enable(); m_shader->SetUniformMat4("pr_matrix", pr);

	oe::TextureManager::loadTextureAtlas("recourses/atlas.png", 0, 16);

	setup();

	m_loop = new oe::GameLoop(render, update, 60, 60, m_window);
	m_loop->start();
	return 0;
}