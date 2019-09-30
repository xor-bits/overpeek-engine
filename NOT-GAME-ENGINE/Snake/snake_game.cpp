#include "snake_game.h"

#include <ctime>
#include <iostream>


const glm::vec2 snake_game::map_size = glm::vec2(10, 10);


snake_game::snake_game()
{
	reset();
}

snake_game::snake_game(int seed)
{
	srand(seed);
	reset();
}

snake_game::~snake_game()
{
	//std::cout << std::endl << std::endl << std::endl;
}

void snake_game::newFood() {
	food = glm::vec2(rand() % int(map_size.x), rand() % int(map_size.y));

	for (int i = 0; i < snake_pieces.size(); i++)
	{
		if (food == snake_pieces[i]) {
			newFood();
		}
	}
}

void snake_game::reset() {
	snake_pieces = std::vector<glm::ivec2>();
	direction = glm::vec2(0, 0);
	snake_pieces.push_back(glm::vec2(rand() % int(map_size.x), rand() % int(map_size.y)));
	newFood();
	running = true;
}

void snake_game::gameOver() {
	running = false;
}

int snake_game::input(unsigned int key) {
	float distance_change = getData2Dist();
	int feedback = 0;
	glm::ivec2 tmp = direction;


	if (key == 0) { /* NO DIRECTION CHANGE */ }
	else if (key == 1) { direction = glm::ivec2(-1, 0); }
	else if (key == 2) { direction = glm::ivec2(1, 0); }
	else if (key == 3) { direction = glm::ivec2(0, -1); }
	else if (key == 4) { direction = glm::ivec2(0, 1); }
	else { return 0; /* UNNKOWN KEY */ }
	
	///std::cout << key << " : ";
	///std::cout << direction.x << "," << direction.y << " : "<< tmp.x << "," << tmp.y << -tmp.x << "," << -tmp.y;
	if (direction == -tmp) {
		//std::cout << " : REVERSED!";
		gameOver();
		feedback -= 50;
		return feedback;
	}
	//std::cout << std::endl;


	for (int i = snake_pieces.size() - 1; i > 0; i--) snake_pieces[i] = snake_pieces[size_t(i - 1)];
	snake_pieces[0].x += direction.x;
	snake_pieces[0].y += direction.y;

	//for (int i = 1; i < snake_pieces.size(); i++)
	//{
	//	if (snake_pieces[0] == snake_pieces[i]) {
	//		gameOver(); 
	//		feedback -= 50;
	//	}
	//}

	if (snake_pieces[0].x >= map_size.x) { gameOver(); feedback -= 50; return feedback; }
	if (snake_pieces[0].x < 0)			 { gameOver(); feedback -= 50; return feedback; }
	if (snake_pieces[0].y >= map_size.y) { gameOver(); feedback -= 50; return feedback; }
	if (snake_pieces[0].y < 0)           { gameOver(); feedback -= 50; return feedback; }

	distance_change = distance_change - getData2Dist();
	//if (distance_change > 0) feedback = 1;
	//if (distance_change < 0) feedback = -1;

	if (snake_pieces[0] == food) {
		newFood();
		snake_pieces.push_back(snake_pieces[getSnakePieceCount() - 1]);
		feedback += 1;
	}

	//std::cout << getFood().x << ", " << getFood().y << std::endl;
	//std::cout << getSnakeHead().x << ", " << getSnakeHead().y << std::endl;


	return feedback;
}

float* snake_game::getData() {
	float* data_array = new float[map_size.x * map_size.y];

	//Empty tiles
	for (int i = 0; i < map_size.x; i++)
	{
		for (int j = 0; j < map_size.y; j++)
		{
			data_array[int(i + j * map_size.x)] = 0;
		}
	}

	//Snake pieces
	for (int i = 0; i < getSnakePieceCount(); i++)
	{
		data_array[int(snake_pieces[i].x + snake_pieces[i].y * map_size.x)] = 1;
	}

	//Snake head
	data_array[int(snake_pieces[0].x + snake_pieces[0].y * map_size.x)] = 2;

	//Food
	data_array[int(food.x + food.y * map_size.x)] = 3;

	return data_array;
}