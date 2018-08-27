#include "graphics/window.h"
#include "graphics/shader.h"
#include "graphics/textureLoader.h"
#include "graphics/fontLoader.h"
#include "graphics/simpleRenderer.h"

#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ctime>

#define M_WINDOW_WIDTH		1280
#define M_WINDOW_HEIGHT		760
#define M_ASPECT			(float)M_WINDOW_WIDTH / (float)M_WINDOW_HEIGHT

using namespace glm;
using namespace graphics;

int main() {
	//Window
	Window window = Window(M_WINDOW_WIDTH, M_WINDOW_HEIGHT, "Test");

	//Renderer
	SimpleRenderer renderer = SimpleRenderer();

	//Load font
	FontLoader fontLoader = FontLoader("arial.ttf");

	//Settings
	glClearColor(0.18f, 0.18f, 0.20f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//Create shader
	Shader shader = Shader("graphics/shaders/shader.test.vert", "graphics/shaders/shader.test.frag");
	shader.enable();

	float size = 1.0f;
	mat4 projection = ortho(-size * M_ASPECT, size * M_ASPECT, size, -size);

	shader.enable();
	shader.SetUniformMat4("pr_matrix", projection);
	
	//Main game loop
	while (!window.close()) {
		window.clear();

		renderer.renderTriangle(shader, 0.0f, 0.0f, 0.2f, 0.2f, vec4(0.0f, 1.0f, 1.0f, 1.0f));
		//renderer.renderQuad(shader, 0.0f, 0.0f, 0.2f, 0.2f, vec4(1.0f, 0.0f, 1.0f, 1.0f));
		//renderer->renderQuad(shader2, 0.0f, 0.0f, 0.2f, 0.2f, vec4(1.0f, 1.0f, 1.0f, 1.0f));
		//fontLoader.renderText(shader, "Test", 0.0f, 0.0f, 1.0f, vec3(1.0f, 1.0f, 1.0f));

		window.update();
	}
	return 0;
}