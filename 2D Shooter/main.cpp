#include "graphics/window.h"
#include "graphics/shader.h"
#include "graphics/textureLoader.h"
#include "graphics/fontLoader.h"
#include "graphics/simpleRenderer.h"

#include "logic/layers/textLayer.h"
#include "logic/gameloop.h"
#include "logic/SAT.h"

#define M_WINDOW_WIDTH		1280
#define M_WINDOW_HEIGHT		760
#define M_ASPECT			(float)M_WINDOW_WIDTH / (float)M_WINDOW_HEIGHT

using namespace glm;
using namespace graphics;
using namespace logic;

Window *window;
Shader *shader;
GameLoop *gameloop;
TextLayer *layer;

void render() {
	window->clear();

	layer->render();

	window->update();
	window->input();
}

void update() {
	if (window->close()) gameloop->stop();
	layer->update();
	layer->setText(std::to_string(gameloop->getFPS()));
}

int main() {
	//Window
	window = new Window(M_WINDOW_WIDTH, M_WINDOW_HEIGHT, "Test");

	//Create shader
	mat4 projection = ortho(-M_ASPECT, M_ASPECT, 1.0f, -1.0f);
	shader = new Shader("shaders/text.vert", "shaders/text.frag");
	shader->enable(); shader->SetUniformMat4("pr_matrix", projection);

	FontLoader tmp = FontLoader("arial.ttf", *shader);
	layer = new TextLayer(-M_ASPECT, -1.0f, glm::vec3(1.0f, 1.0f, 1.0f), "text", tmp);
	layer->setSizeX(0.1f);
	layer->setSizeY(0.1f);
	layer->setYAlignment(TEXT_ALIGN_BOTTOM);

	//Main game loop
	gameloop = new GameLoop(render, update, 10000);
	gameloop->start();
	return 0;
}