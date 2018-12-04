#include "game.h"

#include "../creatures/player.h"
#include "../world/region.h"
#include "../logic/inventory.h"

#include <windows.h>
#include <math.h>

graphics::Window *Game::m_window;
graphics::Shader *Game::m_shader;
graphics::Shader *Game::m_guishader;
graphics::Renderer *Game::m_worldrenderer;
graphics::Renderer *Game::m_creaturerenderer;
graphics::Renderer *Game::m_guirenderer;
logic::GameLoop *Game::m_loop;
glm::ivec2 Game::m_hover_tile;
Inventory *m_inventory;

Region *Game::m_region[RENDER_DST][RENDER_DST];
Player *Game::m_player;

float Game::lastRegionX = 0;
float Game::lastRegionY = 0;
bool Game::debugMode = false;

int Game::hitCooldown = 513709;
bool holdingI = false;

bool paused = false;

void Game::init(graphics::Shader *shader, graphics::Window * window, logic::GameLoop *loop) {
	m_shader = shader; m_window = window; m_loop = loop;

	//Initializing
	system(("mkdir \"" + SAVE_PATH + WORLD_NAME + "\\regions\"").c_str());
	logic::Noise::seed(0); //tools::Clock::getMicroseconds());
	tools::Random::seed(0);

	m_worldrenderer = new graphics::Renderer();
	m_creaturerenderer = new graphics::Renderer();
	m_guirenderer = new graphics::Renderer("arial.ttf");

	audio::AudioManager::init();
	audio::AudioManager::loadAudio("recourses/hit.wav", 0);
	audio::AudioManager::loadAudio("recourses/swing.wav", 1);
	audio::AudioManager::loadAudio("recourses/collect.wav", 2);

	graphics::TextureManager::loadTextureAtlas("recourses/atlas.png", GL_RGBA, 0);
	Database::init();

	m_inventory = new Inventory(m_shader, m_window);

	//Constructing objects
	for (int x = 0; x < RENDER_DST; x++)
	{
		for (int y = 0; y < RENDER_DST; y++)
		{
			m_region[x][y] = new Region(x, y);
		}
	}
	m_player = new Player(0, 0.0, m_inventory);

	m_region[0][0]->addCreature(0, 0, 1, false);


	m_shader->enable();
	m_shader->SetUniformMat4("ml_matrix", glm::mat4(1.0f));
}

void Game::renderInfoScreen() {
	if (!debugMode) return;
	m_shader->enable();
	m_shader->setUniform1i("unif_text", 1);

	std::string text = "FPS: " + std::to_string(m_loop->getFPS());
	m_guirenderer->renderText(-m_window->getAspect(), -1.0, 0.1, 0.1, 0, text, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);
	text = "UPS: " + std::to_string(m_loop->getUPS());
	m_guirenderer->renderText(-m_window->getAspect(), -0.9, 0.1, 0.1, 0, text, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);

	text = "Position X: " + std::to_string(m_player->getX()) + ", Y: " + std::to_string(m_player->getY());
	m_guirenderer->renderText(-m_window->getAspect(), -0.8, 0.1, 0.1, 0, text, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);
	text = "Region X: " + std::to_string(posToRegionPos(m_player->getX())) + ", Y: " + std::to_string(posToRegionPos(m_player->getY()));
	m_guirenderer->renderText(-m_window->getAspect(), -0.7, 0.1, 0.1, 0, text, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);
	text = "Region pos X: " + std::to_string(getRegion(m_player->getX(), m_player->getY())->getX()) + ", Y: " + std::to_string(getRegion(m_player->getX(), m_player->getY())->getY());
	m_guirenderer->renderText(-m_window->getAspect(), -0.6, 0.1, 0.1, 0, text, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);
}

void Game::render() {
	//World
	m_worldrenderer->clear();
	for (int x = 0; x < RENDER_DST; x++)
	{
		for (int y = 0; y < RENDER_DST; y++)
		{
			if (m_region[x][y]) m_region[x][y]->submitToRenderer(m_worldrenderer, -m_player->x, -m_player->y);
		}
	}
	m_worldrenderer->flush(m_shader, 0);


	//Creature
	m_creaturerenderer->clear();
	for (int x = 0; x < RENDER_DST; x++)
	{
		for (int y = 0; y < RENDER_DST; y++)
		{
			if (m_region[x][y]) m_region[x][y]->submitCreaturesToRenderer(m_creaturerenderer, -m_player->x, -m_player->y);
		}
	}
	m_player->submitToRenderer(m_creaturerenderer, -m_player->x, -m_player->y);
	m_creaturerenderer->flush(m_shader, 0);


	//Gui
	m_guirenderer->clear();
	renderInfoScreen();
	m_inventory->render(m_guirenderer);
	m_guirenderer->flush(m_shader, 0);
}

double clamp(double x, double upper, double lower)
{
	return std::min(upper, std::max(x, lower));
}

void Game::update() {
	if (!paused) {

		//Player movement
		float playerSpeed = 0.01;
		if (m_window->getKey(GLFW_KEY_LEFT_SHIFT)) playerSpeed = 0.02;
		else if (m_window->getKey(GLFW_KEY_LEFT_CONTROL)) playerSpeed = 0.002;
		if (m_window->getKey(GLFW_KEY_S)) { m_player->acc_y += playerSpeed; }
		if (m_window->getKey(GLFW_KEY_D)) { m_player->acc_x += playerSpeed; }
		if (m_window->getKey(GLFW_KEY_W)) { m_player->acc_y += -playerSpeed; }
		if (m_window->getKey(GLFW_KEY_A)) { m_player->acc_x += -playerSpeed; }
		m_player->update();
		m_inventory->update();

		//Load regions after camera/player moves
		processNewArea();

		m_player->collide();

		//World updates
		for (int x = 0; x < RENDER_DST; x++)
		{
			for (int y = 0; y < RENDER_DST; y++)
			{
				if (m_region[x][y]) m_region[x][y]->update();
				else m_region[x][y] = new Region(x + posToRegionPos(m_player->x), y + posToRegionPos(m_player->y));
			}
		}

		long long startTime = tools::Clock::getMicroseconds();
		tools::Random::hash(m_player->getX(), m_player->getY());
		double timeTook = (tools::Clock::getMicroseconds() - startTime) / 1000.0;
		tools::Logger::info(timeTook);

	}

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
	
	if (key == GLFW_KEY_P) { paused = !paused; return; }
	if (key == GLFW_KEY_E) { m_player->setX(round(m_player->getX())); m_player->setY(round(m_player->getY())); return; }

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

	if (key == GLFW_KEY_F2) {
		for (int i = 0; i < RENDER_DST; i++) {
			for (int j = 0; j < RENDER_DST; j++) {
				// (m_region[i][j]) m_region[i][j]->debugCeilCreatures();
			}
		}
	}

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

//Free to edit; world generating noise
#define NOISE_SCALE 0.02

#define LEVEL_WATER 0.5
#define LEVEL_SAND 0.52
#define LEVEL_SOIL 0.7
#define LEVEL_STONE 0.72
void Game::getInfoFromNoise(int &tileId, int &objId, double x, double y) {
	x *= NOISE_SCALE;
	y *= NOISE_SCALE;
	float height = (logic::Noise::octaveNoise(x, y, 3) + 1.0) / 2.0;
	int id = 0;
	int object_id = 0;
	
	//height is from 0 to 1
	if (height <= LEVEL_WATER) id = 1;
	else if (height <= LEVEL_SAND) id = 2;
	else if (height <= LEVEL_SOIL) {
		float n = (logic::Noise::octaveNoise(x * 3.0, y * 3.0, 3) + 1.0) / 2.0;
		if (n > 0.7 || n < 0.3) id = 3;
		else  id = 0;
	
		if (n > 0.8) object_id = 2;
		else if (n < 0.2) object_id = 3;
		else if (n > 0.7 || n < 0.3) {
			if (tools::Random::random(0.0, 1.0) > 0.5) object_id = 1;
		}
	}
	else if (height <= LEVEL_STONE) id = 4;
	else { id = 4; object_id = 5; }
	
	tileId = id;
	objId = object_id;
}





//long long startTime = tools::Clock::getMicroseconds();
//std::cout << "ms: " << (tools::Clock::getMicroseconds() - startTime) / 1000.0 << std::endl;
///	 _____   _____   _____   _____   _______   _   _     _	 _____ 
///	/  _  \ /  _  \ /  _  \ /  _  \ |  ___  \ | | | |   | |	/  _  \
///	| | | | | | | | | | | | | | | | | |___| | | | |  \  | |	| | |_|
///	| |_| | | | | | | | | | | | | | |    ___/ | | |   \ | |	| |___
///	|  _ <	| | | | | | | | | | | | | |\ \    | | | |\ \| |	| |\  |
///	| | | | | | | | | | | | | | | | | | \ \   | | | | \   |	| | | |
///	| |_| | | |_| | | |_| | | |_| | | |  \ \  | | | |  \  |	| |_| |
///	\_____/ \_____/ \_____/ \_____/ |_|   \_| |_| |_|   |_| \_____|
///
///
///
///
///	NOTHING GOOD TO
///	EDIT AFTER THIS
///	ALL JUST SOME
///	GETTERS AND STUFF
///
///
///




void Game::processNewArea() {
	float playerX = m_player->getX();
	float playerY = m_player->getY();

	int playerRegionX = posToRegionPos(playerX - REGION_SIZE / 2.0) + 1;
	int playerRegionY = posToRegionPos(playerY - REGION_SIZE / 2.0) + 1;

	if (playerRegionX > lastRegionX) {
		for (int y = 0; y < RENDER_DST; y++) m_region[0][y]->saveTiles();
		for (int x = 0; x < RENDER_DST - 1; x++) {
			for (int y = 0; y < RENDER_DST; y++) m_region[x][y] = m_region[x + 1][y];
		}
		for (int y = 0; y < RENDER_DST; y++) m_region[RENDER_DST - 1][y] = new Region(RENDER_DST - 1 + playerRegionX, y + playerRegionY);
	}
	else if (playerRegionX < lastRegionX) {
		for (int y = 0; y < RENDER_DST; y++) m_region[RENDER_DST - 1][y]->saveTiles();
		for (int x = RENDER_DST - 1; x > 0; x--) {
			for (int y = 0; y < RENDER_DST; y++) m_region[x][y] = m_region[x - 1][y];
		}
		for (int y = 0; y < RENDER_DST; y++) m_region[0][y] = new Region(0 + playerRegionX, y + playerRegionY);
	}
	if (playerRegionY > lastRegionY) {
		for (int x = 0; x < RENDER_DST; x++) m_region[x][0]->saveTiles();
		for (int y = 0; y < RENDER_DST - 1; y++) {
			for (int x = 0; x < RENDER_DST; x++) m_region[x][y] = m_region[x][y + 1];
		}
		for (int x = 0; x < RENDER_DST; x++) m_region[x][RENDER_DST - 1] = new Region(x + playerRegionX, RENDER_DST - 1 + playerRegionY);
	}
	else if (playerRegionY < lastRegionY) {
		for (int x = 0; x < RENDER_DST; x++) m_region[x][RENDER_DST - 1]->saveTiles();
		for (int y = RENDER_DST - 1; y > 0; y--) {
			for (int x = 0; x < RENDER_DST; x++) m_region[x][y] = m_region[x][y - 1];
		}
		for (int x = 0; x < RENDER_DST; x++) m_region[x][0] = new Region(x + playerRegionX, 0 + playerRegionY);
	}

	lastRegionX = playerRegionX;
	lastRegionY = playerRegionY;
}

int Game::screenToWorldX(float x) {
	if (x / TILE_SIZE + m_player->getX() >= 0) return x / TILE_SIZE + m_player->getX();
	else return x / TILE_SIZE - 1 + m_player->getX();
}
int Game::screenToWorldY(float y) {
	if (y / TILE_SIZE + m_player->getY() >= 0) return y / TILE_SIZE + m_player->getY();
	else return y / TILE_SIZE - 1 + m_player->getY();
}


float Game::posToRegionPos(float xOrY) {
	return floor((xOrY + 0.5) / (float)REGION_SIZE);
}

Region* Game::getRegion(float x, float y) {
	x = round(x); y = round(y);

	int selectedRegionX = posToRegionPos(x);
	int selectedRegionY = posToRegionPos(y);

	int playerRegionX = posToRegionPos(round(m_player->getX()));
	int playerRegionY = posToRegionPos(round(m_player->getY()));

	int offsetRegionX = posToRegionPos(round(m_player->getX())) - posToRegionPos(round(m_player->getX() - REGION_SIZE / 2.0)) + 1;
	int offsetRegionY = posToRegionPos(round(m_player->getY())) - posToRegionPos(round(m_player->getY() - REGION_SIZE / 2.0)) + 1;

	int finalRegionX = selectedRegionX - playerRegionX + offsetRegionX;
	int finalRegionY = selectedRegionY - playerRegionY + offsetRegionY;

	if (finalRegionX < 0 || finalRegionX > RENDER_DST - 1 || finalRegionY < 0 || finalRegionY > RENDER_DST - 1) return nullptr;

	return m_region[finalRegionX][finalRegionY];
}

Tile* Game::getTile(float x, float y) {
	x = round(x); y = round(y);

	Region *regionAt = getRegion(x, y);
	if (!regionAt) {
		//tools::Logger::warning(std::string("Region out of viewdistance!"));
		return nullptr;
	}

	int finalX = x - regionAt->getX();
	int finalY = y - regionAt->getY();
	finalX = clamp(finalX, REGION_SIZE - 1, 0);
	finalY = clamp(finalY, REGION_SIZE - 1, 0);

	Tile *tile = regionAt->getTile(finalX, finalY);
	if (!tile) {
		tools::Logger::error(std::string("region returned nullptr instead of tile"));
		return nullptr;
	}
	if (tile->getObjectId() > COUNT_OBJECTS || tile->getObjectId() < 0) {
		tools::Logger::error(std::string("getTile error at ") + std::to_string(finalX) + std::string(", ") + std::to_string(finalY));
		return nullptr;
	}

	if (tile->getX() != x || tile->getY() != y) {
		int selectedRegionX = posToRegionPos(x);
		int selectedRegionY = posToRegionPos(y);

		int playerRegionX = posToRegionPos(round(m_player->getX()));
		int playerRegionY = posToRegionPos(round(m_player->getY()));

		int offsetRegionX = posToRegionPos(round(m_player->getX())) - posToRegionPos(round(m_player->getX() - REGION_SIZE / 2.0)) + 1;
		int offsetRegionY = posToRegionPos(round(m_player->getY())) - posToRegionPos(round(m_player->getY() - REGION_SIZE / 2.0)) + 1;

		int finalRegionX = selectedRegionX - playerRegionX + offsetRegionX;
		int finalRegionY = selectedRegionY - playerRegionY + offsetRegionY;

		if (finalRegionX < 0 || finalRegionX > RENDER_DST - 1 || finalRegionY < 0 || finalRegionY > RENDER_DST - 1) return nullptr;

		Region *tmp = m_region[finalRegionX][finalRegionY];

		tools::Logger::info("Ending results not as expected");
		tools::Logger::info(std::string("selectedRegion ") + std::to_string(selectedRegionX) + std::string(", ") + std::to_string(selectedRegionY));
		tools::Logger::info(std::string("playerRegion ") + std::to_string(playerRegionX) + std::string(", ") + std::to_string(playerRegionY));
		tools::Logger::info(std::string("offsetRegion ") + std::to_string(offsetRegionX) + std::string(", ") + std::to_string(offsetRegionY));
		tools::Logger::info(std::string("finalRegion ") + std::to_string(finalRegionX) + std::string(", ") + std::to_string(finalRegionY));
		tools::Logger::info(std::string("Region1 ") + std::to_string(tmp->getX()) + std::string(", ") + std::to_string(tmp->getY()));
		tools::Logger::info(std::string("Region2 ") + std::to_string(regionAt->getX()) + std::string(", ") + std::to_string(regionAt->getY()));
		tools::Logger::info(std::string("playerPos ") + std::to_string(round(m_player->getX())) + std::string(", ") + std::to_string(round(m_player->getY())));
		tools::Logger::info(std::string("cursorPos ") + std::to_string(x) + std::string(", ") + std::to_string(y));

		tools::Logger::info(std::string("FinalXY ") + std::to_string(finalX) + std::string(", ") + std::to_string(finalY));
		tools::Logger::info(std::string("tile getXY ") + std::to_string(tile->getX()) + std::string(", ") + std::to_string(tile->getY()));
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