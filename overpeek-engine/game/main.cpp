#include "../engine.h"
#include "logic/game.h"

#include <omp.h>
#include <Windows.h>

#define M_WINDOW_WIDTH		1280
#define M_WINDOW_HEIGHT		720
#define M_ASPECT			(float)M_WINDOW_WIDTH / (float)M_WINDOW_HEIGHT

graphics::Window *window;
graphics::Shader *shader;
logic::GameLoop *gameloop;
graphics::Camera *camera;
Game game;

long long tmp = 0;

void keyPress(int key, int action) {
	if (action == GLFW_PRESS) game.keyPress(key); 
}

void buttonPress(int button, int action) {
	if (action == GLFW_PRESS) game.buttonPress(button);
}

void scroll(double y) {
	game.scroll(y);
}

void render() {
	window->clear();

	long long startTime = tools::Clock::getMicroseconds();
	game.render();
	tmp += (tools::Clock::getMicroseconds() - startTime) / 1000.0;

	window->update();
	window->input();
}

int x = 0;
void update() {
	if (window->close() || window->getKey(GLFW_KEY_ESCAPE)) gameloop->stop();
	game.update();
	x++;
}

void rapid() {
	game.rapidUpdate();
}

//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
int main() {
	tools::Logger::setup();

	//Audio
#if ENABLE_AUDIO
	audio::AudioManager::init();
#endif

	//Window
	tools::Logger::info("Creating window");
	window = new graphics::Window(M_WINDOW_WIDTH, M_WINDOW_HEIGHT, "Test game", false);
	window->setSwapInterval(NULL);
	window->setButtonCallback(buttonPress);
	window->setKeyboardCallback(keyPress);
	window->setScrollCallback(scroll);

	//Create shader
	tools::Logger::info("Creating shader");
	shader = new graphics::Shader("shaders/texture.vert.glsl", "shaders/texture.frag.glsl");
	tools::Logger::info("Shader created!");
	
	//Shader stuff
	glm::mat4 orthographic = glm::ortho(-M_ASPECT * DEBUG_ZOOM, M_ASPECT * DEBUG_ZOOM, DEBUG_ZOOM, -DEBUG_ZOOM);
	shader->enable(); shader->SetUniformMat4("pr_matrix", orthographic);

	//Main game loop
	tools::Logger::info("Starting gameloop");
	gameloop = new logic::GameLoop(render, update, rapid, 100, 10000);
	game.init(shader, window, gameloop);
	gameloop->start();
	game.close();
}