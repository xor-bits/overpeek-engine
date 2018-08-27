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

Window *window;
SimpleRenderer *renderer;
FontLoader *fontLoader;
Shader *shader;
Shader *shader2;

unsigned int fps = 144;

void key_callback(int key, int action) {
	std::cout << key << action << std::endl;
}

void render() {
	//Get mouse pos
	double mouseX, mouseY;
	window->getMousePos(mouseX, mouseY);
	//shader.setUniform2f("mouse_pos", vec2(mouseX, mouseY));

	renderer->renderQuad(*shader2, 0.0f, 0.0f, 0.5f, 0.5f, vec4(1.0f, 0.0f, 0.5f, 1.0f));
	fontLoader->renderText(*shader, std::to_string(fps), -M_ASPECT, -0.9f, 1.0f, vec3(1.0f, 1.0f, 1.0f));
}

int main() {
	//Window
	window = new Window(M_WINDOW_WIDTH, M_WINDOW_HEIGHT, "Test");
	window->setKeyboardCallback(&key_callback);

	//Renderer
	renderer = new SimpleRenderer();

	//Load font
	fontLoader = new FontLoader("arial.ttf");

	//Texture loader
<<<<<<< HEAD
	TextureLoader textureLoader = TextureLoader();
=======
	//TextureLoader textureLoader = TextureLoader();
>>>>>>> e104e42458129b39e86597a09d16e0480e24bd4b
	//unsigned int texture = textureLoader.getTexture("test.png");

	//Settings
	glClearColor(0.18f, 0.18f, 0.20f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//Create text shader
	shader = new Shader("graphics/shaders/shader.text.vert", "graphics/shaders/shader.text.frag");

	//Create shader
	shader2 = new Shader("graphics/shaders/shader.test.vert", "graphics/shaders/shader.test.frag");
	shader2->enable();

	float size = 1.0f;
	mat4 projection = ortho(-size * M_ASPECT, size * M_ASPECT, size, -size);
<<<<<<< HEAD
	shader.enable();
	shader.SetUniformMat4("pr_matrix", projection);
	shader2.enable();
	shader2.SetUniformMat4("pr_matrix", projection);
	
	//Main game loop
	while (!window.close()) {
		window.clear();

		//Get mouse pos
		double mouseX, mouseY;
		window.getMousePos(mouseX, mouseY);

		float orientation = atan2(mouseY, mouseX) + half_pi<float>();
		


		std::cout << orientation << std::endl;


		//shader.setUniform2f("mouse_pos", vec2(mouseX, mouseY));

		{
			//DrawGL_TEXTURE_2D, texture);
			//glBindVertexArray(VAO);
			//
			//mat4 modelMatrix = translate(mat4(1.0f), vec3(posX, posY, 0.0f));
			//modelMatrix *= rotate(mat4(1.0f), radians(orientation), vec3(0.0f, 0.0f, 1.0f));
			//shader.SetUniformMat4("ml_matrix", modelMatrix);
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		};

		mat4 ml_matrix = mat4(1.0f);
		ml_matrix *= translate(mat4(1.0f), vec3(mouseX, mouseY, 0.0f));
		ml_matrix *= rotate(mat4(1.0f), orientation, vec3(0.0f, 0.0f, 1.0f));

		shader.SetUniformMat4("ml_matrix", ml_matrix);
		renderer.renderQuad(shader2, 0.0f, 0.0f, 0.2f, 0.2f, vec4(1.0f, 1.0f, 1.0f, 1.0f));
		//fontLoader.renderText(shader, "Test", 0.0f, 0.0f, 1.0f, vec3(1.0f, 1.0f, 1.0f));
		
=======
	shader->enable();
	shader->SetUniformMat4("pr_matrix", projection);
	shader2->enable();
	shader2->SetUniformMat4("pr_matrix", projection);



	//Main game loop
	clock_t startTime = 0;
	unsigned int frames = 0;
	while (!window->close()) {
		window->clear();

		render();

		//Get fps
		frames++;
		if (clock() - startTime > CLOCKS_PER_SEC) { //every second
			std::cout << frames << std::endl;
			fps = frames;
			frames = 0;
			startTime = clock();
		}
>>>>>>> e104e42458129b39e86597a09d16e0480e24bd4b

		//Update window
		window->update();
	}
	return 0;
}