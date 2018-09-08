#include "graphics/window.h"
#include "graphics/shader.h"
#include "graphics/textureLoader.h"
#include "graphics/fontLoader.h"
#include "graphics/simpleRenderer.h"

#include "logic/layers/textLayer.h"
#include "logic/layers/objectLayer.h"
#include "logic/layers/buttonLayer.h"
#include "logic/gameloop.h"
#include "logic/SAT.h"

#define M_WINDOW_WIDTH		1280
#define M_WINDOW_HEIGHT		760
#define M_ASPECT			(float)M_WINDOW_WIDTH / (float)M_WINDOW_HEIGHT

using namespace glm;
using namespace graphics;
using namespace logic;

int fps = 0, ups = 0;

Window *window;
Shader *shader;
SimpleRenderer *renderer;
GameLoop *gameloop;

void render() {
	window->clear();

	renderer->renderTriangle(mat4(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	window->update();
	window->input();
}

void update() {
	if (window->close()) gameloop->stop();
}

int main() {

	//Window
	window = new Window(M_WINDOW_WIDTH, M_WINDOW_HEIGHT, "Test");

	//Create shader
	mat4 projection = ortho(-M_ASPECT, M_ASPECT, 1.0f, -1.0f);
	shader = new Shader("graphics/shaders/shader.test.vert", "graphics/shaders/shader.test.frag");
	shader->enable(); shader->SetUniformMat4("pr_matrix", projection);

	//Renderer
	renderer = new SimpleRenderer(*shader);

	//Main game loop
	gameloop = new GameLoop(render, update, 10000);
	gameloop->start();
	return 0;
}