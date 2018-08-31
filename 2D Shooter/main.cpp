#include "graphics/window.h"
#include "graphics/shader.h"
#include "graphics/textureLoader.h"
#include "graphics/fontLoader.h"
#include "graphics/simpleRenderer.h"
#include "graphics/layers/textLayer.h"
#include "graphics/layers/objectLayer.h"
#include "graphics/layers/buttonLayer.h"

#include "tools/clock.h"

#define M_WINDOW_WIDTH		1280
#define M_WINDOW_HEIGHT		760
#define M_ASPECT			(float)M_WINDOW_WIDTH / (float)M_WINDOW_HEIGHT

using namespace glm;
using namespace graphics;

int fps = 0, ups = 0;

Window *window;
Shader *textShader;
Shader *shader;
FontLoader *fontLoader;
ObjectLayer *layer3;
std::vector<ObjectLayer> projectiles;
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

	//Create textshader
	textShader = new Shader("graphics/shaders/shader.text.vert", "graphics/shaders/shader.text.frag");
	textShader->enable();
	textShader->SetUniformMat4("pr_matrix", projection);


	//Renderer
	renderer = new SimpleRenderer(*shader);

	//Load font
	fontLoader = new FontLoader("arial.ttf", *textShader);



	//Set textlayer
	layer3 = new ObjectLayer(0.0f, 0.0f, 0.1f, 0.1f, vec4(0.2f, 0.3f, 0.4f, 1.0f), *renderer, LAYER_OBJECT_QUAD);



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
		if (current - start > 1000000) {
			fps = frames; ups = updates;
			frames = 0; updates = 0;
			start += 1000000;
		}

		//Update
		window->update();
	}
	return 0;
}

void render() {
	for (int i = 0; i < projectiles.size(); i++)
	{
		projectiles[i].render();
	}

	layer3->render();
}

void update() {
	fvec2 tmp = fvec2(0.0f, 0.0f);
	if (window->getKey(GLFW_KEY_D)) layer3->setAccX(0.001f);
	else if (window->getKey(GLFW_KEY_A)) layer3->setAccX(-0.001f);
	else layer3->setAccX(0.0f);
	if (window->getKey(GLFW_KEY_S)) layer3->setAccY(0.001f);
	else if (window->getKey(GLFW_KEY_W)) layer3->setAccY(-0.001f);
	else layer3->setAccY(0.0f);
	layer3->setVelX(layer3->getVelX() / 1.03);
	layer3->setVelY(layer3->getVelY() / 1.03);

	if (window->getKey(GLFW_KEY_SPACE)) {
		std::cout << projectiles.size() << std::endl;
		ObjectLayer tmp2 = ObjectLayer(layer3->getX(), layer3->getY(), 0.1f, 0.1f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), *renderer, LAYER_OBJECT_TRIANGLE);
		tmp2.setVelY(0.01f);
		projectiles.push_back(tmp2);
	}

	layer3->update();
	for (int i = 0; i < projectiles.size(); i++)
	{
		projectiles[i].update();
	}

}