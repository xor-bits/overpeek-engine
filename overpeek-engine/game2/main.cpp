#include "../engine.h"
#include "cube.h"
#include "../tools/random.h"

#include <omp.h>
#include <Windows.h>
#include <Box2D/Box2D.h>


#define M_WINDOW_WIDTH		900
#define M_WINDOW_HEIGHT		600
#define M_ASPECT			(float)M_WINDOW_WIDTH / (float)M_WINDOW_HEIGHT

#define M_CUBE_COUNT		100

graphics::Window *window;
graphics::Shader *shader;
logic::GameLoop *gameloop;
graphics::Renderer *renderer;
graphics::TextureManager *textureManager;

b2Vec2 gravity(0.0, 10.0);
b2World world(gravity);
b2RevoluteJoint* joint;

Cube *cubes[M_CUBE_COUNT];
Cube *cubeA;
Cube *cubeB;

bool paused = false;
int swapInterval = 0;
int updateCap = 100;
double worldZoom = 60.0;
glm::vec2 cameraOffset = glm::vec2(0.0, 0.0);
std::string fpsText = "0 ms";
std::string upsText = "0 ms";

void setup() {
	tools::Random::seed();
	cubes[0] = new Cube(3, -20, 2, 35, glm::radians(10.0f), false, &world, 0x0004, 0x0002);
	cubes[1] = new Cube(-3, -20, 2, 35, glm::radians(-10.0f), false, &world, 0x0004, 0x0002);
	for (int i = 2; i < M_CUBE_COUNT; i++)
	{
		cubes[i] = new Cube(0, -30 - i, 1, 1, 0, true, &world, 0x0022, 0x0024);
	}

	cubeA = new Cube(0.0, 0.0, 20.0, 2.0, -1.0, true, &world, 0x0044, 0x0022);
	cubeB = new Cube(0.0, 0.0, 1.0, 1.0, 0.0, false, &world, 0x0044, 0x0022);

	b2RevoluteJointDef jointDef;
	jointDef.Initialize(cubeA->m_body, cubeB->m_body, cubeA->m_body->GetWorldCenter());
	jointDef.enableLimit = false;
	jointDef.maxMotorTorque = INFINITY;
	jointDef.motorSpeed = 0.0f;
	jointDef.enableMotor = true;
	joint = (b2RevoluteJoint*)world.CreateJoint(&jointDef);
}

void resetup() {
	for (int i = 0; i < M_CUBE_COUNT; i++) delete cubes[i];
	delete cubeA;
	delete cubeB;
	setup();
}

void worldStep() {
	joint->SetMotorSpeed(tools::Random::random(-50.0, 50.0));
	world.Step(1.0 / 100.0, 6, 2);
}

void keyPress(int key, int action) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_ESCAPE) gameloop->stop();
		if (key == GLFW_KEY_P) paused = !paused;
		if (key == GLFW_KEY_O) worldStep();
		if (key == GLFW_KEY_R) resetup();
	}
}

void buttonPress(int button, int action) {
	if (action == GLFW_PRESS);
}

void scroll(double y) {
	worldZoom -= y*3;
}

void renderDebugScreen() {
	float textScale = 0.05;
	renderer->renderText(-window->getAspect(), -1.0 + textScale * 0, textScale, textScale, 0,
		std::string("Fps: ") +
		std::to_string(gameloop->getFPS()) +
		std::string(", ") +
		fpsText,
		glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);

	renderer->renderText(-window->getAspect(), -1.0 + textScale * 1, textScale, textScale, 0,
		std::string("VSync ") +
		std::to_string(swapInterval) +
		std::string(" frames"),
		glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);

	renderer->renderText(-window->getAspect(), -1.0 + textScale * 2, textScale, textScale, 0,
		std::string("Ups: ") +
		std::to_string(gameloop->getUPS()) +
		std::string(", ") +
		upsText,
		glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);
}

void render() {
	window->clear();
	
	///RENDERING
	long long microSfps = tools::Clock::getMicroseconds();

	renderer->clear(); 
	for (int i = 0; i < M_CUBE_COUNT; i++)
	{
		if (!cubes[i]) continue;
		if (cubes[i]->m_body->GetPosition().y > 35 ||
			cubes[i]->m_body->GetPosition().y < -35 ||
			cubes[i]->m_body->GetPosition().x > 35 || 
			cubes[i]->m_body->GetPosition().x < -35) {
			cubes[i]->m_body->SetTransform(b2Vec2(0.0, -30), cubes[i]->m_body->GetAngle());
			cubes[i]->m_body->SetLinearVelocity(b2Vec2(0.0, 0.0));
		}
		cubes[i]->render(renderer, worldZoom, cameraOffset);
	}
	cubeA->render(renderer, worldZoom, cameraOffset);
	cubeB->render(renderer, worldZoom, cameraOffset);
	renderDebugScreen();

	renderer->flush(shader, textureManager->getTexture(0));

	fpsText = std::string("ms: ") + std::to_string((tools::Clock::getMicroseconds() - microSfps) / 1000.0f);
	///

	window->update();
	window->input();
}

void update() {
	if (window->close()) gameloop->stop();

	if (window->getKey(GLFW_KEY_W)) cameraOffset.y += 0.01 * worldZoom;
	if (window->getKey(GLFW_KEY_S)) cameraOffset.y -= 0.01 * worldZoom;
	if (window->getKey(GLFW_KEY_D)) cameraOffset.x -= 0.01 * worldZoom;
	if (window->getKey(GLFW_KEY_A)) cameraOffset.x += 0.01 * worldZoom;

	///UPDATE
	long long microSups = tools::Clock::getMicroseconds();
	
	if (!paused) {
		worldStep();
	}

	upsText = std::string("ms: ") + std::to_string((tools::Clock::getMicroseconds() - microSups) / 1000.0);
}

void rapid() {}

//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
int main() {
	//Window
	window = new graphics::Window(M_WINDOW_WIDTH, M_WINDOW_HEIGHT, "Test game", false);
	window->setButtonCallback(buttonPress);
	window->setKeyboardCallback(keyPress);
	window->setSwapInterval(swapInterval);
	window->setScrollCallback(scroll);

	//Create shader
	shader = new graphics::Shader("shaders/texture.vert.glsl", "shaders/texture.frag.glsl");

	//Shader stuff
	float debugZoom = 1.0;
	glm::mat4 orthographic = glm::ortho(-M_ASPECT * debugZoom, M_ASPECT* debugZoom, debugZoom, -debugZoom);
	shader->enable(); shader->SetUniformMat4("pr_matrix", orthographic);

	//Renderer
	renderer = new graphics::Renderer("arial.ttf");
	textureManager->loadTexture("recourses/atlas.png", GL_RGBA, 0);

	//Box2D
	setup();

	//Logger
	tools::Logger::info("Game setup succeeded. Running gameloop..");

	//Main game loop
	gameloop = new logic::GameLoop(render, update, rapid, updateCap, 10000);
	gameloop->start();
}