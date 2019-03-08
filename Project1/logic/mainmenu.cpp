#include "../pch.h"
#include "mainmenu.h"
#include "game.h"


int MainMenu::m_active_button;
oe::GameLoop *MainMenu::m_loop;
oe::Renderer *MainMenu::m_renderer;
oe::Shader *MainMenu::m_shader;
oe::Shader *MainMenu::m_point_shader;
oe::Shader *MainMenu::m_post_shader;
float MainMenu::m_t;
glm::vec2 MainMenu::m_objpos[1000];
glm::vec2 MainMenu::m_objvel[1000];
glm::vec4 MainMenu::m_objcol[1000];


void MainMenu::init(oe::Renderer *renderer, oe::Shader *shader, oe::Shader *pointshader, oe::Shader *postshader) {
	m_loop = new oe::GameLoop(render, update, 30, Game::getWindow());
	m_renderer = renderer;
	m_shader = shader;
	m_point_shader = pointshader;
	m_post_shader = postshader;

	for (int i = 0; i < 1000; i++)
	{
		m_objpos[i].x = oe::Random::random(-1.0, 1.0);
		m_objpos[i].y = oe::Random::random(-1.0, 1.0);

		m_objvel[i].x = oe::Random::random(-0.2, 0.2);
		m_objvel[i].y = oe::Random::random(-0.2, 0.2);

		float r = oe::Random::random(0.0, 1.0);
		float g = oe::Random::random(0.0, 1.0);
		float b = oe::Random::random(0.0, 1.0);
		m_objcol[i] = glm::vec4(r, g, b, 1.0);
	}

	m_loop->start();
}

void MainMenu::deinit() {
	delete m_loop;
	delete m_renderer;
	delete m_shader;
}

void MainMenu::render(float corrector) {
	for (int i = 0; i < 1000; i++)
	{
		glm::vec2 pos(m_objpos[i] + m_objvel[i] * corrector / 30.0f);
		m_renderer->renderPoint(pos, glm::vec2(0.02), 20, m_objcol[i]);
	}
	m_renderer->renderText(glm::vec2(0.0, -1.0 + 0.2 * Game::renderScale()), glm::vec2(0.2), "$7MAIN MENU", oe::center);

	m_renderer->drawToFramebuffer(m_shader, m_point_shader, oe::TextureManager::getTexture(0), true, true);
	//for (int i = 0; i < 8; i++) {
	//	m_post_shader->setUniform1i("unif_effect", 1);
	//	m_renderer->drawFramebufferToFramebuffer(m_post_shader, "unif_texture", false);
	//	m_post_shader->setUniform1i("unif_effect", 2);
	//	m_renderer->drawFramebufferToFramebuffer(m_post_shader, "unif_texture", true);
	//}
	m_renderer->drawFramebuffer(m_post_shader, "unif_texture", true);
}

void MainMenu::update() {
	for (int i = 0; i < 1000; i++)
	{
		m_objpos[i].x += m_objvel[i].x / 30.0f;
		m_objpos[i].y += m_objvel[i].y / 30.0f;

		if (m_objpos[i].x > 1.0) m_objvel[i].x *= -1;
		if (m_objpos[i].x < -1.0) m_objvel[i].x *= -1;
		if (m_objpos[i].y > 1.0) m_objvel[i].y *= -1;
		if (m_objpos[i].y < -1.0) m_objvel[i].y *= -1;
	}
}

void MainMenu::key(int key, int action) {
	m_loop->stop();
}

void MainMenu::button(int key, int action) {

}