#include "../pch.h"


#include "game.h"

#include "../creatures/player.h"
#include "../world/region.h"
#include "../world/map.h"
#include "../logic/inventory.h"
#include "gui.h"
#include "database.h"

std::unique_ptr<graphics::Window> Game::m_window;
std::unique_ptr<graphics::Shader> Game::m_shader;
std::unique_ptr<graphics::Shader> Game::m_pointshader;
std::unique_ptr<graphics::Shader> Game::m_postshader;
std::unique_ptr<graphics::Shader> Game::m_guishader;
std::unique_ptr<graphics::Renderer> Game::m_worldrenderer;
std::unique_ptr<graphics::Renderer> Game::m_guirenderer;
std::unique_ptr<logic::GameLoop> Game::m_loop;
std::unique_ptr<Gui> Game::m_gui;
std::unique_ptr<Inventory> m_inventory;
std::unique_ptr<Map> Game::m_map;
std::unique_ptr<Player> Game::m_player;

#if !STORE_MAP_IN_RAM
Region Game::m_region[RENDER_DST][RENDER_DST];
bool Game::m_regionExist[RENDER_DST][RENDER_DST];
std::vector<Region> Game::m_regionUnloadArray;

float Game::lastRegionX = 0;
float Game::lastRegionY = 0;
int Game::seed = 0;

int Game::asyncLoadX = 0;
int Game::asyncLoadY = 0;
int Game::asyncSaveIndex = 0;
#endif

bool Game::debugMode = false;
bool Game::advancedDebugMode = false;
bool Game::tilesChanged = true;
bool Game::justPaused = false;
bool Game::paused = false;


std::string renderer;

void Game::init() {
	//Audio
#if ENABLE_AUDIO
	tools::Logger::info("Creating audio device");
	audio::AudioManager::init();
#endif

	//Window
	tools::Logger::info("Creating window");
	m_window = std::unique_ptr<graphics::Window>(new graphics::Window(M_WINDOW_WIDTH, M_WINDOW_HEIGHT, M_WINDOW_DEFAULT_TITLE, false, M_DEFAULT_MULTISAMPLE));
	m_window->setSwapInterval(NULL);
	m_window->setButtonCallback(buttonPress);
	m_window->setKeyboardCallback(keyPress);
	m_window->setScrollCallback(scroll);

	//Create shader
	tools::Logger::info("Creating all the shaders");
	tools::Logger::info("Shader for postprocessing");
	m_postshader = std::unique_ptr<graphics::Shader>(new graphics::Shader("shaders/postprocess.vert.glsl", "shaders/postprocess.frag.glsl"));
	tools::Logger::info("Shader for textures");
	m_shader = std::unique_ptr<graphics::Shader>(new graphics::Shader("shaders/texture.vert.glsl", "shaders/texture.frag.glsl"));
	m_pointshader = std::unique_ptr<graphics::Shader>(new graphics::Shader("shaders/geometrytexture.vert.glsl", "shaders/geometrytexture.frag.glsl", "shaders/geometrytexture.geom.glsl"));
	tools::Logger::info("All shaders created successfully!");

	//Shader stuff
	m_postshader->enable();
	m_postshader->setUniform1i("unif_effect", 0);
	m_postshader->setUniform1f("unif_width", m_window->getWidth());
	m_postshader->setUniform1f("unif_height", m_window->getHeight());
	glm::mat4 orthographic = glm::ortho(-M_ASPECT * DEBUG_ZOOM, M_ASPECT * DEBUG_ZOOM, DEBUG_ZOOM, -DEBUG_ZOOM);
	m_shader->enable(); m_shader->SetUniformMat4("pr_matrix", orthographic);
	
	//Gameloop
	tools::Logger::info("Starting gameloop");
	m_loop = std::unique_ptr<logic::GameLoop>(new logic::GameLoop(render, update, rapidUpdate, UPDATES_PER_SECOND, 10000));

	loadGame();
	renderer = std::string((char*)glGetString(GL_RENDERER));

	tools::Logger::info("Running update and renderloops");
	m_loop->start();
}

void Game::rapidUpdate() {
}

void Game::renderInfoScreen() {
	if (!debugMode) return;

	float textScale = 0.05;
	float x = -m_window->getAspect();

	std::string text = "FPS: " + std::to_string(m_loop->getFPS());
	m_guirenderer->renderText(glm::vec2(x, -1.0 + (textScale * 0)), glm::vec2(textScale, textScale), text, glm::vec4(1.0, 1.0, 1.0, 1.0), TEXT_ORIGIN_LEFT, TEXT_ORIGIN_TOP);
	text = "UPS: " + std::to_string(m_loop->getUPS());
	m_guirenderer->renderText(glm::vec2(x, -1.0 + (textScale * 1)), glm::vec2(textScale, textScale), text, glm::vec4(1.0, 1.0, 1.0, 1.0), TEXT_ORIGIN_LEFT, TEXT_ORIGIN_TOP);
	
	
	if (!advancedDebugMode) return;
	text = "Position X: " + std::to_string(m_player->getX()) + ", Y: " + std::to_string(m_player->getY());
	m_guirenderer->renderText(glm::vec2(x, -1.0 + (textScale * 2)), glm::vec2(textScale, textScale), text, glm::vec4(1.0, 1.0, 1.0, 1.0), TEXT_ORIGIN_LEFT, TEXT_ORIGIN_TOP);
	text = "Tile: " + Database::tiles[m_map->getTile(m_player->getY(), m_player->getY())->m_tile].name;
	m_guirenderer->renderText(glm::vec2(x, -1.0 + (textScale * 3)), glm::vec2(textScale, textScale), text, glm::vec4(1.0, 1.0, 1.0, 1.0), TEXT_ORIGIN_LEFT, TEXT_ORIGIN_TOP);
	text = "Object: " + Database::objects[m_map->getTile(m_player->getY(), m_player->getY())->m_object].name;
	m_guirenderer->renderText(glm::vec2(x, -1.0 + (textScale * 4)), glm::vec2(textScale, textScale), text, glm::vec4(1.0, 1.0, 1.0, 1.0), TEXT_ORIGIN_LEFT, TEXT_ORIGIN_TOP);
	text = "Renderer: " + renderer;
	m_guirenderer->renderText(glm::vec2(x, -1.0 + (textScale * 5)), glm::vec2(textScale, textScale), text, glm::vec4(1.0, 1.0, 1.0, 1.0), TEXT_ORIGIN_LEFT, TEXT_ORIGIN_TOP);
}

void Game::render() {
	//return;
	if (!m_window) return;
	
	//World tiles
	if (!paused || justPaused) {

#if !STORE_MAP_IN_RAM
		for (int x = 0; x < RENDER_DST; x++)
		{
			for (int y = 0; y < RENDER_DST; y++)
			{
				if (m_regionExist[x][y]) m_region[x][y].submitTilesToRenderer(m_worldrenderer, -m_player->x, -m_player->y);
				if (m_regionExist[x][y]) m_region[x][y].submitObjectsToRenderer(m_worldrenderer, -m_player->x, -m_player->y);
				if (m_regionExist[x][y]) m_region[x][y].submitCreaturesToRenderer(m_worldrenderer, -m_player->x, -m_player->y);
			}
		}
#else
		m_map->submitToRenderer(m_worldrenderer.get());
#endif
		m_player->submitToRenderer(m_worldrenderer.get(), -m_player->getX(), -m_player->getY());
		m_gui->renderBlur(m_worldrenderer.get());
		m_inventory->render(m_worldrenderer.get());
	}
	
	//Gui
	renderInfoScreen();
	m_gui->renderNoBlur(m_guirenderer.get());

	//Flush
	if (!paused) {
		m_worldrenderer->draw(m_shader.get(), m_pointshader.get(), "unif_text", graphics::TextureManager::getTexture(0));
	}
	else if (justPaused) {
		m_worldrenderer->drawToFramebuffer(m_shader.get(), m_pointshader.get(), "unif_text", graphics::TextureManager::getTexture(0), false);
		m_postshader->enable();
		for (int i = 0; i < 16; i++) {
			m_postshader->setUniform1i("unif_effect", 1);
			m_worldrenderer->drawFramebufferToFramebuffer(m_postshader.get(), "unif_texture", true);
			m_postshader->setUniform1i("unif_effect", 2);
			m_worldrenderer->drawFramebufferToFramebuffer(m_postshader.get(), "unif_texture", false);
		}
		m_postshader->setUniform1i("unif_effect", 0);
		m_worldrenderer->drawFramebuffer(m_postshader.get(), "unif_texture", false);
	}
	else if (paused) {
		m_postshader->enable();
		m_postshader->setUniform1i("unif_effect", 0);
		m_worldrenderer->drawFramebuffer(m_postshader.get(), "unif_texture", false);
	}
	
	m_guirenderer->draw(m_shader.get(), m_pointshader.get(), "unif_text", graphics::TextureManager::getTexture(0));

	//Other
	justPaused = false;

	//Window updates
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
		bool running = false;
		bool moving = false;
		if (m_window->getKey(GLFW_KEY_LEFT_SHIFT)) running = true;
		if (m_window->getKey(GLFW_KEY_LEFT_SHIFT) && !m_gui->exhausted()) playerSpeed *= 2;
		else if (m_window->getKey(GLFW_KEY_LEFT_CONTROL)) playerSpeed *= 0.2;
		else if (m_window->getKey(GLFW_KEY_TAB)) playerSpeed *= 20;
		if (m_window->getKey(GLFW_KEY_S)) { m_player->vel_y = playerSpeed; moving = true; }
		if (m_window->getKey(GLFW_KEY_D)) { m_player->vel_x = playerSpeed; moving = true; }
		if (m_window->getKey(GLFW_KEY_W)) { m_player->vel_y = -playerSpeed; moving = true; }
		if (m_window->getKey(GLFW_KEY_A)) { m_player->vel_x = -playerSpeed; moving = true; }
		if (moving && running) { m_gui->setStamina(m_gui->getStamina() - 0.01); }
		m_gui->update();
		m_player->update();
		m_inventory->update();

		//Load regions after camera/player moves
#if !STORE_MAP_IN_RAM
		processNewArea();
#endif

		//Player collision
		m_player->collide();

		//World updates
		#if !STORE_MAP_IN_RAM
		for (int x = 0; x < RENDER_DST; x++)
		{
			for (int y = 0; y < RENDER_DST; y++)
			{
				if (!m_region[x][y].null) {
					m_region[x][y].update();
				}
			}
		}
		#else
		m_map->update();
		#endif
	}

	#if !STORE_MAP_IN_RAM
	asyncLoadRegions();
	asyncUnload();

	//Load regions after camera/player moves
	processNewArea();
	for (int x = 0; x < RENDER_DST; x++)
	{
		for (int y = 0; y < RENDER_DST; y++)
		{
			if (tilesChanged) {
				if (m_regionExist[x][y]) m_region[x][y].tilesChanged();
			}
		}
	}
	if (tilesChanged) tilesChanged = false;
	#endif

}

#if !STORE_MAP_IN_RAM
void Game::asyncUnload() {
	if (m_regionUnloadArray.size() > 0) {
		asyncSaveIndex++;
		if (asyncSaveIndex >= m_regionUnloadArray.size()) {
			asyncSaveIndex = 0;
		}
		
		//long long startTime = tools::Clock::getMicroseconds();
		m_regionUnloadArray[asyncSaveIndex].saveTiles();
		//tools::Logger::info(tools::Clock::getMicroseconds() - startTime);
		m_regionUnloadArray.erase(m_regionUnloadArray.begin() + asyncSaveIndex);
	}
}
#endif

void Game::keyPress(int key, int action) {
	if (key == GLFW_KEY_ESCAPE) { paused = !paused; justPaused = true; return; }
	m_gui->keyPress(key, action);

	if (paused) return;

	//Player Hitting
	if (key == GLFW_KEY_UP && !m_gui->exhausted()) { m_player->heading = HEADING_UP; m_player->hit(); m_gui->setStamina(m_gui->getStamina() - 0.2); return; }
	if (key == GLFW_KEY_DOWN && !m_gui->exhausted()) { m_player->heading = HEADING_DOWN; m_player->hit(); m_gui->setStamina(m_gui->getStamina() - 0.2); return; }
	if (key == GLFW_KEY_LEFT && !m_gui->exhausted()) { m_player->heading = HEADING_LEFT; m_player->hit(); m_gui->setStamina(m_gui->getStamina() - 0.2); return; }
	if (key == GLFW_KEY_RIGHT && !m_gui->exhausted()) { m_player->heading = HEADING_RIGHT; m_player->hit(); m_gui->setStamina(m_gui->getStamina() - 0.2); return; }
	if ((key == GLFW_KEY_UP || key == GLFW_KEY_DOWN || key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) && m_gui->exhausted()) { m_gui->setStamina(m_gui->getStamina() - 0.2); return; }
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
#if !STORE_MAP_IN_RAM
		for (int x = 0; x < RENDER_DST; x++)
		{
			for (int y = 0; y < RENDER_DST; y++)
			{
				if (m_regionExist[x][y])  m_region[x][y].debugCeilCreatures();
			}
		}
#else
		m_map->debugCeilCreatures();
#endif
	}
	
	//Reload game
	if (key == GLFW_KEY_F3) {
		close();

#if !STORE_MAP_IN_RAM
		lastRegionX = 0;
		lastRegionY = 0;
#endif
		tilesChanged = true;

		paused = false;
		
		loadGame();

		return;
	}

	//Get FPS
	if (key == GLFW_KEY_F4) tools::Logger::info(std::string("Fps: ") + std::to_string(m_loop->getFPS()));
	
	//Clear inventoy
	if (key == GLFW_KEY_F5) m_inventory->clear();
	
	//Add creature at player
	if (key == GLFW_KEY_F6) m_map->addCreature(m_player->getX(), m_player->getY(), 1, false);
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
	m_player->save();

#if !STORE_MAP_IN_RAM
	int worldData[1] = { seed };
	for (int x = 0; x < RENDER_DST; x++) {
		for (int y = 0; y < RENDER_DST; y++) {
			if (m_regionExist[x][y]) m_region[x][y].saveTiles();
		}
	}
	tools::BinaryIO::write<int>(getSaveLocation() + "world_data", worldData, 1);
#else
	m_map->save();
#endif
#endif

}

void Game::loadGame() {
	//Initializing
	tools::Logger::info("Creating game...");

	//Load seed
#if !DEBUG_DISABLE_SAVING

#if !STORE_MAP_IN_RAM
	void* seedptr = tools::BinaryIO::read<int>(getSaveLocation() + "world_data");
	if (seedptr) seed = ((int*)seedptr)[0];
	else seed = tools::Clock::getMicroseconds();
	tools::Random::seed(seed);
	m_noise = FastNoise(seed);
#else
#endif

#else

#if !STORE_MAP_IN_RAM
	seed = tools::Clock::getMicroseconds();
	tools::Random::seed(seed);
	m_noise = FastNoise(seed);
	tools::Logger::info(std::string("World seed: ") + std::to_string(seed)); // 2058261791
#endif
#endif
	

	m_worldrenderer = std::unique_ptr<graphics::Renderer>(new graphics::Renderer("resources/arial.ttf", m_window.get()));
	m_guirenderer = std::unique_ptr<graphics::Renderer>(new graphics::Renderer("resources/arial.ttf", m_window.get()));

	tools::Logger::info("Loading resources");
	graphics::TextureManager::loadTextureAtlas("resources/atlas.png", GL_RGBA, 0);
	audio::AudioManager::loadAudio("resources/hit.wav", 0);
	audio::AudioManager::loadAudio("resources/swing.wav", 1);
	audio::AudioManager::loadAudio("resources/collect.wav", 2);
	tools::Logger::info("Resources loaded!");

	tools::Logger::info("Loading database");
	Database::init();
	tools::Logger::info("Database loaded!");

	m_inventory = std::unique_ptr<Inventory>(new Inventory(m_shader.get(), m_window.get()));


	//Constructing objects
#if STORE_MAP_IN_RAM
	m_map = std::unique_ptr<Map>(new Map(WORLD_NAME));
#else
	m_region[0][0];
	for (int x = 0; x < RENDER_DST; x++)
	{
		for (int y = 0; y < RENDER_DST; y++)
		{
			m_regionExist[x][y] = false;
		}
	}
#endif
	float playerX = MAP_SIZE / 2.0, playerY = MAP_SIZE / 2.0;

#if !DEBUG_DISABLE_SAVING
#endif
	m_player = std::unique_ptr<Player>(new Player(playerX, playerY, m_inventory.get()));
	m_player->load();

	m_gui = std::unique_ptr<Gui>(new Gui(
		Database::creatures[m_player->getId()].health, 
		Database::creatures[m_player->getId()].stamina, 
		Database::creatures[m_player->getId()].healthgain, 
		Database::creatures[m_player->getId()].staminagain
	));

	m_shader->enable();
	glm::mat4 ml_matrix(1.0f);
	m_shader->SetUniformMat4("ml_matrix", ml_matrix);
	tools::Logger::info("Game ready!");
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


#if !STORE_MAP_IN_RAM
void Game::addRegionToUnloadArray(Region region) {
#if !STORE_MAP_IN_RAM
	m_regionUnloadArray.push_back(region);
#endif
}

void Game::asyncLoadRegions() {
	asyncLoadX++;

	if (asyncLoadX >= RENDER_DST_HALF) {
		asyncLoadX = 0;
		asyncLoadY++;
		if (asyncLoadY >= RENDER_DST_HALF) asyncLoadY = 0;
	}

	float playerX = m_player->getX();
	float playerY = m_player->getY();

	int playerRegionY = posToRegionPos(playerY - REGION_SIZE / 2.0) + 1;
	int playerRegionX = posToRegionPos(playerX - REGION_SIZE / 2.0) + 1;

	//Load regions top left
	if (!m_regionExist[RENDER_DST_HALF - 1 - asyncLoadX][RENDER_DST_HALF - 1 - asyncLoadY]) {
		asyncLoadRegionSection(RENDER_DST_HALF - 1 - asyncLoadX, RENDER_DST_HALF - 1 - asyncLoadY, playerRegionX, playerRegionY);
	}
	else {
		m_region[RENDER_DST_HALF - 1 - asyncLoadX][RENDER_DST_HALF - 1 - asyncLoadY].tilesChanged();
	}

	//Load regions bottom right
	if (!m_regionExist[RENDER_DST_HALF + asyncLoadX][RENDER_DST_HALF + asyncLoadY]) {
		asyncLoadRegionSection(RENDER_DST_HALF + asyncLoadX, RENDER_DST_HALF + asyncLoadY, playerRegionX, playerRegionY);
	}
	else {
		m_region[RENDER_DST_HALF + asyncLoadX][RENDER_DST_HALF + asyncLoadY].tilesChanged();
	}

	//Load regions top right
	if (!m_regionExist[RENDER_DST_HALF + asyncLoadX][RENDER_DST_HALF - 1 - asyncLoadY]) {
		asyncLoadRegionSection(RENDER_DST_HALF + asyncLoadX, RENDER_DST_HALF - 1 - asyncLoadY, playerRegionX, playerRegionY);
	}
	else {
		m_region[RENDER_DST_HALF + asyncLoadX][RENDER_DST_HALF - 1 - asyncLoadY].tilesChanged();
	}

	//Load regions bottom left
	if (!m_regionExist[RENDER_DST_HALF - 1 - asyncLoadX][RENDER_DST_HALF + asyncLoadY]) {
		asyncLoadRegionSection(RENDER_DST_HALF - 1 - asyncLoadX, RENDER_DST_HALF + asyncLoadY, playerRegionX, playerRegionY);
	}
	else {
		m_region[RENDER_DST_HALF - 1 - asyncLoadX][RENDER_DST_HALF + asyncLoadY].tilesChanged();
	}
}

void Game::asyncLoadRegionSection(unsigned int x, unsigned int y, int playerRegionX, int playerRegionY) {
	m_region[x][y] = Region(x + playerRegionX, y + playerRegionY);
	m_regionExist[x][y] = true;
	m_region[x][y].tilesChanged();
}

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
		addRegionToUnloadArray(m_region[x][y]);
	}
#endif
	if (logic::isInRange(x + deltaX, 0, RENDER_DST - 1) && logic::isInRange(y + deltaY, 0, RENDER_DST - 1)) {
		m_region[x][y] = m_region[x + deltaX][y + deltaY];
		m_regionExist[x][y] = m_regionExist[x + deltaX][y + deltaY];
	}
	else {
		m_regionExist[x][y] = false;
	}

	asyncLoadX = 0;
	asyncLoadY = 0;
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

	int playerRegionX = posToRegionPos(m_player->getX() + REGION_SIZE / 2.0);
	int playerRegionY = posToRegionPos(m_player->getY() + REGION_SIZE / 2.0);

	int offsetRegionX = posToRegionPos(round(m_player->getX())) - posToRegionPos(round(m_player->getX() - REGION_SIZE / 2.0)) + 2; //Change last ( + 2) when changing render dst
	int offsetRegionY = posToRegionPos(round(m_player->getY())) - posToRegionPos(round(m_player->getY() - REGION_SIZE / 2.0)) + 2; //Change last ( + 2) when changing render dst

	int finalRegionX = selectedRegionX - playerRegionX + RENDER_DST / 2.0;
	int finalRegionY = selectedRegionY - playerRegionY + RENDER_DST / 2.0;


	if (!logic::isInRange(finalRegionX, 0, RENDER_DST - 1) || !logic::isInRange(finalRegionY, 0, RENDER_DST - 1)) return nullptr;

	int finalX = x - m_region[finalRegionX][finalRegionY].getX();
	int finalY = y - m_region[finalRegionX][finalRegionY].getY();

	if (!logic::isInRange(finalX, 0, REGION_SIZE - 1) || !logic::isInRange(finalY, 0, REGION_SIZE - 1)) return nullptr;
	if (!m_regionExist[finalRegionX][finalRegionY]) return nullptr;

	return &m_region[finalRegionX][finalRegionY];
}

Tile* Game::getTile(float x, float y, std::string debugText) {
	x = round(x); y = round(y);

	Region *regionAt = getRegion(x, y);
	if (!regionAt) return nullptr;

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

#else

void Game::addCreature(float x, float y, int id, bool item) {}

#endif

Player *Game::getPlayer() {
	return m_player.get();
}

Gui *Game::getGui() {
	return m_gui.get();
}

logic::GameLoop *Game::getLoop() {
	return m_loop.get();
}

Map *Game::getMap() {
	return m_map.get();
}

graphics::Shader *Game::getShader() {
	return m_shader.get();
}

graphics::Window *Game::getWindow() {
	return m_window.get();
}

std::string Game::getSaveLocation() {
	return SAVE_PATH + "\\" + WORLD_NAME + "\\";
}