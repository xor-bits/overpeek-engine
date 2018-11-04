#include "game.h"

#include "../creatures/player.h"
#include "../world/region.h"
#include "../logic/inventory.h"

#include <windows.h>
#include <algorithm>

graphics::Window *Game::m_window;
graphics::Shader *Game::m_shader;
graphics::Renderer *Game::m_renderer;
logic::GameLoop *Game::m_loop;
glm::ivec2 Game::m_hover_tile;
Inventory *m_inventory;

Region *Game::m_region[RENDER_DST * 2][RENDER_DST * 2];
Player *Game::m_player;

float Game::lastRegionX = 0;
float Game::lastRegionY = 0;
bool Game::debugMode = false;

int Game::hitCooldown = 513709;
bool holdingI = false;

void Game::init(graphics::Shader *shader, graphics::Window * window, logic::GameLoop *loop) {
	m_shader = shader; m_window = window; m_loop = loop;

	//Initializing
	system(("mkdir \"" + SAVE_PATH + WORLD_NAME + "\\regions\"").c_str());
	logic::Noise::seed(0);//tools::Clock::getMicroseconds());
	tools::Random::seed(0);

	m_renderer = new graphics::Renderer("arial.ttf");

	audio::AudioManager::init();
	audio::AudioManager::loadAudio("recourses/hit.wav", 0);
	audio::AudioManager::loadAudio("recourses/swing.wav", 1);
	audio::AudioManager::loadAudio("recourses/collect.wav", 2);

	graphics::TextureManager n;
	n.loadTexture("recourses/atlas.png", GL_RGBA, 0);
	Database::init();

	m_inventory = new Inventory(m_shader, m_window);

	//Instantiating
	for (int x = 0; x < RENDER_DST; x++)
	{
		for (int y = 0; y < RENDER_DST; y++)
		{
			m_region[x][y] = new Region(x, y);
		}
	}
	m_player = new Player(-4, 0.0, m_inventory);


	m_shader->enable();
	m_shader->SetUniformMat4("ml_matrix", glm::mat4(1.0f));
}

void Game::renderInfoScreen() {
	if (!debugMode) return;
	m_shader->enable();
	m_shader->setUniform1i("unif_text", 1);

	std::string text = "FPS: " + std::to_string(m_loop->getFPS());
	m_renderer->renderText(-m_window->getAspect(), -1.0, 0.1, 0.1, text, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);
	text = "UPS: " + std::to_string(m_loop->getUPS());
	m_renderer->renderText(-m_window->getAspect(), -0.9, 0.1, 0.1, text, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);

	text = "Position X: " + std::to_string(m_player->x) + ", Y: " + std::to_string(m_player->y);
	m_renderer->renderText(-m_window->getAspect(), -0.8, 0.1, 0.1, text, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);
	text = "Region X: " + std::to_string(posToRegionPos(m_player->x)) + ", Y: " + std::to_string(posToRegionPos(m_player->y));
	m_renderer->renderText(-m_window->getAspect(), -0.7, 0.1, 0.1, text, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);
}

void Game::render() {
	m_renderer->clear();
	for (int x = 0; x < RENDER_DST; x++)
	{
		for (int y = 0; y < RENDER_DST; y++)
		{
			if (m_region[x][y]) m_region[x][y]->submitToRenderer(m_renderer, -m_player->x, -m_player->y);
		}
	}
	for (int x = 0; x < RENDER_DST; x++)
	{
		for (int y = 0; y < RENDER_DST; y++)
		{
			if (m_region[x][y]) m_region[x][y]->submitCreaturesToRenderer(m_renderer, -m_player->x, -m_player->y);
		}
	}
	m_player->submitToRenderer(m_renderer, -m_player->x, -m_player->y);
	m_inventory->render(m_renderer);
	renderInfoScreen();

	m_renderer->flush(m_shader, 0);
}

double clamp(double x, double upper, double lower)
{
	return min(upper, max(x, lower));
}

void Game::update() {
	//Player movement
	float playerSpeed = 0.01;
	if (m_window->getKey(GLFW_KEY_LEFT_SHIFT)) playerSpeed = 0.02;
	else if (m_window->getKey(GLFW_KEY_LEFT_CONTROL)) playerSpeed = 0.002;
	if (m_window->getKey(GLFW_KEY_S)) { m_player->acc_y += playerSpeed; }
	if (m_window->getKey(GLFW_KEY_D)) { m_player->acc_x += playerSpeed; }
	if (m_window->getKey(GLFW_KEY_W)) { m_player->acc_y += -playerSpeed; }
	if (m_window->getKey(GLFW_KEY_A)) { m_player->acc_x += -playerSpeed; }

	//m_enemy->hit();
	m_player->update();

	processNewArea();
	for (int x = 0; x < RENDER_DST; x++)
	{
		for (int y = 0; y < RENDER_DST; y++)
		{
			if (m_region[x][y]) m_region[x][y]->update();
		}
	}
	
	m_inventory->update();

	for (int x = 0; x < RENDER_DST; x++)
	{
		for (int y = 0; y < RENDER_DST; y++)
		{
			if (!m_region[x][y]) m_region[x][y] = new Region(x + m_player->getRegionX(), y + m_player->getRegionY());
		}
	}

	if (!debugMode) return;

	std::cout << "Tile" << std::endl;
	std::cout << " " << m_player->x << ", ";
	std::cout << " " << m_player->y << ", ";
	std::cout << std::endl;
	std::cout << " " << getTile(m_player->x, m_player->y)->getX() << ", ";
	std::cout << " " << getTile(m_player->x, m_player->y)->getY() << ", ";
	std::cout << std::endl;

}

void Game::rapidUpdate() {
	//for (int x = 0; x < RENDER_DST; x++)
	//{
	//	for (int y = 0; y < RENDER_DST; y++)
	//	{
	//		if (!m_region[x][y]) m_region[x][y] = new Region(x + m_player->getRegionX(), y + m_player->getRegionY());
	//	}
	//}
}

void Game::keyPress(int key) {
	//Player Hitting
	if (key == GLFW_KEY_UP) { m_player->heading = HEADING_UP; m_player->hit(); return; }
	if (key == GLFW_KEY_DOWN) { m_player->heading = HEADING_DOWN; m_player->hit(); return; }
	if (key == GLFW_KEY_LEFT) { m_player->heading = HEADING_LEFT; m_player->hit(); return; }
	if (key == GLFW_KEY_RIGHT) { m_player->heading = HEADING_RIGHT; m_player->hit(); return; }

	//if (key == GLFW_KEY_E) { m_player->hit(); return; }
	//if (key == GLFW_KEY_Q) { m_player->place(); return; }

	//Inventory
	if (key == GLFW_KEY_R) { m_inventory->visible = !m_inventory->visible; return; }
	if (key == GLFW_KEY_ESCAPE) { m_inventory->visible = false; return; }

	//Inventory slot selecting
	if (key == GLFW_KEY_1) { m_inventory->selectedSlot = 0; return; }
	else if (key == GLFW_KEY_2) { m_inventory->selectedSlot = 1; return; }
	else if (key == GLFW_KEY_3) { m_inventory->selectedSlot = 2; return; }
	else if (key == GLFW_KEY_4) { m_inventory->selectedSlot = 3; return; }
	else if (key == GLFW_KEY_5) { m_inventory->selectedSlot = 4; return; }

	if (key == GLFW_KEY_F1) { debugMode = !debugMode; return; }

}

void Game::buttonPress(int button) {}

void Game::scroll(double y) {
	//Inventory slot scrolling
	if (y < 0) m_inventory->selectedSlot++;
	if (y > 0) m_inventory->selectedSlot--;
}

void Game::close() {
	for (int x = 0; x < RENDER_DST; x++) {
		for (int y = 0; y < RENDER_DST; y++) {
			if (m_region[x][y]) m_region[x][y]->saveTiles();
		}
	}
}





//long long startTime = tools::Clock::getMicroseconds();
//std::cout << "ms: " << (tools::Clock::getMicroseconds() - startTime) / 1000.0 << std::endl;
/*

NOTHING GOOD TO EDIT AFTER THIS
ALL JUST SOME GETTERS AND STUFF

*/





void Game::processNewArea() {
	if ((int)m_player->getRegionX() > (int)lastRegionX) {
		for (int y = 0; y < RENDER_DST; y++) m_region[0][y]->saveTiles();
		for (int x = 0; x < RENDER_DST - 1; x++) {
			for (int y = 0; y < RENDER_DST; y++) m_region[x][y] = m_region[x + 1][y];
		}
		for (int y = 0; y < RENDER_DST; y++) m_region[RENDER_DST - 1][y] = new Region(RENDER_DST - 1 + (int)m_player->getRegionX(), y + (int)m_player->getRegionY());
	}
	else if ((int)m_player->getRegionX() < (int)lastRegionX) {
		for (int y = 0; y < RENDER_DST; y++) m_region[RENDER_DST - 1][y]->saveTiles();
		for (int x = RENDER_DST - 1; x > 0; x--) {
			for (int y = 0; y < RENDER_DST; y++) m_region[x][y] = m_region[x - 1][y];
		}
		for (int y = 0; y < RENDER_DST; y++) m_region[0][y] = new Region(0 + (int)m_player->getRegionX(), y + (int)m_player->getRegionY());
	}
	if ((int)m_player->getRegionY() > (int)lastRegionY) {
		for (int x = 0; x < RENDER_DST; x++) m_region[x][0]->saveTiles();
		for (int y = 0; y < RENDER_DST - 1; y++) {
			for (int x = 0; x < RENDER_DST; x++) m_region[x][y] = m_region[x][y + 1];
		}
		for (int x = 0; x < RENDER_DST; x++) m_region[x][RENDER_DST - 1] = new Region(x + (int)m_player->getRegionX(), RENDER_DST - 1 + (int)m_player->getRegionY());
	}
	else if ((int)m_player->getRegionY() < (int)lastRegionY) {
		for (int x = 0; x < RENDER_DST; x++) m_region[x][RENDER_DST - 1]->saveTiles();
		for (int y = RENDER_DST - 1; y > 0; y--) {
			for (int x = 0; x < RENDER_DST; x++) m_region[x][y] = m_region[x][y - 1];
		}
		for (int x = 0; x < RENDER_DST; x++) m_region[x][0] = new Region(x + (int)m_player->getRegionX(), 0 + (int)m_player->getRegionY());
	}

	lastRegionX = m_player->getRegionX();
	lastRegionY = m_player->getRegionY();
}

int Game::screenToWorldX(float x) {
	if (x / TILE_SIZE + m_player->x >= 0) return x / TILE_SIZE + m_player->x;
	else return x / TILE_SIZE - 1 + m_player->x;
}
int Game::screenToWorldY(float y) {
	if (y / TILE_SIZE + m_player->y >= 0) return y / TILE_SIZE + m_player->y;
	else return y / TILE_SIZE - 1 + m_player->y;
}


int Game::posToRegionPos(float xOrY) {
	if (xOrY >= 0) return xOrY / REGION_SIZE + 0.5;
	return xOrY / REGION_SIZE - 0.5;
}

Region* Game::getRegion(float x, float y) {
	int cursorRegionX = posToRegionPos(x);
	int cursorRegionY = posToRegionPos(y);
	int playerRegionX = m_player->getRegionX();
	int playerRegionY = m_player->getRegionY();

	int finalRegionX = cursorRegionX - playerRegionX + ceil(RENDER_DST / 2);
	int finalRegionY = cursorRegionY - playerRegionY + ceil(RENDER_DST / 2);
	finalRegionX = clamp(finalRegionX, RENDER_DST - 1, 0);
	finalRegionY = clamp(finalRegionY, RENDER_DST - 1, 0);

	return m_region[finalRegionX][finalRegionY];
}

Tile* Game::getTile(float x, float y) {
	x += 0.00001; y += 0.00001;
	Region *regionAt = getRegion(x, y);
	if (!regionAt) return nullptr;

	int finalX = x + REGION_SIZE / 2.0 - regionAt->getX();
	int finalY = y + REGION_SIZE / 2.0 - regionAt->getY();
	finalX = clamp(finalX, REGION_SIZE - 1, 0);
	finalY = clamp(finalY, REGION_SIZE - 1, 0);

	Tile *tile = regionAt->getTile(finalX, finalY);
	if (!tile) return nullptr;
	if (tile->getObjectId() > COUNT_OBJECTS || tile->getObjectId() < 0) {
		std::cout << "getTile error at " << finalX << ", " << finalX;
		throw;
		return nullptr;
	}

	//DIRTY FIX, REPAIR NEEDED
	if (tile->getX() != floor(x) || tile->getY() != floor(y)) {
		return nullptr;
	}

	return tile;
}

Player *Game::getPlayer() {
	return m_player;
}

bool Game::trySetTileObject(float x, float y, int id) {
	Tile *tile = getTile(x, y);
	if (!tile) return false;
	if (tile->getObjectId() != 5) return false;
	tile->setObjectId(id);
	return true;
}

bool Game::trySetTileObject(Tile *tile, int id) {
	std::cout << "Set to " << id << std::endl;
	if (!tile) return false;
	std::cout << "Set to " << id << std::endl;
	if (tile->getObjectId() == 5) return false;
	tile->setObjectId(id);
	std::cout << "Set to " << id << std::endl;
	return true;
}