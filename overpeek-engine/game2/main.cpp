#include "../engine.h"

#include <omp.h>
#include <Windows.h>
#include <Box2D/Box2D.h>


#define M_WINDOW_WIDTH		900
#define M_WINDOW_HEIGHT		600
#define M_ASPECT			(float)M_WINDOW_WIDTH / (float)M_WINDOW_HEIGHT
#define M_WORLD_ZOOM		1.0

graphics::Window *window;
graphics::Shader *shader;
logic::GameLoop *gameloop;
graphics::Renderer *renderer;
graphics::TextureManager *textureManager;
b2Vec2 gravity(0.0, 10.0);
b2World world(gravity);
b2Body *groundBody;
b2Body *body;
b2Body *body2;

void keyPress(int key, int action) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_S) world.Step(1.0 / 100.0, 8, 4);
	}
}

void buttonPress(int button, int action) {
	if (action == GLFW_PRESS);
}

long long tmp = 0;
float angle = 0;
void render() {
	window->clear();

	long long startTime = tools::Clock::getMicroseconds();
	
	///RENDERING
	renderer->clear();
	//renderer->renderBoxCentered(groundBody->GetPosition().x / M_WORLD_ZOOM, groundBody->GetPosition().y / M_WORLD_ZOOM, 100.0, 20.0 / M_WORLD_ZOOM, groundBody->GetAngle(), 1);
	//renderer->renderBoxCentered(body->GetPosition().x / M_WORLD_ZOOM, body->GetPosition().y / M_WORLD_ZOOM, 2.0 / M_WORLD_ZOOM, 2.0 / M_WORLD_ZOOM, body->GetAngle(), 1);
	//renderer->renderBoxCentered(body2->GetPosition().x / M_WORLD_ZOOM, body2->GetPosition().y / M_WORLD_ZOOM, 4.0 / M_WORLD_ZOOM, 4.0 / M_WORLD_ZOOM, body2->GetAngle(), 1);

	angle += 0.01;
	renderer->renderBox(0.0, 0.0, 0.5, 0.5, angle, 1);

	renderer->flush(shader, textureManager->getTexture(0));
	///

	tmp += (tools::Clock::getMicroseconds() - startTime) / 1000.0;

	window->update();
	window->input();
}

void update() {
	if (window->close() || window->getKey(GLFW_KEY_ESCAPE)) gameloop->stop();
	///UPDATE
	world.Step(1.0 / 100.0, 6, 2);
	
}

void rapid() {}

//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
int main() {
	//Window
	window = new graphics::Window(M_WINDOW_WIDTH, M_WINDOW_HEIGHT, "Test game", false);
	window->setButtonCallback(buttonPress);
	window->setKeyboardCallback(keyPress);

	//Create shader
	shader = new graphics::Shader("shaders/texture.vert.glsl", "shaders/texture.frag.glsl");

	//Shader stuff
	float debugZoom = 50.0;
	glm::mat4 orthographic = glm::ortho(-M_ASPECT * debugZoom, M_ASPECT* debugZoom, debugZoom, -debugZoom);
	shader->enable(); shader->SetUniformMat4("pr_matrix", orthographic);

	//Renderer
	renderer = new graphics::Renderer("arial.ttf");
	textureManager->loadTexture("recourses/atlas.png", GL_RGBA, 0);

	//Box2D
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, 10.0f);
	groundBody = world.CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 4.0f);
	body = world.CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	body->CreateFixture(&fixtureDef);

	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 10.0f);
	body2 = world.CreateBody(&bodyDef);
	dynamicBox.SetAsBox(2.0, 2.0);
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	body2->CreateFixture(&fixtureDef);


	//Main game loop
	gameloop = new logic::GameLoop(render, update, rapid, 100, 10000);
	gameloop->start();
}