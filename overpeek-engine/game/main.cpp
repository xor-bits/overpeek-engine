#include "../engine.h"
#include "logic/game.h"

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

int main() {
	//Window
	window = new graphics::Window(M_WINDOW_WIDTH, M_WINDOW_HEIGHT, "Test", false);

	//Create shader
	shader = new graphics::Shader("shaders/texture.vert.glsl", "shaders/texture.frag.glsl");
	textShader = new graphics::Shader("shaders/text.vert.glsl", "shaders/text.frag.glsl");
	
	glm::mat4 orthographic = glm::ortho(-M_ASPECT, M_ASPECT, 1.0f, -1.0f);
	shader->enable(); shader->SetUniformMat4("pr_matrix", orthographic);
	textShader->enable(); textShader->SetUniformMat4("pr_matrix", orthographic);
	graphics::SimpleRenderer::init(shader, textShader, "arial.ttf");

	game->init(shader);

	//Main game loop
	gameloop = new logic::GameLoop(render, update, 10000);
	gameloop->start();
}