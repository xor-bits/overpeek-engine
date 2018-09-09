#include "graphics/all.h"

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
Shader *textShader;
GameLoop *gameloop;
Camera *camera;
SimpleRenderer *renderer;

vec3 camPos = vec3(0.0f);

void render() {
	window->clear();

	vec3 points[4] = {
		{ -1.0f, -1.0f, -1.0f },
		{ 1.0f, -1.0f, -1.0f },
		{ 1.0f, 1.0f, -1.0f },
		{ -1.0f, 1.0f, -1.0f }
	};
	renderer->submit(new Quad(shader, points, vec4(1.0f, 0.0f, 0.0f, 1.0f)));

	renderer->render();

	window->update();
	window->input();
}

void update() {
	if (window->close() || window->getKey(GLFW_KEY_ESCAPE)) gameloop->stop();

	
	if (window->getKey(GLFW_KEY_S)) camPos.z += 0.02f;
	if (window->getKey(GLFW_KEY_W)) camPos.z -= 0.02f;
	if (window->getKey(GLFW_KEY_D)) camPos.x += 0.02f;
	if (window->getKey(GLFW_KEY_A)) camPos.x -= 0.02f;
	if (window->getKey(GLFW_KEY_SPACE)) camPos.y += 0.02f;
	if (window->getKey(GLFW_KEY_LEFT_SHIFT)) camPos.y -= 0.02f;


	vec2 mouse = window->getMousePos();
	std::cout << gameloop->getFPS() << std::endl;
	//std::cout << mouse.x << ", " << mouse.y << std::endl;

	camera->setRotation(mouse.x, mouse.y);
	camera->setPosition(camPos);
	
	shader->enable();
	shader->SetUniformMat4("pr_matrix", camera->getProjectionMatrix());
	shader->SetUniformMat4("vw_matrix", camera->getViewMatrix());
}

int main() {
	//Window
	window = new Window(M_WINDOW_WIDTH, M_WINDOW_HEIGHT, "Test");
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Create shader
	shader = new Shader("shaders/normal.vert", "shaders/normal.frag");

	textShader = new Shader("shaders/text.vert", "shaders/text.frag");
	mat4 orthographic = ortho(-M_ASPECT, M_ASPECT, 1.0f, -1.0f);
	textShader->enable(); textShader->SetUniformMat4("pr_matrix", orthographic);

	//Camera
	camera = new Camera(vec3(0.0f, 0.0f, 5.0f), 0.0f, 0.0f, 90.0f, M_ASPECT);

	renderer = new SimpleRenderer();

	//Main game loop
	gameloop = new GameLoop(render, update, 10000);
	gameloop->start();
	return 0;
}