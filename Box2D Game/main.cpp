#include "../overpeek-engine/engine.h"
#include <Box2D/Box2D.h>

#include "box.h"
#include "circle.h"
#include "triangle.h"


oe::Window* window;
oe::Renderer* renderer;

b2World* world;
std::vector<object*> boxes;
box *ground;




void setup() {
	//Dynamic body
	//for (int i = 0; i < 10; i++)
	//{
	//    boxes.push_back(new triangle(0.0f, -5.0f, 0.5f, false, world));
	//}

	//Static body
	//boxes.push_back(new box(0.0f, 10.0f, 20.0f, 1.0f, true, world));
	boxes.push_back(new box(5.5f, 6.0f, 5.0f, 0.2f, -0.2f, true, world));
	boxes.push_back(new box(-5.5f, 6.0f, 5.0f, 0.2f, 0.2f, true, world));
	boxes.push_back(new box(10.0f, 0.0f, 0.2f, 5.0f, 0.0f, true, world));
	boxes.push_back(new box(-10.0f, 0.0f, 0.2f, 5.0f, 0.0f, true, world));
}

void render(float div) {
	window->clear();
	window->input();

	//Input
	if (window->getKey(OE_KEY_SPACE)) {
		float c = oe::Random::random(0.0f, 100.0f);
		if (c > 66.0f) boxes.push_back(new box(oe::Random::random(-5.0f, 5.0f), -10.0f, oe::Random::random(0.05f, 0.3f), oe::Random::random(0.01f, 0.3f), false, world));
		else if (c > 33.0f) boxes.push_back(new circle(oe::Random::random(-5.0f, 5.0f), -10.0f, oe::Random::random(0.05f, 0.3f), false, world));
		else boxes.push_back(new triangle(oe::Random::random(-5.0f, 5.0f), -10.0f, oe::Random::random(0.05f, 0.3f), false, world));
	}
	if (window->getKey(OE_KEY_R)) {
		for (int i = 0; i < boxes.size(); i++)
		{
			delete boxes[i];
		}
		boxes = std::vector<object*>();
		setup();
	}
	
	//Boxes
	//float maxX = 0, minX = 0;
	for (int i = 0; i < boxes.size(); i++)
	{
		//if (boxes[i]->getX() > maxX) maxX = boxes[i]->getX();
		//if (boxes[i]->getX() < minX) minX = boxes[i]->getX();
		//
		boxes[i]->render(renderer);
	}
	//oe::Logger::out("Min: ", minX, ", Max: ", maxX);

	renderer->fontRenderer->renderText(glm::vec3(-window->getAspect(), -1.0f, 0.0f), glm::vec2(0.1f), (std::string("FPS: ") + std::to_string(window->getGameLoop()->getFPS())).c_str(), oe::topLeft);

	window->setPolygonMode(POLYGON_LINE); 
	renderer->draw(oe::TextureManager::getTexture(0), true);

	window->update();
}

void update() {
	world->Step(1.0f / 144.0f, 3, 8);

	//Boxes
	for (int i = 0; i < boxes.size(); i++)
	{
		if (boxes[i]->getY() > 10.0f) {
			boxes[i]->setPos(boxes[i]->getX(), -10.0f);
			//delete boxes[i];
			//boxes.erase(boxes.begin() + i);
		}
	}
}

void keyboard(int key, int action) {
}

int main() {
	window = new oe::Window(900, 600, "Box2D Game", WINDOW_MULTISAMPLE_X2 | WINDOW_RESIZEABLE);
	renderer = new oe::Renderer("../Project1/res/font/arial.ttf", window->getWidth(), window->getHeight());
	renderer->shader = new oe::Shader("../Project1/res/shader/texture.vert.glsl", "../Project1/res/shader/texture.frag.glsl");
	window->createGameLoop(render, update, 144);

	oe::TextureManager::loadTextureAtlas("../Project1/res/texture/atlas.png", 0, 16);
	glm::mat4 ortho = glm::ortho(-window->getAspect(), window->getAspect(), 1.0f, -1.0f);
	renderer->shader->SetUniformMat4("pr_matrix", ortho);
	window->setKeyboardCallback(keyboard);
	window->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	window->setBackFaceCulling(false);

	b2Vec2 gravity = b2Vec2(0.0f, 9.81f);
	world = new b2World(gravity);
	setup();

	window->getGameLoop()->start();
}