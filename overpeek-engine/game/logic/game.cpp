#include "game.h"

#include "../creatures/player.h"
#include "../world/region.h"
#include "../logic/inventory.h"

#include <windows.h>
#include <math.h>
#include <omp.h>

graphics::Window *Game::m_window;
graphics::Shader *Game::m_shader;
graphics::Shader *Game::m_guishader;
graphics::Renderer *Game::m_worldrenderer;
graphics::Renderer *Game::m_creaturerenderer;
graphics::Renderer *Game::m_guirenderer;
logic::GameLoop *Game::m_loop;
glm::ivec2 Game::m_hover_tile;
Inventory *m_inventory;

Region Game::m_region[RENDER_DST][RENDER_DST];
Player *Game::m_player;

float Game::lastRegionX = 0;
float Game::lastRegionY = 0;
bool Game::debugMode = false;
bool Game::tilesChanged = true;

int Game::hitCooldown = 513709;
bool holdingI = false;

bool paused = false;

void Game::init(graphics::Shader *shader, graphics::Window * window, logic::GameLoop *loop) {
	m_shader = shader; m_window = window; m_loop = loop;

	//Initializing
	tools::Logger::info("Creating game...");
	system(("mkdir \"" + SAVE_PATH + WORLD_NAME + "\\regions\"").c_str());
	logic::Noise::seed(1);
	//logic::Noise::seed(tools::Clock::getMicroseconds());
	tools::Random::seed(0);

	m_worldrenderer = new graphics::Renderer();
	m_creaturerenderer = new graphics::Renderer();
	m_guirenderer = new graphics::Renderer("arial.ttf");

#if ENABLE_AUDIO
	audio::AudioManager::loadAudio("recourses/hit.wav", 0);
	audio::AudioManager::loadAudio("recourses/swing.wav", 1);
	audio::AudioManager::loadAudio("recourses/collect.wav", 2);
#endif

	tools::Logger::info("Loading textures");
	graphics::TextureManager::loadTextureAtlas("recourses/atlas.png", GL_RGBA, 0);
	Database::init();
	tools::Logger::info("Textures loaded!");

	m_inventory = new Inventory(m_shader, m_window);

	//Constructing objects
	m_region[0][0];
	for (int x = 0; x < RENDER_DST; x++)
	{
		for (int y = 0; y < RENDER_DST; y++)
		{
			m_region[x][y] = Region(x, y);
		}
	}
	m_player = new Player(0, 0.0, m_inventory);


	m_shader->enable();
	m_shader->SetUniformMat4("ml_matrix", glm::mat4(1.0f));
	tools::Logger::info("Game ready!");
}

void Game::renderInfoScreen() {
	if (!debugMode) return;
	m_shader->enable();
	m_shader->setUniform1i("unif_text", 1);

	float textScale = 0.05;

	std::string text = "FPS: " + std::to_string(m_loop->getFPS());
	m_guirenderer->renderText(-m_window->getAspect(), -1.0 + (textScale * 0), textScale, textScale, 0, text, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);
	text = "UPS: " + std::to_string(m_loop->getUPS());
	m_guirenderer->renderText(-m_window->getAspect(), -1.0 + (textScale * 1), textScale, textScale, 0, text, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);

	text = "Position X: " + std::to_string(m_player->getX()) + ", Y: " + std::to_string(m_player->getY());
	m_guirenderer->renderText(-m_window->getAspect(), -1.0 + (textScale * 2), textScale, textScale, 0, text, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);
	text = "Tile X: " + std::to_string(getTile(m_player->getX(), m_player->getY(), "Info render")->getX()) + ", Y: " + std::to_string(getTile(m_player->getX(), m_player->getY(), "Info render")->getY());
	m_guirenderer->renderText(-m_window->getAspect(), -1.0 + (textScale * 3), textScale, textScale, 0, text, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);
	text = "Region X: " + std::to_string(posToRegionPos(m_player->getX())) + ", Y: " + std::to_string(posToRegionPos(m_player->getY()));
	m_guirenderer->renderText(-m_window->getAspect(), -1.0 + (textScale * 4), textScale, textScale, 0, text, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);
	text = "Region pos X: " + std::to_string(getRegion(m_player->getX(), m_player->getY())->getX()) + ", Y: " + std::to_string(getRegion(m_player->getX(), m_player->getY())->getY());
	m_guirenderer->renderText(-m_window->getAspect(), -1.0 + (textScale * 5), textScale, textScale, 0, text, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);
	text = "Renderer: " + std::string((char*)glGetString(GL_RENDERER));
	m_guirenderer->renderText(-m_window->getAspect(), -1.0 + (textScale * 6), textScale, textScale, 0, text, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);
}

void Game::render() {
	//World
	m_worldrenderer->clear();
	for (int x = 0; x < RENDER_DST; x++)
	{
		for (int y = 0; y < RENDER_DST; y++)
		{
			if (!m_region[x][y].null) m_region[x][y].submitToRenderer(m_worldrenderer, -m_player->x, -m_player->y);
		}
	}


	//Creature
	m_creaturerenderer->clear();
	for (int x = 0; x < RENDER_DST; x++)
	{
		for (int y = 0; y < RENDER_DST; y++)
		{
			if (!m_region[x][y].null) m_region[x][y].submitCreaturesToRenderer(m_creaturerenderer, -m_player->x, -m_player->y);
		}
	}
	m_player->submitToRenderer(m_creaturerenderer, -m_player->x, -m_player->y);


	//Gui
	m_guirenderer->clear();
	renderInfoScreen();
	m_inventory->render(m_guirenderer);
	
	//Flush
	m_worldrenderer->flush(m_shader, 0);
	m_creaturerenderer->flush(m_shader, 0);
	m_guirenderer->flush(m_shader, 0);
}

double clamp(double x, double upper, double lower)
{
	return std::min(upper, std::max(x, lower));
}

void Game::update() {
	if (!paused) {
		//Player movement
		float playerSpeed = 0.03;
		if (m_window->getKey(GLFW_KEY_LEFT_SHIFT)) playerSpeed *= 2;
		else if (m_window->getKey(GLFW_KEY_LEFT_CONTROL)) playerSpeed *= 0.2;
		if (m_window->getKey(GLFW_KEY_S)) { m_player->vel_y = playerSpeed; }
		if (m_window->getKey(GLFW_KEY_D)) { m_player->vel_x = playerSpeed; }
		if (m_window->getKey(GLFW_KEY_W)) { m_player->vel_y = -playerSpeed; }
		if (m_window->getKey(GLFW_KEY_A)) { m_player->vel_x = -playerSpeed; }
		m_player->update();
		m_inventory->update();

		//Load regions after camera/player moves
		processNewArea();

		//Player collision
		m_player->collide();

		//World updates
		for (int x = 0; x < RENDER_DST; x++)
		{
			for (int y = 0; y < RENDER_DST; y++)
			{
				if (!m_region[x][y].null) {
					m_region[x][y].update();
				}
			}
		}

	}

	//Load regions after camera/player moves
	processNewArea();
	for (int x = 0; x < RENDER_DST; x++)
	{
		for (int y = 0; y < RENDER_DST; y++)
		{
			if (tilesChanged) {
				m_region[x][y].tilesChanged();
			}
		}
	}
	if (tilesChanged) tilesChanged = false;

}

void Game::rapidUpdate() {}

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
		tools::Logger::info(std::string("Fps: ") + std::to_string(m_loop->getFPS()));
		for (int x = 0; x < RENDER_DST; x++)
		{
			for (int y = 0; y < RENDER_DST; y++)
			{
				if (!m_region[x][y].null) m_region[x][y].debugCeilCreatures();
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
			if (!m_region[x][y].null) {
				m_region[x][y].saveTiles();
			}
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
//std::cout << "microseconds: " << tools::Clock::getMicroseconds() - startTime << std::endl;
///	 _____   _____   _____   _____   _____   _____   _____   _____   _____   _____   _____   _____   _____   _______   _   _     _	 _____ 
///	/  _  \ /  _  \ /  _  \ /  _  \ /  _  \ /  _  \ /  _  \ /  _  \ /  _  \ /  _  \ /  _  \ /  _  \ /  _  \ |  ___  \ | | | |   | |	/  _  \
///	| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |___| | | | |  \  | |	| | |_|
///	| |_| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |    ___/ | | |   \ | |	| |___
///	|  _ <	| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |\ \    | | | |\ \| |	| |\  |
///	| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | \ \   | | | | \   |	| | | |
///	| |_| | | |_| | | |_| | | |_| | | |_| | | |_| | | |_| | | |_| | | |_| | | |_| | | |_| | | |_| | | |_| | | |  \ \  | | | |  \  |	| |_| |
///	\_____/ \_____/ \_____/ \_____/ \_____/ \_____/ \_____/ \_____/ \_____/ \_____/ \_____/ \_____/ \_____/ |_|   \_| |_| |_|   |_| \_____|
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





void Game::shiftRegions(int deltaX, int deltaY, int playerRegionX, int playerRegionY) {
	if (deltaX >= 0 && deltaY >= 0) {
		for (int x = 0; x < RENDER_DST; x++)
		{
			for (int y = 0; y < RENDER_DST; y++)
			{
				shiftRegionLoop(x, y, deltaX, deltaY, playerRegionX, playerRegionY);
			}
		}
	}
	else if (deltaX <= 0 && deltaY >= 0) {
		for (int x = RENDER_DST - 1; x > -1; x--)
		{
			for (int y = 0; y < RENDER_DST; y++)
			{
				shiftRegionLoop(x, y, deltaX, deltaY, playerRegionX, playerRegionY);
			}
		}
	}
	else if (deltaX >= 0 && deltaY <= 0) {
		for (int x = 0; x < RENDER_DST; x++)
		{
			for (int y = RENDER_DST - 1; y > -1; y--)
			{
				shiftRegionLoop(x, y, deltaX, deltaY, playerRegionX, playerRegionY);
			}
		}
	}
	else { // if (deltaX <= 0 && deltaY <= 0)
		for (int x = RENDER_DST - 1; x > -1; x--)
		{
			for (int y = RENDER_DST - 1; y > -1; y--)
			{
				shiftRegionLoop(x, y, deltaX, deltaY, playerRegionX, playerRegionY);
			}
		}
	}
}

void Game::shiftRegionLoop(int x, int y, int deltaX, int deltaY, int playerRegionX, int playerRegionY) {
	if (!logic::isInRange(x - deltaX, 0, RENDER_DST - 1) || !logic::isInRange(y - deltaY, 0, RENDER_DST - 1)) {
		m_region[x][y].saveTiles();
	}
	if (logic::isInRange(x + deltaX, 0, RENDER_DST - 1) && logic::isInRange(y + deltaY, 0, RENDER_DST - 1)) {
		m_region[x][y] = m_region[x + deltaX][y + deltaY];
	}
	else {
		m_region[x][y] = Region(x + playerRegionX, y + playerRegionY); 
		tilesChanged = true;
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

#define REGION_LOAD_FUNC false
void Game::processNewArea() {
	float playerX = m_player->getX();
	float playerY = m_player->getY();

	int playerRegionX = posToRegionPos(playerX - REGION_SIZE / 2.0) + 1;
	int playerRegionY = posToRegionPos(playerY - REGION_SIZE / 2.0) + 1;

	if (lastRegionX != playerRegionX || lastRegionY != playerRegionY) {

		int regionsTravelledX = playerRegionX - lastRegionX;
		int regionsTravelledY = playerRegionY - lastRegionY;

		shiftRegions(regionsTravelledX, regionsTravelledY, playerRegionX, playerRegionY);
	}

	lastRegionX = playerRegionX;
	lastRegionY = playerRegionY;
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

	int offsetRegionX = posToRegionPos(round(m_player->getX())) - posToRegionPos(round(m_player->getX() - REGION_SIZE / 2.0)) + 1; //Change last ( + 2) when changing render dst
	int offsetRegionY = posToRegionPos(round(m_player->getY())) - posToRegionPos(round(m_player->getY() - REGION_SIZE / 2.0)) + 1; //Change last ( + 2) when changing render dst

	int finalRegionX = selectedRegionX - playerRegionX + offsetRegionX;
	int finalRegionY = selectedRegionY - playerRegionY + offsetRegionY;


	if (finalRegionX < 0 || finalRegionX > RENDER_DST - 1 || finalRegionY < 0 || finalRegionY > RENDER_DST - 1) {
		if (!debugMode) return nullptr;
	}

	int finalX = x - m_region[finalRegionX][finalRegionY].getX();
	int finalY = y - m_region[finalRegionX][finalRegionY].getY();

	if (!logic::isInRange(finalX, 0, REGION_SIZE - 1) || !logic::isInRange(finalY, 0, REGION_SIZE - 1)) {
		if (debugMode) tools::Logger::warning("Finals out of range!");
		if (debugMode) tools::Logger::warning("");
		if (debugMode) tools::Logger::warning(std::to_string(x) + std::string(", ") + std::to_string(y));
		if (debugMode) tools::Logger::warning(std::to_string(m_region[finalRegionX][finalRegionY].getX()) + std::string(", ") + std::to_string(m_region[finalRegionX][finalRegionY].getY()));
		if (debugMode) tools::Logger::warning(std::to_string(finalX) + std::string(", ") + std::to_string(finalY));
		if (debugMode) tools::Logger::warning(std::to_string(finalRegionX) + std::string(", ") + std::to_string(finalRegionY));
		if (debugMode) tools::Logger::warning(std::to_string(selectedRegionX) + std::string(", ") + std::to_string(selectedRegionY));
		return nullptr;
	}

	return &m_region[finalRegionX][finalRegionY];
}

Tile* Game::getTile(float x, float y, std::string debugText) {
	x = round(x); y = round(y);

	Region *regionAt = getRegion(x, y);
	if (!regionAt) {
		if (debugMode) tools::Logger::warning(std::string("Region was nullptr!"));
		return nullptr;
	}

	int finalX = x - regionAt->getX();
	int finalY = y - regionAt->getY();

	if (!logic::isInRange(finalX, 0, REGION_SIZE - 1) || !logic::isInRange(finalY, 0, REGION_SIZE - 1)) {
		if (debugMode) tools::Logger::warning("Finals out of range!");
		if (debugMode) tools::Logger::warning(debugText);
		if (debugMode) tools::Logger::warning(std::to_string(x) + std::string(", ") + std::to_string(y));
		if (debugMode) tools::Logger::warning(std::to_string(regionAt->getX()) + std::string(", ") + std::to_string(regionAt->getY()));
		if (debugMode) tools::Logger::warning(std::to_string(finalX) + std::string(", ") + std::to_string(finalY));
		return nullptr;
	}

	Tile *tile = regionAt->getTile(finalX, finalY);
	if (!tile) {
		if (debugMode) tools::Logger::error(std::string("region returned nullptr instead of tile"));
		return nullptr;
	}
	if (tile->getObjectId() > COUNT_OBJECTS || tile->getObjectId() < 0) {
		if (debugMode) tools::Logger::error(std::string("getTile error at ") + std::to_string(finalX) + std::string(", ") + std::to_string(finalY));
		return nullptr;
	}

	if (tile->getX() != x || tile->getY() != y) {
		if (!debugMode) return nullptr;

		int selectedRegionX = posToRegionPos(x);
		int selectedRegionY = posToRegionPos(y);

		int playerRegionX = posToRegionPos(round(m_player->getX()));
		int playerRegionY = posToRegionPos(round(m_player->getY()));

		int offsetRegionX = posToRegionPos(round(m_player->getX())) - posToRegionPos(round(m_player->getX() - REGION_SIZE / 2.0)) + 1;
		int offsetRegionY = posToRegionPos(round(m_player->getY())) - posToRegionPos(round(m_player->getY() - REGION_SIZE / 2.0)) + 1;

		int finalRegionX = selectedRegionX - playerRegionX + offsetRegionX;
		int finalRegionY = selectedRegionY - playerRegionY + offsetRegionY;

		tools::Logger::info("Ending results not as expected");
		tools::Logger::info(std::string("selectedRegion ") + std::to_string(selectedRegionX) + std::string(", ") + std::to_string(selectedRegionY));
		tools::Logger::info(std::string("playerRegion ") + std::to_string(playerRegionX) + std::string(", ") + std::to_string(playerRegionY));
		tools::Logger::info(std::string("offsetRegion ") + std::to_string(offsetRegionX) + std::string(", ") + std::to_string(offsetRegionY));
		tools::Logger::info(std::string("finalRegion ") + std::to_string(finalRegionX) + std::string(", ") + std::to_string(finalRegionY));
		tools::Logger::info(std::string("playerPos ") + std::to_string(round(m_player->getX())) + std::string(", ") + std::to_string(round(m_player->getY())));
		tools::Logger::info(std::string("cursorPos ") + std::to_string(x) + std::string(", ") + std::to_string(y));

		tools::Logger::info(std::string("FinalXY ") + std::to_string(finalX) + std::string(", ") + std::to_string(finalY));
		tools::Logger::info(std::string("tile getXY ") + std::to_string(tile->getX()) + std::string(", ") + std::to_string(tile->getY()));
		tools::Logger::info(std::string("debug text: ") + debugText);
	}

	return tile;
}

void Game::findAllCreatures(Region* region, int x, int y, Creature** array, unsigned int& amount) {
	amount = 0;

	for (int i = 0; i < MAX_CREATURES; i++)
	{
		Creature *creature = region->getCreature(i);
		if (creature) {
			if (creature->getX() > x - 1 && creature->getX() < x + 1) {
				if (creature->getY() > y - 1 && creature->getY() < y + 1) {
					array[amount] = creature;
					amount++;
				}
			}
		}
	}
}

Player *Game::getPlayer() {
	return m_player;
}

bool Game::trySetTileObject(float x, float y, int id) {
	Tile *tile = getTile(x, y, "setting tile obj");
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