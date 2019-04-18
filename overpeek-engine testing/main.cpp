#include "stdafx.h"


void render(float corrector); void update();
oe::Window *window = new oe::Window(900, 600, "overpeek-engine tester", false, 0, false);
oe::Shader* shader = new oe::Shader("res/shader/texture.vert.glsl", "res/shader/texture.frag.glsl");
oe::Shader* guishader = new oe::Shader("res/shader/texture.vert.glsl", "res/shader/texture.frag.glsl");
oe::Renderer *renderer = new oe::Renderer("res/font/arialbd.ttf", window);
oe::GameLoop *gameloop = new oe::GameLoop(render, update, 5, window);

float xyangle = 0;
float xzangle = 0;
//float box_vel_x = 1;

void render(float corrector) {
	xyangle += 10.0 / gameloop->fastFPS();

	//Inputs
	if (window->getKey(OE_KEY_W)) xzangle -= 5.0 / gameloop->fastFPS();
	if (window->getKey(OE_KEY_S)) xzangle += 5.0 / gameloop->fastFPS();

	//Model view projection matrix updates
	glm::mat4 pers= glm::perspectiveFov(90.0f, 1.0f, 1.0f, 0.01f, 100.0f);
	glm::mat4 view = glm::lookAt(
		glm::vec3(cos(xyangle) * cos(xzangle), sin(xzangle), sin(xyangle) * cos(xzangle)),
		glm::vec3(0.0f, 0.0f, 0.0),
		glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f))
	);
	glm::mat4 mvp = pers * view;
	shader->enable(); shader->SetUniformMat4("pr_matrix", mvp);

	//Text
	std::string fps = "Fps: " + std::to_string(gameloop->getFPS());
	renderer->fontRenderer->renderText(glm::vec3(0.0f, 0.7f, 0.0f), glm::vec2(0.2f), fps.c_str(), oe::center);

	//Triangle
	glm::vec3 tip(0.0f, -0.5f, 0.5f);
	glm::vec3 left(0.5f, 0.5f, 0.5f);
	glm::vec3 right(-0.5f, 0.5f, 0.5f);
	renderer->triangleRenderer->submitVertex(oe::VertexData(tip  , glm::vec2(0.0f, 0.0f), 20, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));
	renderer->triangleRenderer->submitVertex(oe::VertexData(left , glm::vec2(0.0f, 1.0f), 20, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
	renderer->triangleRenderer->submitVertex(oe::VertexData(right, glm::vec2(1.0f, 1.0f), 20, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
	
	renderer->draw(shader, shader, oe::TextureManager::getTexture(0), true);



	//Gui rendering
	renderer->fontRenderer->renderText(glm::vec3(-window->getAspect(), -1.0f, 0.0f), glm::vec2(0.1f), (std::string("Renderer: ") + window->getRenderer()).c_str(), oe::topLeft, glm::vec4(0.0f, 0.0f, 0.0f, 0.4f));
	renderer->draw(guishader, guishader, oe::TextureManager::getTexture(0), true);
}

void update() {
	//box_x += box_vel_x / 5.0f;
	//
	//if (box_x >  1.0) box_vel_x *= -1;
	//if (box_x < -1.0) box_vel_x *= -1;
}

int main() {
	oe::TextureManager::loadTextureAtlas("res/texture/atlas.png", 0, 16);
	glm::mat4 pr = glm::ortho(-window->getAspect(), window->getAspect(), 1.0f, -1.0f);
	guishader->enable(); guishader->SetUniformMat4("pr_matrix", pr);
	window->setBackFaceCulling(false);
	
	gameloop->start();

	return 0;
}