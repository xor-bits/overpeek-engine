#include "../engine.h"
#include "logic/game.h"

#include <omp.h>

#define M_WINDOW_WIDTH		900
#define M_WINDOW_HEIGHT		600
#define M_ASPECT			(float)M_WINDOW_WIDTH / (float)M_WINDOW_HEIGHT

graphics::Window *window;
graphics::Shader *shader;
graphics::Shader *textShader;
logic::GameLoop *gameloop;
graphics::Camera *camera;
Game *game;

void render() {
	window->clear();

	game->render();

	window->update();
	window->input();
}

void update() {
	if (window->close() || window->getKey(GLFW_KEY_ESCAPE)) gameloop->stop();
	game->update();
}

void rapid() {
	game->rapidUpdate();
}

int main() {
	//Window
	window = new graphics::Window(M_WINDOW_WIDTH, M_WINDOW_HEIGHT, "Test game", false);

	//Create shader
	shader = new graphics::Shader("shaders/texture.vert.glsl", "shaders/texture.frag.glsl");
	textShader = new graphics::Shader("shaders/text.vert.glsl", "shaders/text.frag.glsl");
	
	float debugZoom = 1.0;
	glm::mat4 orthographic = glm::ortho(-M_ASPECT * debugZoom, M_ASPECT* debugZoom, debugZoom, -debugZoom);
	shader->enable(); shader->SetUniformMat4("pr_matrix", orthographic);
	textShader->enable(); textShader->SetUniformMat4("pr_matrix", orthographic);
	graphics::SimpleRenderer::init(shader, textShader, "arial.ttf");

	//Main game loop
	gameloop = new logic::GameLoop(render, update, rapid, 10000);
	game->init(shader, window, gameloop);
	gameloop->start();
}