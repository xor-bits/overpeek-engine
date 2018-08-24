#include "graphics/window.h"
#include "graphics/shader.h"
#include "graphics/textureLoader.h"
#include "graphics/fontLoader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>

#define M_WINDOW_WIDTH		1280
#define M_WINDOW_HEIGHT		760
#define M_ASPECT			(float)M_WINDOW_WIDTH / (float)M_WINDOW_HEIGHT

using namespace glm;
using namespace graphics;

int main() {
	//Window
	Window window = Window(M_WINDOW_WIDTH, M_WINDOW_HEIGHT, "Test");

	//Load font
	FontLoader fontLoader = FontLoader("arial.ttf", "graphics/shaders/shader.text.vert", "graphics/shaders/shader.text.frag");

	//Texture loader
	TextureLoader textureLoader = TextureLoader();
	unsigned int texture = textureLoader.getTexture("test.png");

	//Settings
	glClearColor(0.18f, 0.18f, 0.20f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	{
		//Setup vertex data
	//float vertices[] = {
	//	 //Position				//Color				//Texture pos
	//	-0.5f, -0.5f,  0.0f,	1.0f, 0.0f, 0.0f,	//1.0f, 1.0f, //0 top left
	//	 0.5f, -0.5f,  0.0f,	0.0f, 1.0f, 0.0f,	//0.0f, 1.0f, //1 top right
	//	 0.5f,  0.5f,  0.0f,	0.0f, 0.0f, 1.0f,	//1.0f, 0.0f, //2 bottom right
	//	-0.5f,  0.5f,  0.0f,	1.0f, 0.0f, 1.0f,	//0.0f, 0.0f  //3 bottom left
	//};
	//unsigned int indices[] = {
	//	 0, 1, 3,
	//	 1, 2, 3
	//};

	//unsigned int VBO, VAO, EBO;
	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	//glBindVertexArray(VAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Vertex array configuration
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	//unsigned int texture = TextureLoader::loadTexture("test.png", GL_RGB);

	//Create normal shader
	//Shader shader = Shader("graphics/shaders/shader.test.vert", "graphics/shaders/shader.test.frag");

	//float size = 1.0f;
	//mat4 projection = ortho(-size * M_ASPECT, size * M_ASPECT, size, -size);
	//shader.SetUniformMat4("pr_matrix", projection);
	};
	
	//Create text shader
	Shader shader = Shader("graphics/shaders/shader.text.vert", "graphics/shaders/shader.text.frag");
	shader.enable();

	float size = 1.0f;
	mat4 projection = ortho(-size * M_ASPECT, size * M_ASPECT, size, -size);
	shader.SetUniformMat4("pr_matrix", projection);

	
	//Main game loop
	while (!window.close()) {
		window.clear();

		//Get mouse pos
		double mouseX, mouseY;
		window.getMousePos(mouseX, mouseY);
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

		//glBindTexture(GL_TEXTURE_2D, texture);
		
		fontLoader.renderText(shader, "Test", 0.0f, 0.0f, 1.0f, vec3(1.0f, 1.0f, 1.0f));
		fontLoader.renderText(shader, "Red", 0.0f, 0.2f, 1.0f, vec3(1.0f, 0.2f, 0.2f));
		fontLoader.renderText(shader, "Green", 0.0f, -0.2f, 1.0f, vec3(0.2f, 1.0f, 0.2f));
		fontLoader.renderText(shader, "Blue", 0.0f, 0.4f, 1.0f, vec3(0.2f, 0.2f, 1.0f));
		

		//Update window
		window.update();
	}
	return 0;
}