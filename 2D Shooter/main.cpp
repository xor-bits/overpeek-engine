#include "graphics/window.h"
#include "graphics/shader.h"
#include "graphics/textureLoader.h"
#include "graphics/fontLoader.h"
#include "graphics/simpleRenderer.h"
#include "graphics/layers/textLayer.h"

#include <ctime>

#define M_WINDOW_WIDTH		1280
#define M_WINDOW_HEIGHT		760
#define M_ASPECT			(float)M_WINDOW_WIDTH / (float)M_WINDOW_HEIGHT

using namespace glm;
using namespace graphics;

int main() {
	//Window
	Window window = Window(M_WINDOW_WIDTH, M_WINDOW_HEIGHT, "Test");



	//Create shader
	float size = 1.0f;
	mat4 projection = ortho(-size * M_ASPECT, size * M_ASPECT, size, -size);
	Shader shader = Shader("graphics/shaders/shader.test.vert", "graphics/shaders/shader.test.frag");
	shader.enable();
	shader.SetUniformMat4("pr_matrix", projection);

	//Create textshader
	Shader textShader = Shader("graphics/shaders/shader.text.vert", "graphics/shaders/shader.text.frag");
	textShader.enable();
	textShader.SetUniformMat4("pr_matrix", projection);


	//Renderer
	SimpleRenderer renderer = SimpleRenderer();

	//Load font
	FontLoader fontLoader = FontLoader("arial.ttf", textShader);

	//Set textlayer
	Layer *layer = new TextLayer(0.0f, 0.0f, vec3(0.0f, 1.0f, 1.0f), "text", fontLoader);
	layer->setVelX(0.001f);

	//Settings
	glClearColor(0.18f, 0.18f, 0.20f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//Main game loop
	while (!window.close()) {
		window.clear();

		layer->update();
		layer->render();

		window.update();
	}
	return 0;
}