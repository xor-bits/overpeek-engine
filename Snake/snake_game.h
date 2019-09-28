#pragma once

#include <glm/glm.hpp>
#include <vector>


class snake_game
{
private:
	glm::ivec2 direction = glm::vec2(0, 0);

	std::vector<glm::ivec2> snake_pieces;
	glm::ivec2 food = glm::ivec2(0, 0);
	bool running;

private:
	void newFood();
	void reset();
	void gameOver();

public:
	static const glm::vec2 map_size;

public:

	/*
	Setup game
	*/
	snake_game();

	/*
	Setup game with random seed
	*/
	snake_game(int seed);

	/*
	Clean up
	*/
	~snake_game();

	/*
	Returns current score of the game
	*/
	unsigned int getScore() {
		return getSnakePieceCount(); 
	}

	/*
	Check if game was losed
	*/
	bool isGameOver() {
		return !running;
	}
	
	/*
	Basic input for the game
	there are 5 possible keystrokes:
	0 - (No keystroke)
	1 - Left arrow
	2 - Right arrow
	3 - Up arrow
	4 - Down arrow

	returns 0 if something went wrong
	returns -1 if died
	returns 1 if collected food or approached food
	*/
	int input(unsigned int key);

	//get direction
	glm::ivec2 getHeading() { return direction; }

	//get food position
	glm::ivec2 getFood() { return food; }

	//get all snake pieces
	glm::ivec2* getSnakePieces() { return &snake_pieces[0]; }

	//get snake head piece
	glm::ivec2 getSnakeHead() { return snake_pieces[0]; }

	//Get snake piece count
	unsigned int getSnakePieceCount() { return snake_pieces.size(); }

	//get offset to food from head
	float* getData2() {
		float* data = new float[2];
		data[0] = food.x - snake_pieces[0].y;
		data[1] = food.y - snake_pieces[0].y;
		return data;
	}

	//get distance to food from head
	float getData2Dist() {
		float data = abs(food.x - snake_pieces[0].y);
		data += abs(food.y - snake_pieces[0].y);
		return data;
	}

	/*
	Returns 2d array of all tile data

	0 is empty tile
	1 is snake tile
	2 is snake head tile
	3 is food tile
	*/
	float* getData();

};

