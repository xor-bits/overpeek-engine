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
glm::vec3 MainMenu::m_objpos[1000];
glm::vec3 MainMenu::m_objvel[1000];
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
		m_objpos[i].z = oe::Random::random(-1.0, 1.0);

		m_objvel[i].x = oe::Random::random(-0.2, 0.2);
		m_objvel[i].y = oe::Random::random(-0.2, 0.2);
		m_objvel[i].z = oe::Random::random(-0.2, 0.2);

		float r = oe::Random::random(0.0, 1.0);
		float g = oe::Random::random(0.0, 1.0);
		float b = oe::Random::random(0.0, 1.0);
		m_objcol[i] = glm::vec4(r, g, b, 1.0f);
	}

	m_loop->start();
}

void MainMenu::deinit() {
	delete m_loop;
	delete m_renderer;
	delete m_shader;
}

float anglex = 0.0f;
float angley = 0.0f;
void MainMenu::render(float corrector) {
	double mx, my;
	Game::getWindow()->getMousePos(mx, my);
	anglex = -mx * 4;
	angley = -my * 4;

	glm::mat4 projection = glm::perspectiveFov(90.0f, Game::getWindow()->getAspect(), 1.0f, 0.01f, 1000.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(cos(anglex), angley, sin(anglex)), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	glm::mat4 mvp = projection * view;

	m_shader->enable(); m_shader->SetUniformMat4("pr_matrix", mvp);
	m_point_shader->enable(); m_point_shader->SetUniformMat4("pr_matrix", projection); m_point_shader->SetUniformMat4("vw_matrix", view);
	

	for (int i = 0; i < 1000; i++)
	{
		glm::vec3 pos(m_objpos[i] + m_objvel[i] * corrector / 30.0f);
		glm::vec2 size = glm::vec2(0.02);

		m_renderer->pointRenderer->submitVertex(oe::VertexData(pos, size, 20, m_objcol[i]));
		//m_renderer->lineRenderer->submitVertex(oe::VertexData(pos, size, 20, m_objcol[i]));
		//m_renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y, pos.z), glm::vec2(0.0f, 0.0f), 20, m_objcol[i]));
		//m_renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y + size.y, pos.z), glm::vec2(0.0f, 1.0f), 20, m_objcol[i]));
		//m_renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x + size.x, pos.y + size.y, pos.z), glm::vec2(1.0f, 1.0f), 20, m_objcol[i]));
		//m_renderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x + size.x, pos.y, pos.z), glm::vec2(1.0f, 0.0f), 20, m_objcol[i]));
	}
	m_renderer->fontRenderer->renderText(glm::vec3(2.0, -1.0 + 0.2 * Game::renderScale(), 0.0f), glm::vec2(2), "$7MAIN MENU", oe::center);

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
		m_objpos[i].z += m_objvel[i].z / 30.0f;

		if (m_objpos[i].x > 1.0) m_objvel[i].x *= -1;
		if (m_objpos[i].x < -1.0) m_objvel[i].x *= -1;
		if (m_objpos[i].y > 1.0) m_objvel[i].y *= -1;
		if (m_objpos[i].y < -1.0) m_objvel[i].y *= -1;
		if (m_objpos[i].z > 1.0) m_objvel[i].z *= -1;
		if (m_objpos[i].z < -1.0) m_objvel[i].z *= -1;
	}
}

void MainMenu::key(int key, int action) {
	m_loop->stop();
}

void MainMenu::button(int key, int action) {

}