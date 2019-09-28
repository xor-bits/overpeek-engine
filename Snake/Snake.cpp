#include "../overpeek-engine/engine.h"

#include "snake_game.h"

oe::Window* window;
snake_game* snake;

const glm::vec2 window_size = glm::vec2(600, 600);
const glm::vec2 tile_size = glm::vec2(2.0f / snake_game::map_size.x, 2.0f / snake_game::map_size.y);

void key(int key, int action) {
	if (key == OE_KEY_LEFT && action == OE_PRESS)		{ snake->input(1); }
	else if (key == OE_KEY_RIGHT && action == OE_PRESS) { snake->input(2); }
	else if (key == OE_KEY_UP && action == OE_PRESS)    { snake->input(3); }
	else if (key == OE_KEY_DOWN && action == OE_PRESS)  { snake->input(4); }
	else if (action == OE_PRESS)						{ snake->input(0); }
}

void render(float fix) {
	window->clear();
	window->input();

	for (int i = 1; i < snake->getSnakePieceCount(); i++)
	{
		
		window->getRenderer()->quadRenderer->submitQuad(glm::vec3(snake->getSnakePieces()[i].x * tile_size.x - 1.0f, snake->getSnakePieces()[i].y * tile_size.y - 1.0f, 0.0f), tile_size, NULL, OE_COLOR_GREEN);
	}
	window->getRenderer()->quadRenderer->submitQuad(glm::vec3(snake->getSnakeHead().x * tile_size.x - 1.0f, snake->getSnakeHead().y * tile_size.y - 1.0f, 0.0f), tile_size, NULL, OE_COLOR_DARKGREEN);
	
	window->getRenderer()->quadRenderer->submitQuad(glm::vec3(snake->getFood().x * tile_size.x - 1.0f, snake->getFood().y * tile_size.y - 1.0f, 0.0f), tile_size, NULL, OE_COLOR_RED);

	window->getRenderer()->draw(NULL, NULL);
	window->update();
}

void update() {
	//Nothing
}

int main()
{
	//window creation
	window = new oe::Window(600, 600, "SNAKE", WINDOW_MULTISAMPLE_X2);
	window->createRenderer(nullptr);
	window->createGameLoop(render, update, 4);
	window->setKeyboardCallback(key);

	snake = new snake_game();

	//gameloop
	window->getGameLoop()->start();
}