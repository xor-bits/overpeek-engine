#include "game.h"

#include "../creatures/player.h"
#include "../world/region.h"
#include "../logic/inventory.h"

#include <windows.h>
#include <math.h>
#include <omp.h>

graphics::Window *Game::m_window;
graphics::Shader *Game::m_shader;
graphics::Shader *Game::m_postshader;
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
int Game::seed = 0;

bool Game::debugMode = false;
bool Game::advancedDebugMode = false;
bool Game::tilesChanged = true;

int Game::hitCooldown = 513709;
bool holdingI = false;

bool paused = false;


std::string renderer;

void Game::init() {
	tools::Logger::setup();

	//Audio
#if ENABLE_AUDIO
	tools::Logger::info("Creating audio device");
	audio::AudioManager::init();
	audio::AudioManager::loadAudio("recourses/hit.wav", 0);
	audio::AudioManager::loadAudio("recourses/swing.wav", 1);
	audio::AudioManager::loadAudio("recourses/collect.wav", 2);
#endif

	//Window
	tools::Logger::info("Creating window");
	m_window = new graphics::Window(M_WINDOW_WIDTH, M_WINDOW_HEIGHT, M_WINDOW_DEFAULT_TITLE, false, M_DEFAULT_MULTISAMPLE);
	m_window->setSwapInterval(NULL);
	m_window->setButtonCallback(buttonPress);
	m_window->setKeyboardCallback(keyPress);
	m_window->setScrollCallback(scroll);

	//Create shader
	tools::Logger::info("Creating shader");
	tools::Logger::info("postprocess");
	m_postshader = new graphics::Shader("shaders/postprocess.vert.glsl", "shaders/postprocess.frag.glsl");
	tools::Logger::info("texture");
	m_shader = new graphics::Shader("shaders/texture.vert.glsl", "shaders/texture.frag.glsl");
	tools::Logger::info("Shader created!");

	//Shader stuff
	glm::mat4 orthographic = glm::ortho(-M_ASPECT * DEBUG_ZOOM, M_ASPECT * DEBUG_ZOOM, DEBUG_ZOOM, -DEBUG_ZOOM);
	m_postshader->enable(); m_postshader->SetUniformMat4("pr_matrix", orthographic);
	m_shader->enable(); m_shader->SetUniformMat4("pr_matrix", orthographic);
	
	//Gameloop
	tools::Logger::info("Starting gameloop");
	m_loop = new logic::GameLoop(render, update, 100, 10000);

	loadGame();
	renderer = std::string((char*)glGetString(GL_RENDERER));

	m_loop->start();
}

void Game::renderInfoScreen() {
	if (!debugMode) return;

	float textScale = 0.05;
	float x = -m_window->getAspect();

	std::string text = "FPS: " + std::to_string(m_loop->getFPS());
	m_guirenderer->renderText(x, -1.0 + (textScale * 0), textScale, textScale, 0, text, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);
	text = "UPS: " + std::to_string(m_loop->getUPS());
	m_guirenderer->renderText(x, -1.0 + (textScale * 1), textScale, textScale, 0, text, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);


	if (!advancedDebugMode) return;
	text = "Position X: " + std::to_string(m_player->getX()) + ", Y: " + std::to_string(m_player->getY());
	m_guirenderer->renderText(x, -1.0 + (textScale * 2), textScale, textScale, 0, text, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);
	text = "Tile X: " + std::to_string(getTile(m_player->getX(), m_player->getY(), "Info render")->getX()) + ", Y: " + std::to_string(getTile(m_player->getX(), m_player->getY(), "Info render")->getY());
	m_guirenderer->renderText(x, -1.0 + (textScale * 3), textScale, textScale, 0, text, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);
	text = "Region X: " + std::to_string(posToRegionPos(m_player->getX())) + ", Y: " + std::to_string(posToRegionPos(m_player->getY()));
	m_guirenderer->renderText(x, -1.0 + (textScale * 4), textScale, textScale, 0, text, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);
	text = "Region pos X: " + std::to_string(getRegion(m_player->getX(), m_player->getY())->getX()) + ", Y: " + std::to_string(getRegion(m_player->getX(), m_player->getY())->getY());
	m_guirenderer->renderText(x, -1.0 + (textScale * 5), textScale, textScale, 0, text, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);
	text = "Renderer: " + renderer;
	m_guirenderer->renderText(x, -1.0 + (textScale * 6), textScale, textScale, 0, text, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);
	text = "Biome: " + std::string(Database::getBiome(getTileBiome(m_player->getX(), m_player->getY(), BIOME_SCRAMBLE1), getTileBiome(m_player->getX(), m_player->getY(), BIOME_SCRAMBLE2)).name);
	m_guirenderer->renderText(x, -1.0 + (textScale * 7), textScale, textScale, 0, text, glm::vec3(1.0, 1.0, 1.0), TEXT_ALIGN_RIGHT, TEXT_ALIGN_BOTTOM);
}

void Game::render() {
	if (!m_window) return;
	
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
	m_inventory->render(m_guirenderer); 
	renderInfoScreen();
	//long long startTime = tools::Clock::getMicroseconds();
	//tools::Logger::info(tools::Clock::getMicroseconds() - startTime);
	
	//Flush
	m_shader->enable();
	m_worldrenderer->flush(m_shader, 0);
	m_creaturerenderer->flush(m_shader, 0);
	m_guirenderer->flush(m_shader, 0);

	// Render to our framebuffer
	//m_worldrenderer->renderToFramebuffer(m_postshader, m_shader, 0);

	m_window->update();
	m_window->clear();
	m_window->input();
}

double clamp(double x, double upper, double lower)
{
	return std::min(upper, std::max(x, lower));
}

void Game::update() {
	if (!m_window) return;

	if (m_window->close()) m_loop->stop();
	if (!paused) {
		//Player movement
		float playerSpeed = 0.03;
		if (m_window->getKey(GLFW_KEY_LEFT_SHIFT)) playerSpeed *= 2;
		else if (m_window->getKey(GLFW_KEY_LEFT_CONTROL)) playerSpeed *= 0.2;
		else if (m_window->getKey(GLFW_KEY_TAB)) playerSpeed *= 20;
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

void Game::keyPress(int key, int action) {
	if (key == GLFW_KEY_ESCAPE) m_loop->stop();

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

	//Debug commands
	//Activate debug and advanced debug modes
	if (key == GLFW_KEY_F1) {
		debugMode = !debugMode; 
		if (m_window->getKey(GLFW_KEY_LEFT_SHIFT)) {
			advancedDebugMode = debugMode;
		}
		else advancedDebugMode = false;
		return; 
	}

	//Debug ceil creaures
	if (key == GLFW_KEY_F2) {
		for (int x = 0; x < RENDER_DST; x++)
		{
			for (int y = 0; y < RENDER_DST; y++)
			{
				if (!m_region[x][y].null) m_region[x][y].debugCeilCreatures();
			}
		}
	}
	
	//Reload game
	if (key == GLFW_KEY_F3) {
		close();
		delete m_inventory;
		delete m_player;

		lastRegionX = 0;
		lastRegionY = 0;
		debugMode = false;
		tilesChanged = true;

		hitCooldown = 513709;
		holdingI = false;

		paused = false;
		
		loadGame();

		return;
	}

	//Get FPS
	if (key == GLFW_KEY_F4) tools::Logger::info(std::string("Fps: ") + std::to_string(m_loop->getFPS()));
	
	//Clear inventoy
	if (key == GLFW_KEY_F5) m_inventory->clear();
	
	//Add creature at player
	if (key == GLFW_KEY_F6) addCreature(m_player->getX(), m_player->getY(), 1, false);
}

void Game::buttonPress(int button, int action) {}

void Game::scroll(double y) {
	//Inventory slot scrolling
	if (y < 0) m_inventory->selectedSlot++;
	if (y > 0) m_inventory->selectedSlot--;
}

void Game::close() {
	saveGame();
}

void Game::saveGame() {

#if !DEBUG_DISABLE_SAVING
	float playerData[2] = { m_player->getX(), m_player->getY() };
	for (int x = 0; x < RENDER_DST; x++) {
		for (int y = 0; y < RENDER_DST; y++) {
			if (!m_region[x][y].null) {
				m_region[x][y].saveTiles();
			}
		}
	}
	tools::Logger::info(seed);
	int worldData[1] = { seed }; 


	tools::BinaryIO::write<float>(getSaveLocation() + "player_data", playerData, 2);
	tools::BinaryIO::write<int>(getSaveLocation() + "world_data", worldData, 1);
#endif

	tools::Logger::info("Save complete!");
}

void Game::loadGame() {
	//Initializing
	tools::Logger::info("Creating game...");
	system(("mkdir \"" + SAVE_PATH + WORLD_NAME + "\\regions\"").c_str());

	//Load seed
#if !DEBUG_DISABLE_SAVING
	void* seedptr = tools::BinaryIO::read<int>(getSaveLocation() + "world_data");
	if (seedptr) seed = ((int*)seedptr)[0];
	else seed = tools::Clock::getMicroseconds();
#else
	seed = tools::Clock::getMicroseconds();
	seed = 0;
#endif
	logic::Noise::seed(seed);
	tools::Random::seed(seed);

	tools::Logger::info(std::string("World seed: ") + std::to_string(seed)); // 2058261791

	m_worldrenderer = new graphics::Renderer();
	m_creaturerenderer = new graphics::Renderer();
	m_guirenderer = new graphics::Renderer("arial.ttf");

	tools::Logger::info("Loading textures");
	graphics::TextureManager::loadTextureAtlas("recourses/atlas.png", GL_RGBA, 0);
	tools::Logger::info("Textures loaded!");
	tools::Logger::info("Loading database");
	Database::init();
	tools::Logger::info("Database loaded!");

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
	float playerX = 0, playerY = 0;

#if !DEBUG_DISABLE_SAVING
	float *playerData = (float*)tools::BinaryIO::read<float>(getSaveLocation() + "player_data");
	if (playerData) { playerX = playerData[0]; playerY = playerData[1]; }
#endif
	m_player = new Player(playerX, playerY, m_inventory);


	m_shader->enable();
	m_shader->SetUniformMat4("ml_matrix", glm::mat4(1.0f));
	tools::Logger::info("Game ready!");
}

float Game::getTileBiome(float x, float y, float z) {
	x += z * 3; y -= z;
	float height = (logic::Noise::octaveNoise(x * 0.02 / 4.0, y * 0.02 / 4.0, 3) + 1.0) / 2.0;
	return height;
}

void Game::getInfoFromNoise(int &tileId, int &objId, double x, double y) {
	Database::Biome biome = Database::getBiome(getTileBiome(x, y, BIOME_SCRAMBLE1), getTileBiome(x, y, BIOME_SCRAMBLE2));

	x += 99999; y -= 555555;

	x *= NOISE_SCALE; y *= NOISE_SCALE;

	float height = (logic::Noise::octaveNoise(x, y, 3) + 1.0) / 2.0;
	
	for (int i = 0; i < biome.heightMap.size(); i++)
	{
		if (height <= biome.heightMap[i].height) {
			tileId = biome.heightMap[i].id;

			if (biome.heightMap[i].grassId != 0) {
				float grassnoise = (logic::Noise::octaveNoise(x * biome.heightMap[i].grassNoiseFrequency, y * biome.heightMap[i].grassNoiseFrequency, 578.1) + 1.0) / 2.0;
				if (grassnoise > biome.heightMap[i].grassRarity) objId = biome.heightMap[i].grassId;
			}
			if (biome.heightMap[i].plantId != 0) {
				//long long startTime = tools::Clock::getMicroseconds();
				//tools::Logger::info(tools::Clock::getMicroseconds() - startTime);
				float plantnoise = (logic::Noise::octaveNoise(14.2 + x * biome.heightMap[i].plantNoiseFrequency, 4.75 - y * biome.heightMap[i].plantNoiseFrequency, 34.24) + 1.0) / 2.0;
				if (plantnoise > biome.heightMap[i].plantRarity) objId = biome.heightMap[i].plantId;
			}
			break;
		}
	}
	//tools::Logger::info(biome.name);
	//tools::Logger::info(biome.heightMap[0].id);
	
	
	//tools::Logger::info("Id " + Database::tiles[id].name);
	//if (height <= LEVEL_WATER) id = 1;
	//else if (height <= LEVEL_SAND) id = 2;
	//else if (height <= LEVEL_SOIL) {
	//	float n = (logic::Noise::octaveNoise(x * 3.0, y * 3.0, 3) + 1.0) / 2.0;
	//	if (n > 1.0 - NOISE_GRASS || n < NOISE_GRASS) id = 3;
	//	else  id = 0;
	//
	//	if (n > 1 - NOISE_FOREST) object_id = round(tools::Random::random(2.0, 3.0));
	//	else if (n < NOISE_FOREST) object_id = round(tools::Random::random(2.0, 3.0));
	//	else if (n > 1.0 - NOISE_GRASS || n < NOISE_GRASS) {
	//		if (tools::Random::random(0.0, 1.0) > 0.5) object_id = 1;
	//	}
	//}
	//else if (height <= LEVEL_STONE) id = 4;
	//else { id = 4; object_id = 5; }
}





//long long startTime = tools::Clock::getMicroseconds();
//tools::Logger::info(tools::Clock::getMicroseconds() - startTime);
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
#if !DEBUG_DISABLE_SAVING
	if (!logic::isInRange(x - deltaX, 0, RENDER_DST - 1) || !logic::isInRange(y - deltaY, 0, RENDER_DST - 1)) {
		m_region[x][y].saveTiles();
	}
#endif
	if (logic::isInRange(x + deltaX, 0, RENDER_DST - 1) && logic::isInRange(y + deltaY, 0, RENDER_DST - 1)) {
		m_region[x][y] = m_region[x + deltaX][y + deltaY];
	}
	else {
		m_region[x][y] = Region(x + playerRegionX, y + playerRegionY); 
		tilesChanged = true;
	}
}

void Game::addCreature(float x, float y, int id, bool item) {
	Region *regionToAdd = getRegion(x, y);
	regionToAdd->addCreature(x, y, id, item);
}

#define REGION_LOAD_FUNC false
void Game::processNewArea() {
	float playerX = m_player->getX();
	float playerY = m_player->getY();

	int playerRegionY = posToRegionPos(playerY - REGION_SIZE / 2.0) + 1;
	int playerRegionX = posToRegionPos(playerX - REGION_SIZE / 2.0) + 1;

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

	int offsetRegionX = posToRegionPos(round(m_player->getX())) - posToRegionPos(round(m_player->getX() - REGION_SIZE / 2.0)) + 2; //Change last ( + 2) when changing render dst
	int offsetRegionY = posToRegionPos(round(m_player->getY())) - posToRegionPos(round(m_player->getY() - REGION_SIZE / 2.0)) + 2; //Change last ( + 2) when changing render dst

	int finalRegionX = selectedRegionX - playerRegionX + offsetRegionX;
	int finalRegionY = selectedRegionY - playerRegionY + offsetRegionY;


	if (finalRegionX < 0 || finalRegionX > RENDER_DST - 1 || finalRegionY < 0 || finalRegionY > RENDER_DST - 1) {
		if (!debugMode) return nullptr;
	}

	int finalX = x - m_region[finalRegionX][finalRegionY].getX();
	int finalY = y - m_region[finalRegionX][finalRegionY].getY();

	if (!logic::isInRange(finalX, 0, REGION_SIZE - 1) || !logic::isInRange(finalY, 0, REGION_SIZE - 1)) {
#if !SHOW_DEBUG_MESSAGES
		return nullptr;
#endif

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
#if !SHOW_DEBUG_MESSAGES
		return nullptr;
#endif
		if (debugMode) tools::Logger::warning(std::string("Region was nullptr!"));

		return nullptr;
	}

	int finalX = x - regionAt->getX();
	int finalY = y - regionAt->getY();

	if (!logic::isInRange(finalX, 0, REGION_SIZE - 1) || !logic::isInRange(finalY, 0, REGION_SIZE - 1)) {
#if !SHOW_DEBUG_MESSAGES
		return nullptr;
#endif

		if (debugMode) tools::Logger::warning("Finals out of range!");
		if (debugMode) tools::Logger::warning(debugText);
		if (debugMode) tools::Logger::warning(std::to_string(x) + std::string(", ") + std::to_string(y));
		if (debugMode) tools::Logger::warning(std::to_string(regionAt->getX()) + std::string(", ") + std::to_string(regionAt->getY()));
		if (debugMode) tools::Logger::warning(std::to_string(finalX) + std::string(", ") + std::to_string(finalY));

		return nullptr;
	}

	Tile *tile = regionAt->getTile(finalX, finalY);
	if (!tile) {
#if !SHOW_DEBUG_MESSAGES
		return nullptr;
#endif

		if (debugMode) tools::Logger::error(std::string("region returned nullptr instead of tile"));

		return nullptr;
	}
	if (tile->getObjectId() > COUNT_OBJECTS || tile->getObjectId() < 0) {
#if !SHOW_DEBUG_MESSAGES
		return nullptr;
#endif

		if (debugMode) tools::Logger::error(std::string("getTile error at ") + std::to_string(finalX) + std::string(", ") + std::to_string(finalY));

		return nullptr;
	}

	if (tile->getX() != x || tile->getY() != y) {
#if !SHOW_DEBUG_MESSAGES
		return nullptr;
#endif

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

		return nullptr;
	}

	return tile;
}

void Game::findAllCreatures(float _x, float _y, Creature** _array, unsigned int& _amount, float _radius) {
	_amount = 0;

	for (int x = 0; x < RENDER_DST; x++)
	{
		for (int y = 0; y < RENDER_DST; y++)
		{
			if (m_region[x][y].null) continue;

			for (int i = 0; i < MAX_CREATURES; i++)
			{
				Creature *creature = m_region[x][y].getCreature(i); 
				if (creature) {
					if (creature->getX() >= _x - _radius && creature->getX() <= _x + _radius) {
						if (creature->getY() >= _y - _radius && creature->getY() <= _y + _radius) {
							_array[_amount] = creature;
							_amount++;
						}
					}
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

std::string Game::getSaveLocation() {
	return (SAVE_PATH + WORLD_NAME + "\\").c_str();
}