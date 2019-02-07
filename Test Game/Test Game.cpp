// Test Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

void render();
void update();
void rapid();

graphics::Window *window = new graphics::Window(900, 600, "Engine tester", false, 0);
graphics::Shader *shader = new graphics::Shader("shaders/texture.vert.glsl", "shaders/texture.frag.glsl");
graphics::Renderer *renderer = new graphics::Renderer("resources/arial.ttf", window);
logic::GameLoop *loop = new logic::GameLoop(render, update, rapid, 20, 10000);


void render() {
	//std::string text = "0";
	//
	//for (int i = 1; i < 70; i++)
	//{
	//	text.append(std::string(", ") + std::to_string(i));
	//}
	//
	//
	//for (float y = -1.0; y < 0.5; y += 0.01)
	//{
	//	renderer->renderText(glm::vec2(0.0, y), glm::vec2(0.01), text, glm::vec4(1.0), TEXT_ORIGIN_CENTER, TEXT_ORIGIN_CENTER);
	//}

	for (int x = -400; x < 100; x++)
	{
		for (int y = -25; y < 25; y++)
		{
			renderer->renderBox(glm::vec2(x / 500.0f, y / 500.0f), glm::vec2(1.0f / 500.0f), 0, glm::vec4(1.0));
		}
	}

	renderer->draw(shader, "unif_text", graphics::TextureManager::getTexture(0));

	window->checkErrors();
	window->update();
	window->input();
	window->clear();
}

void update() {
	if (window->getButton(GLFW_KEY_ESCAPE)) loop->stop();
}


void rapid() {

}

int main()
{
	shader->enable();
	glm::mat4 pr = glm::ortho(-1.0f, 1.0f, 1.0f, -1.0f);
	shader->SetUniformMat4("pr_matrix", pr);
	graphics::TextureManager::loadTextureAtlas("resources/atlas.png", GL_RGBA, 0);
	loop->start();

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
