#include "game.h"

#include "../creatures/player.h"
#include "../world/region.h"
#include "../logic/inventory.h"

#include <Windows.h>

graphics::Window *Game::m_window;
graphics::Shader *Game::m_shader;
graphics::SimpleRenderer *Game::m_renderer;
logic::GameLoop *Game::m_loop;
Tile *Game::m_hover_tile;
Inventory *m_inventory;

Region *Game::m_region[RENDER_DST * 2][RENDER_DST * 2];
Player *Game::m_player;
float Game::lastRegionX = 0;
float Game::lastRegionY = 0;

int Game::hitCooldown = 513709;

bool holdingI = false;

void Game::init(graphics::Shader *shader, graphics::Window * window, logic::GameLoop *loop) {
	m_shader = shader; m_window = window; m_loop = loop;

	system(std::string("mkdir \"" + SAVE_PATH + WORLD_NAME + "\\regions\"").c_str());
	logic::Noise::seed(1);//tools::Clock::getMicroseconds());

	graphics::TextureLoader::loadTexture("recourses/null.png", GL_RGB, 0);
	graphics::TextureLoader::loadTexture("recourses/textures.png", GL_RGBA, 1);

	audio::AudioManager::init();
	audio::AudioManager::loadAudio("recourses/hit.wav", 0);

	for (int x = 0; x < RENDER_DST; x++)
	{
		for (int y = 0; y < RENDER_DST; y++)
		{
			m_region[x][y] = new Region(x, y);
		}
	}
	Region::initRender(m_shader);
	m_player = new Player(0.0, 0.0, m_shader);

	m_inventory = new Inventory(m_shader, m_renderer, m_window);
}

void Game::render() {
	glBindTexture(GL_TEXTURE_2D, graphics::TextureLoader::getTexture(1));
	for (int x = 0; x < RENDER_DST; x++)
	{
		for (int y = 0; y < RENDER_DST; y++)
		{
			if (m_region[x][y] != nullptr) m_region[x][y]->render();
		}
	}
	m_player->render();
	
	if (m_loop) renderDebugScreen(); 
	m_inventory->render();
}

void Game::renderDebugScreen() {

	std::string fpsString = "Fps: "; fpsString += std::to_string(m_loop->getFPS());
	std::string upsString = "Ups: "; upsString += std::to_string(m_loop->getUPS());
	std::string posString = "X: "; posString += std::to_string(m_player->getX());
	posString += ", Y: "; posString += std::to_string(m_player->getY());

	m_renderer->renderText(-m_window->getAspect(), -1.00, 0.05, 0.05, fpsString, glm::vec3(1.0f, 1.0f, 1.0f), TEXT_ALIGN_LEFT, TEXT_ALIGN_BOTTOM);
	m_renderer->renderText(-m_window->getAspect(), -0.95, 0.05, 0.05, upsString, glm::vec3(1.0f, 1.0f, 1.0f), TEXT_ALIGN_LEFT, TEXT_ALIGN_BOTTOM);
	m_renderer->renderText(-m_window->getAspect(), -0.90, 0.05, 0.05, posString, glm::vec3(1.0f, 1.0f, 1.0f), TEXT_ALIGN_LEFT, TEXT_ALIGN_BOTTOM);

	if (m_hover_tile) {
		std::string hoverTile = "Tile X: "; hoverTile += std::to_string(m_hover_tile->getX());
		hoverTile += ", Tile Y: "; hoverTile += std::to_string(m_hover_tile->getY());
		hoverTile += ", Tile ID: "; hoverTile += std::to_string(m_hover_tile->getId());
		hoverTile += ", Tile Object ID: "; hoverTile += std::to_string(m_hover_tile->getObjectId());
		m_renderer->renderText(-m_window->getAspect(), -0.85, 0.05, 0.05, hoverTile, glm::vec3(1.0f, 1.0f, 1.0f), TEXT_ALIGN_LEFT, TEXT_ALIGN_BOTTOM);
	}
}

void Game::update() {
	//Player movement
	float playerSpeed = 0.02;
	if (m_window->getKey(GLFW_KEY_LEFT_SHIFT)) playerSpeed = 10;
	if (m_window->getKey(GLFW_KEY_S)) m_player->setAccY(playerSpeed);
	if (m_window->getKey(GLFW_KEY_D)) m_player->setAccX(playerSpeed);
	if (m_window->getKey(GLFW_KEY_W)) m_player->setAccY(-playerSpeed);
	if (m_window->getKey(GLFW_KEY_A)) m_player->setAccX(-playerSpeed);
	m_player->update();

	//Inventory
	if (m_window->getKey(GLFW_KEY_I)) {
		if (!holdingI) {
			m_inventory->visible = !m_inventory->visible;
		}
		holdingI = true;
	} else holdingI = false;
	if (m_window->getKey(GLFW_KEY_ESCAPE)) m_inventory->visible = false;

	//Move regions in array
	if (m_player->getRegionX() > lastRegionX) {
		for (int y = 0; y < RENDER_DST; y++) delete m_region[0][y];
		for (int x = 0; x < RENDER_DST - 1; x++) {
			for (int y = 0; y < RENDER_DST; y++) m_region[x][y] = m_region[x + 1][y];
		}
		for (int y = 0; y < RENDER_DST; y++) m_region[RENDER_DST - 1][y] = nullptr;
	}
	else if (m_player->getRegionX() < lastRegionX) {
		for (int y = 0; y < RENDER_DST; y++) delete m_region[RENDER_DST - 1][y];
		for (int x = RENDER_DST - 1; x > 0; x--) {
			for (int y = 0; y < RENDER_DST; y++) m_region[x][y] = m_region[x - 1][y];
		}
		for (int y = 0; y < RENDER_DST; y++) m_region[0][y] = nullptr;
	} 
	if (m_player->getRegionY() > lastRegionY) {
		for (int x = 0; x < RENDER_DST; x++) delete m_region[x][0];
		for (int y = 0; y < RENDER_DST - 1; y++) {
			for (int x = 0; x < RENDER_DST; x++) m_region[x][y] = m_region[x][y + 1];
		}
		for (int x = 0; x < RENDER_DST; x++) m_region[x][RENDER_DST - 1] = nullptr;
	}
	else if (m_player->getRegionY() < lastRegionY) {
		for (int x = 0; x < RENDER_DST; x++) delete m_region[x][RENDER_DST - 1];
		for (int y = RENDER_DST - 1; y > 0; y--) {
			for (int x = 0; x < RENDER_DST; x++) m_region[x][y] = m_region[x][y - 1];
		}
		for (int x = 0; x < RENDER_DST; x++) m_region[x][0] = nullptr;
	}

	//Cursor
	float c_x = m_window->getMousePos().x, c_y = m_window->getMousePos().y;
	m_hover_tile = getTile(screenToWorldX(c_x), screenToWorldY(c_y));
	
	//Hitting
	int hitSpeed = 2;
	if (hitCooldown < hitSpeed) hitCooldown++;
	if (m_window->getButton(GLFW_MOUSE_BUTTON_LEFT) && hitCooldown >= hitSpeed) {
<<<<<<< HEAD
		if (m_hover_tile->getObjectId() != 5) {
			hitCooldown = 0;
			audio::AudioManager::play(0);
			if (m_hover_tile->decreaceObjectHealth(0.2f)) {
				m_inventory->addItem(1);
				std::cout << "Wood++\n";
=======
		if (m_hover_tile) {
			if (m_hover_tile->getObjectId() != 0) {
				hitCooldown = 0;
				audio::AudioManager::play(0);
				if (m_hover_tile->decreaceObjectHealth(0.2f)) {
					m_inventory->addItem(1);
					std::cout << "Wood++\n";
				}
>>>>>>> 58354a52ddbe18f20adb822ed668e871de978bb7
			}
		}
	}

<<<<<<< HEAD
	for (int x = 0; x < RENDER_DST; x++)
	{
		for (int y = 0; y < RENDER_DST; y++)
		{
			if (m_region[x][y] != nullptr) m_region[x][y]->update(-m_player->getX() * TILE_SIZE, -m_player->getY() * TILE_SIZE);
=======
	lastRegionX = m_player->getRegionX();
	lastRegionY = m_player->getRegionY();

	//Update regions
	for (int x = 0; x < RENDER_DST; x++) {
		for (int y = 0; y < RENDER_DST; y++) {
			if (m_region[x][y]) m_region[x][y]->update();
>>>>>>> 58354a52ddbe18f20adb822ed668e871de978bb7
		}
	}

	m_inventory->update();
}

void Game::rapidUpdate() {
	for (int x = 0; x < RENDER_DST; x++) {
		for (int y = 0; y < RENDER_DST; y++) {
			if (m_region[x][y] == nullptr) {
				m_region[x][y] = new Region(x + m_player->getRegionX(), y + m_player->getRegionY());
			}
		}
	}
}

void Game::close() {
	for (int x = 0; x < RENDER_DST; x++) {
		for (int y = 0; y < RENDER_DST; y++) {
			if (m_region[x][y] != nullptr) {
				m_region[x][y]->saveTiles();
			}
		}
	}
}

int Game::screenToWorldX(float x) {
	if (x / TILE_SIZE + m_player->getX() >= 0) return x / TILE_SIZE + m_player->getX();
	else return x / TILE_SIZE - 1 + m_player->getX();
}
int Game::screenToWorldY(float y) {
	if (y / TILE_SIZE + m_player->getY() >= 0) return y / TILE_SIZE + m_player->getY();
	else return y / TILE_SIZE - 1 + m_player->getY();
}

Region *Game::getRegion(int x, int y) {
	int cursorRegionX = floor((x / (float)REGION_SIZE) + 0.5);
	int cursorRegionY = floor((y / (float)REGION_SIZE) + 0.5);
	int playerRegionX = floor((m_player->getX() / (float)REGION_SIZE) + 0.5);
	int playerRegionY = floor((m_player->getY() / (float)REGION_SIZE) + 0.5);

	int finalRegionX = cursorRegionX - playerRegionX + ceil(RENDER_DST / 2);
	int finalRegionY = cursorRegionY - playerRegionY + ceil(RENDER_DST / 2);

	if (finalRegionX < 0 || finalRegionX > RENDER_DST) return nullptr;
	if (finalRegionY < 0 || finalRegionY > RENDER_DST) return nullptr;
	if (m_region[finalRegionX][finalRegionY] != nullptr) return m_region[finalRegionX][finalRegionY];
	return nullptr;
}
Tile *Game::getTile(int x, int y) {
	Region *regionAt = getRegion(x, y);

	if (regionAt) {
		int finalX = x + ceil(REGION_SIZE / 2.0) - regionAt->getX();
		int finalY = y + ceil(REGION_SIZE / 2.0) - regionAt->getY();
		if (finalX < 0 || finalX > REGION_SIZE) return nullptr;
		if (finalY < 0 || finalY > REGION_SIZE) return nullptr;
		return regionAt->getTile(finalX, finalY);
	}

	return nullptr;
}