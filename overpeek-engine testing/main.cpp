#include "stdafx.h"

#define PI	3.141f

void render(float corrector); void update();
oe::Window *window = new oe::Window(1920, 1080, "overpeek-engine tester", WINDOW_FULLSCREEN | WINDOW_TRANSPARENT | WINDOW_MULTISAMPLE_X2);
oe::Shader* shader = new oe::Shader("res/shader/texture.vert.glsl", "res/shader/texture.frag.glsl");
oe::Shader* guishader = new oe::Shader("res/shader/texture.vert.glsl", "res/shader/texture.frag.glsl");
oe::Renderer *renderer = new oe::Renderer("res/font/arialbd.ttf", window);
oe::GameLoop *gameloop = new oe::GameLoop(render, update, 5, window);

float xyangle = PI / 2.0f;
float zangle = 0.0f;

glm::vec3 pos = glm::vec3(0.0f, 0.0f, -1.0f);
//float box_vel_x = 1;

void render(float corrector) {
	window->setCursorPos(1920 / 2, 1080 / 2);
	window->input();
	window->clear();

	//xyangle += 5.0 / gameloop->fastFPS();
	double mx, my;
	window->getMousePos(mx, my);
	xyangle -= mx;
	//zangle += my;

	//Inputs
	if (window->getKey(OE_KEY_W)) { 
		pos.z += 5.0 / gameloop->fastFPS() * sin(xyangle);
		pos.x += 5.0 / gameloop->fastFPS() * cos(xyangle);
	}
	if (window->getKey(OE_KEY_S)) {
		pos.z -= 5.0 / gameloop->fastFPS() * sin(xyangle);
		pos.x -= 5.0 / gameloop->fastFPS() * cos(xyangle);
	}
	if (window->getKey(OE_KEY_A)) {
		pos.z += 5.0 / gameloop->fastFPS() * sin(xyangle + PI / 2.0f);
		pos.x += 5.0 / gameloop->fastFPS() * cos(xyangle + PI / 2.0f);
	}
	if (window->getKey(OE_KEY_D)) {
		pos.z += 5.0 / gameloop->fastFPS() * sin(xyangle - PI / 2.0f);
		pos.x += 5.0 / gameloop->fastFPS() * cos(xyangle - PI / 2.0f);
	}

	//Model view projection matrix updates
	glm::mat4 pers= glm::perspectiveFov(glm::radians(90.0f), window->getAspect(), 1.0f, 0.01f, 100.0f);
	glm::mat4 view = glm::lookAt(
		pos,
		glm::vec3(pos.x + cos(xyangle), zangle, pos.z + sin(xyangle)),
		glm::vec3(0.0f, -1.0f, 0.0f)
	);
	glm::mat4 mvp = pers * view;
	//glm::mat4 mvp = glm::ortho(-window->getAspect(), window->getAspect(), 1.0f, -1.0f);
	shader->enable(); shader->SetUniformMat4("pr_matrix", mvp);

	//Triangle
	glm::vec3 tip(0.0f, -0.5f, 0.5f);
	glm::vec3 left(0.5f, 0.5f, 0.5f);
	glm::vec3 right(-0.5f, 0.5f, 0.5f);
	renderer->triangleRenderer->submitVertex(oe::VertexData(tip  , glm::vec2(0.0f, 0.0f), 20, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));
	renderer->triangleRenderer->submitVertex(oe::VertexData(left , glm::vec2(0.0f, 1.0f), 20, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
	renderer->triangleRenderer->submitVertex(oe::VertexData(right, glm::vec2(1.0f, 1.0f), 20, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
	

	//Circle
	//renderer->triangleRenderer->submitSphere(0.5f, -0.5f, 0.1f, 32, OE_COLOR_CYAN, 20);
	//renderer->triangleRenderer->submitSphere(0.5f, 0.5f, 0.1f, 32, OE_COLOR_CYAN, 20);
	//renderer->triangleRenderer->submitSphere(-0.5f, -0.5f, 0.1f, 32, OE_COLOR_CYAN, 20);
	//renderer->triangleRenderer->submitSphere(-0.5f, 0.5f, 0.1f, 32, OE_COLOR_CYAN, 20);
	renderer->draw(shader, shader, oe::TextureManager::getTexture(0), true);


	//Gui rendering
	std::string fps = "Fps: " + std::to_string(gameloop->getFPS());
	renderer->fontRenderer->renderText(glm::vec3(-window->getAspect(), -0.9f, 0.0f), glm::vec2(0.1f), fps.c_str(), oe::topLeft, glm::vec4(0.0f, 0.0f, 0.0f, 0.4f));
	renderer->fontRenderer->renderText(glm::vec3(-window->getAspect(), -1.0f, 0.0f), glm::vec2(0.1f), (std::string("Renderer: ") + window->getRenderer()).c_str(), oe::topLeft, glm::vec4(0.0f, 0.0f, 0.0f, 0.4f));
	renderer->draw(guishader, guishader, oe::TextureManager::getTexture(0), true);

	window->update();
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
	window->setClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	window->showCursor(false);
	window->setSwapInterval(0);
	
	gameloop->start();

	return 0;
}