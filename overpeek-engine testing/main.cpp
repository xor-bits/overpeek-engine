#include "stdafx.h"


void render(float corrector); void update();
oe::Window *window = new oe::Window(900, 600, "overpeek-engine tester", false, 0, false);
oe::Shader *shader = new oe::Shader("res/shader/texture.vert.glsl", "res/shader/texture.frag.glsl");
oe::Renderer *renderer = new oe::Renderer("res/font/arial.ttf", window);
oe::GameLoop *gameloop = new oe::GameLoop(render, update, 5, window);

float box_x = 0;
float box_vel_x = 1;

void render(float corrector) {
	if (window->getKey(OE_KEY_A)) box_vel_x -= 1.0 / gameloop->fastFPS();
	if (window->getKey(OE_KEY_D)) box_vel_x += 1.0 / gameloop->fastFPS();

	//oe::Logger::out(oe::info, gameloop->fastFPS());

	renderer->renderBox(glm::vec2(box_x + (box_vel_x * corrector / 5.0f), 0.0), glm::vec2(0.05), 20, glm::vec4(1.0, 0.5, 0.0, 1.0));
	renderer->draw(shader, shader, oe::TextureManager::getTexture(0), true);
}

void update() {
	box_x += box_vel_x / 5.0f;

	if (box_x >  1.0) box_vel_x *= -1;
	if (box_x < -1.0) box_vel_x *= -1;
}

int main() {
	oe::TextureManager::loadTextureAtlas("res/texture/atlas.png", 0, 16);
	glm::mat4 pr = glm::ortho(-window->getAspect(), window->getAspect(), 1.0f, -1.0f);
	shader->enable(); shader->SetUniformMat4("pr_matrix", pr);

	gameloop->start();
	return 0;
}