#include "graphics/all.h"

#include "logic/gameloop.h"
#include "logic/SAT.h"

#define M_WINDOW_WIDTH		1920
#define M_WINDOW_HEIGHT		1080
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

Quad *sprite;

vec3 camPos = vec3(0.0f);
vec2 offset = dvec2(0.0, 0.0);
float zoom = 2;
float constant = 2;
float expValue = 2;

void render() {
	window->clear();

	//renderer->beginSubmits();
	renderer->submit(sprite);
	//renderer->endSubmits();
	shader->setUniform1f("zoom", zoom);
	shader->setUniform2f("offset", offset);
	shader->setUniform1f("constant", constant);
	shader->setUniform1f("exp", floor(expValue));
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

	float speedMult = 1;
	if (window->getKey(GLFW_KEY_LEFT_SHIFT)) speedMult = 5;
	if (window->getKey(GLFW_KEY_R)) constant += speedMult * 0.001f;
	if (window->getKey(GLFW_KEY_F)) constant -= speedMult * 0.001f;
	if (window->getKey(GLFW_KEY_T)) expValue += speedMult * 0.01f;
	if (window->getKey(GLFW_KEY_G)) expValue -= speedMult * 0.01f;

	if (window->getKey(GLFW_KEY_PAGE_UP)) zoom *= 0.99;
	if (window->getKey(GLFW_KEY_PAGE_DOWN)) zoom *= 1.01;
	if (window->getKey(GLFW_KEY_UP)) offset.y += 0.01 * zoom;
	if (window->getKey(GLFW_KEY_DOWN)) offset.y -= 0.01 * zoom;
	if (window->getKey(GLFW_KEY_LEFT)) offset.x -= 0.01 * zoom;
	if (window->getKey(GLFW_KEY_RIGHT)) offset.x += 0.01 * zoom;
	


	vec2 mouse = window->getMousePos();
	std::cout << gameloop->getFPS() << std::endl;

	camera->setRotation(mouse.x, mouse.y);
	camera->setPosition(camPos);
	
	shader->enable();
	shader->SetUniformMat4("pr_matrix", camera->getProjectionMatrix());
	shader->SetUniformMat4("vw_matrix", camera->getViewMatrix());
}

int main() {
	//Window
	window = new Window(M_WINDOW_WIDTH, M_WINDOW_HEIGHT, "Test");
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Create shader
	shader = new Shader("shaders/normal.vert", "shaders/normal.frag");

	textShader = new Shader("shaders/text.vert", "shaders/text.frag");
	mat4 orthographic = ortho(-M_ASPECT, M_ASPECT, 1.0f, -1.0f);
	textShader->enable(); textShader->SetUniformMat4("pr_matrix", orthographic);

	//Camera
	camera = new Camera(vec3(0.0f, 0.0f, 5.0f), 0.0f, 0.0f, 90.0f, M_ASPECT);

	renderer = new SimpleRenderer();

	vec3 points[4] = {
		{ -1.0f, -1.0f, -1.0f },
		{  1.0f, -1.0f, -1.0f },
		{  1.0f,  1.0f, -1.0f },
		{ -1.0f,  1.0f, -1.0f }
	};
	sprite = new Quad(shader, points, vec4(1.0f, 0.0f, 0.0f, 1.0f));

	//Main game loop
	gameloop = new GameLoop(render, update, 10000);
	gameloop->start();
}