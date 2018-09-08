#include "graphics/window.h"
#include "graphics/shader.h"
#include "graphics/textureLoader.h"
#include "graphics/fontLoader.h"
#include "graphics/simpleRenderer.h"
#include "graphics/layers/textLayer.h"
#include "graphics/layers/objectLayer.h"
#include "graphics/layers/buttonLayer.h"
#include "logic/SAT.h"

#include "tools/clock.h"
#include "tools/random.h"

#define ROOT_TWO 1.4142135623730951

#define M_WINDOW_WIDTH		1280
#define M_WINDOW_HEIGHT		760
#define M_ASPECT			(float)M_WINDOW_WIDTH / (float)M_WINDOW_HEIGHT

using namespace glm;
using namespace graphics;

int fps = 0, ups = 0;

Window *window;
Shader *shader;
ObjectLayer *layer3;
ObjectLayer *layer2;
SimpleRenderer *renderer;

void render();
void update();

int main() {

	//Window
	window = new Window(M_WINDOW_WIDTH, M_WINDOW_HEIGHT, "Test");

	//Settings
	glClearColor(0.18f, 0.18f, 0.20f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	//Create shader
	float size = 1.0f;
	mat4 projection = ortho(-size * M_ASPECT, size * M_ASPECT, size, -size);
	shader = new Shader("graphics/shaders/shader.test.vert", "graphics/shaders/shader.test.frag");
	shader->enable();
	shader->SetUniformMat4("pr_matrix", projection);


	//Renderer
	renderer = new SimpleRenderer(*shader);



	//Set layer
	layer3 = new ObjectLayer(0.2f, 0.0f, 0.1f, 0.1f, vec4(0.2f, 0.3f, 0.4f, 1.0f), *renderer, LAYER_OBJECT_CIRCLE, true);
	layer2 = new ObjectLayer(0.0f, 0.0f, 0.1f, 0.1f, vec4(0.4f, 0.5f, 0.5f, 1.0f), *renderer, LAYER_OBJECT_CIRCLE, true);



	//Main game loop
	int frames = 0, updates = 0;
	long long start = tools::getMicroseconds();
	long long previous = start;
	long long lag = 0;
	while (!window->close())
	{
		//Clear
		window->clear();

		long long current = tools::getMicroseconds();
		long long elapsed = current - previous;
		previous = current;
		lag += elapsed;

		window->input();
		
		while (lag >= 10000) //10 000 microseconds(10 ms) per update
		{
			updates++;
			update();
			lag -= 10000;
		}
		
		frames++;
		render();


		//Frame/update counter
		if (current - start > 10000) {
			fps = frames; ups = updates;
			frames = 0; updates = 0;
			start += 10000;
		}

		//Update
		window->update();
	}
	return 0;
}

void render() {
	//renderer->renderCircle(mat4(1.0f), 0.0f, 0.0f, layer3->getRotation(), 3, vec4(1.0f, 0.8f, 0.6f, 1.0f));

	//renderer->renderQuad(mat4(1.0f), 0.5f, 1.0f, 0.1f, 0.1f, vec4(0.0f, 0.0f, 1.0f, 1.0f));
	renderer->renderLine(mat4(1.0f), 2, vec2(1.0f, 0.0f), vec2(-1.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	vec2 p = logic::closestPointOnLine(vec2(1.0f, 0.0f), vec2(-1.0f, 1.0f), vec2(layer3->getX(), layer3->getY()));
	renderer->renderQuad(mat4(1.0f), p.x, p.y, 0.1f, 0.1f, vec4(1.0f, 0.0f, 0.0f, 1.0f));


	layer2->render();
	layer3->render();
}

void update() {
	if (window->getKey(GLFW_KEY_D)) layer3->setAccX(0.0008f);
	else if (window->getKey(GLFW_KEY_A)) layer3->setAccX(-0.0008f);
	else layer3->setAccX(0.0f);
	if (window->getKey(GLFW_KEY_S)) layer3->setAccY(0.0008f);
	else if (window->getKey(GLFW_KEY_W)) layer3->setAccY(-0.0008f);
	else layer3->setAccY(0.0f);

	if (layer2->getX() > M_ASPECT) layer2->setVelX(-layer2->getVelX());
	else if (layer2->getX() < -M_ASPECT) layer2->setVelX(-layer2->getVelX());
	if (layer2->getY() > 1.0) layer2->setVelY(-layer2->getVelY());
	else if (layer2->getY() < -1.0) layer2->setVelY(-layer2->getVelY());
	//layer2->setVelX(layer2->getVelX() * 0.99);
	//layer2->setVelY(layer2->getVelY() * 0.99);

	if (layer3->getX() > M_ASPECT) layer3->setX(-M_ASPECT);
	else if (layer3->getX() < -M_ASPECT) layer3->setX(M_ASPECT);
	if (layer3->getY() > 1.0) layer3->setY(-1.0);
	else if (layer3->getY() < -1.0) layer3->setY(1.0);
	layer3->setVelX(layer3->getVelX() * 0.95);
	layer3->setVelY(layer3->getVelY() * 0.95);

	double mX, mY;
	window->getMousePos(mX, mY);
	float angle = atan2(mY - layer3->getY(), mX - layer3->getX()) + glm::half_pi<float>();
	layer3->setRotation(angle);

	//Separating axis theorem
	bool interact = logic::circleSAT(layer3, layer2);
	if (interact) {
		float tmpx = layer3->getVelX();
		float tmpy = layer3->getVelY();
		layer3->setVelX(layer2->getVelX());
		layer3->setVelY(layer2->getVelY());

		layer2->setVelX(tmpx);
		layer2->setVelY(tmpy);
	}

	layer2->update();
	layer3->update();

}