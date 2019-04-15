#include "../pch.h"


#include "game.h"

#include "../creatures/player.h"
#include "../world/region.h"
#include "../world/map.h"
#include "../logic/inventory.h"
#include "gui.h"
#include "database.h"
#include "mainmenu.h"

std::unique_ptr<oe::Window> Game::m_window;
std::unique_ptr<oe::Shader> Game::m_shader;
std::unique_ptr<oe::Shader> Game::m_pointshader;
std::unique_ptr<oe::Shader> Game::m_postshader;
std::unique_ptr<oe::Shader> Game::m_guishader;
std::unique_ptr<oe::Renderer> Game::m_worldrenderer;
std::unique_ptr<oe::Renderer> Game::m_guirenderer;
std::unique_ptr<oe::GameLoop> Game::m_loop;
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

bool Game::debugMode = true;
bool Game::advancedDebugMode = false;
bool Game::tilesChanged = true;
bool Game::justPaused = true;
bool Game::paused = false;
bool Game::mainMenu = false;


std::string renderer;

void Game::init() {

	//Window
	oe::Logger::out(oe::info, "Creating window");
	m_window = std::unique_ptr<oe::Window>(new oe::Window(M_WINDOW_WIDTH, M_WINDOW_HEIGHT, (M_WINDOW_DEFAULT_TITLE).c_str(), false, M_DEFAULT_MULTISAMPLE, !M_ASPECT_FIXED));
	m_window->setSwapInterval(NULL);
	m_window->setButtonCallback(buttonPress);
	m_window->setKeyboardCallback(keyPress);
	m_window->setScrollCallback(scroll);
	m_window->setResizeCallback(resize);
	m_window->setCharmodCallback(charmod);
	m_window->setSwapInterval(0);
	m_window->setClearColor(1.0, 1.0, 1.0, 1.0);
	m_window->setIcon("res/texture/icon.png");
	m_window->setLineWidth(5.0f);
	//m_window->setBackFaceCulling(false);
	renderer = m_window->getRenderer();


	//Loading screen (barely noticeable lol)
	oe::Shader *tmpshader = new oe::Shader("res/shader/texture-single.vert.glsl", "res/shader/texture-single.frag.glsl");
	glm::mat4 projection = glm::ortho(-m_window->getAspect(), m_window->getAspect(), 1.0f, -1.0f);
	tmpshader->enable(); tmpshader->SetUniformMat4("pr_matrix", projection);
	m_guirenderer = std::unique_ptr<oe::Renderer>(new oe::Renderer("res/font/arial.ttf", m_window.get()));
	oe::TextureManager::loadTexture("res/texture/splash.png", 3);
	m_window->clear();
	glm::vec3 pos = glm::vec3(-0.5f, -0.5f, 0.0f);
	glm::vec2 size = glm::vec2(1.0);
	m_guirenderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y, pos.z), glm::vec2(0.0f, 0.0f), 0, OE_COLOR_WHITE));
	m_guirenderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y + size.y, pos.z), glm::vec2(0.0f, 1.0f), 0, OE_COLOR_WHITE));
	m_guirenderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x + size.x, pos.y + size.y, pos.z), glm::vec2(1.0f, 1.0f), 0, OE_COLOR_WHITE));
	m_guirenderer->quadRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x + size.x, pos.y, pos.z), glm::vec2(1.0f, 0.0f), 0, OE_COLOR_WHITE));

	m_guirenderer->draw(tmpshader, tmpshader, oe::TextureManager::getTexture(3), false);
	m_window->update();
	delete tmpshader;

	//Audio
#if ENABLE_AUDIO
	oe::Logger::out(oe::info, "Creating audio device");
	oe::AudioManager::init();
#endif

	//Create shader
	oe::Logger::out(oe::info, "Creating all the shaders");
	oe::Logger::out(oe::info, "Shader for postprocessing");
	m_postshader = std::unique_ptr<oe::Shader>(new oe::Shader("res/shader/postprocess.vert.glsl", "res/shader/postprocess.frag.glsl"));
	oe::Logger::out(oe::info, "Shader for textures");
	m_shader = std::unique_ptr<oe::Shader>(new oe::Shader("res/shader/texture.vert.glsl", "res/shader/texture.frag.glsl"));
	m_pointshader = std::unique_ptr<oe::Shader>(new oe::Shader("res/shader/geometrytexture.vert.glsl", "res/shader/geometrytexture.frag.glsl", "res/shader/geometrytexture.geom.glsl"));
	oe::Logger::out(oe::info, "All shaders created successfully!");

	//Shader stuff
	resize(m_window->getWidth(), m_window->getHeight());

	//Gameloop
	m_loop = std::unique_ptr<oe::GameLoop>(new oe::GameLoop(render, update, UPDATES_PER_SECOND, m_window.get()));
	m_worldrenderer = std::unique_ptr<oe::Renderer>(new oe::Renderer("res/font/arial.ttf", m_window.get()));
	m_guirenderer = std::unique_ptr<oe::Renderer>(new oe::Renderer("res/font/arial.ttf", m_window.get()));

	//Resources
	oe::Logger::out(oe::info, "Loading resources");
	oe::TextureManager::loadTextureAtlas("res/texture/atlas.png", 0, 16);
	oe::AudioManager::loadAudio("res/audio/hit.wav", 0);
	oe::AudioManager::loadAudio("res/audio/swing.wav", 1);
	oe::AudioManager::loadAudio("res/audio/collect.wav", 2);

	oe::Logger::out(oe::info, "Loading database");
	Database::init();

	//Constructing objects
	float playerX = MAP_SIZE / 2.0, playerY = MAP_SIZE / 2.0;
	m_inventory = std::unique_ptr<Inventory>(new Inventory(m_shader.get(), m_window.get()));
	m_player = std::unique_ptr<Player>(new Player(playerX, playerY, m_inventory.get()));
	m_map = std::unique_ptr<Map>(new Map());

	m_gui = std::unique_ptr<Gui>(new Gui(
		Database::creatures[m_player->getId()].health,
		Database::creatures[m_player->getId()].stamina,
		Database::creatures[m_player->getId()].healthgain,
		Database::creatures[m_player->getId()].staminagain
	));

	//Main menu
	mainMenu = true;
	MainMenu::init(m_guirenderer.get(), m_shader.get(), m_pointshader.get(), m_postshader.get());

	//Reset shader
	resize(m_window->getWidth(), m_window->getHeight());

	//Loading world
	loadWorld(WORLD_NAME, true);

	//Ready
	oe::Logger::out(oe::info, "Game ready! Running update and renderloops");
	m_loop->start();
}

void Game::rapidUpdate() {
}

void Game::render(float corrector) {
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
		m_map->submitToRenderer(m_worldrenderer.get(), -m_player->getX() - m_player->getVelX() * corrector / UPDATES_PER_SECOND, -m_player->getY() - m_player->getVelY() * corrector / UPDATES_PER_SECOND, corrector);
#endif

		m_player->submitToRenderer(m_worldrenderer.get(), -m_player->getX() - m_player->getVelX() * corrector / UPDATES_PER_SECOND, -m_player->getY() - m_player->getVelY() * corrector / UPDATES_PER_SECOND, corrector);
		m_gui->renderBlur(m_worldrenderer.get());
		m_inventory->render(m_worldrenderer.get());
	}
	
	//Gui
	m_gui->renderNoBlur(m_guirenderer.get());

	//Flush
	if (!paused) {
		m_worldrenderer->draw(m_shader.get(), m_pointshader.get(), oe::TextureManager::getTexture(0), true);
	}
	else if (justPaused) {
		m_worldrenderer->drawToFramebuffer(m_shader.get(), m_pointshader.get(), oe::TextureManager::getTexture(0), true, false);
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
	
	m_guirenderer->draw(m_shader.get(), m_pointshader.get(), oe::TextureManager::getTexture(0), true);

	//Other
	justPaused = false;
}

void Game::update() {
	//double mx, my;
	//m_window->getMousePos(mx, my);
	//oe::Logger::out(oe::info, "mouse: ", mx, my);

	if (!m_window) return;

	if (m_window->close()) m_loop->stop();
	if (!paused) {
		//Player movement
		if (!m_gui->chatOpened()) {
			float playerSpeed = Database::creatures[0].walkSpeed / UPDATES_PER_SECOND;

			bool running = false;
			bool moving = false;
			if (m_window->getKey(OE_KEY_LEFT_SHIFT)) running = true;
			if (m_window->getKey(OE_KEY_LEFT_SHIFT) && !m_player->exhausted()) playerSpeed *= 2;
			else if (m_window->getKey(OE_KEY_LEFT_CONTROL)) playerSpeed *= 0.2;
			else if (m_window->getKey(OE_KEY_TAB)) playerSpeed *= 20;
			if (m_window->getKey(OE_KEY_S)) { m_player->acc_y = playerSpeed; moving = true; }
			if (m_window->getKey(OE_KEY_D)) { m_player->acc_x = playerSpeed; moving = true; }
			if (m_window->getKey(OE_KEY_W)) { m_player->acc_y = -playerSpeed; moving = true; }
			if (m_window->getKey(OE_KEY_A)) { m_player->acc_x = -playerSpeed; moving = true; }
			if (moving && running) { m_player->setStamina(m_player->getStamina() - 1 / UPDATES_PER_SECOND); }
		}
		m_player->update(UPDATES_PER_SECOND);
		m_player->collide(UPDATES_PER_SECOND);

		m_gui->update(UPDATES_PER_SECOND);
		m_inventory->update();

		//Load regions after camera/player moves
#if !STORE_MAP_IN_RAM
		processNewArea();
#endif

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
		m_map->update(UPDATES_PER_SECOND);
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
		
		//long long startTime = oe::Clock::getMicroseconds();
		m_regionUnloadArray[asyncSaveIndex].saveTiles();
		//oe::Logger::info(oe::Clock::getMicroseconds() - startTime);
		m_regionUnloadArray.erase(m_regionUnloadArray.begin() + asyncSaveIndex);
	}
}
#endif

void Game::keyPress(int key, int action) {
	if (mainMenu) MainMenu::key(key, action);


	m_gui->keyPress(key, action);
	if (action == OE_PRESS && key == OE_KEY_ESCAPE) { paused = !paused; justPaused = true; return; }

	if (m_gui->chatOpened()) return;
	//Postshader
	if (action == OE_PRESS && key == OE_KEY_F7) { m_postshader->enable(); m_postshader->setUniform1i("unif_lens", 0); justPaused = true; return; }
	if (action == OE_PRESS && key == OE_KEY_F8) { m_postshader->enable(); m_postshader->setUniform1i("unif_lens", 1); justPaused = true; return; }

	if (paused) return;

	//Player Hitting
	if (action == OE_PRESS && key == OE_KEY_UP) { m_player->heading = HEADING_UP; m_player->hit();return; }
	if (action == OE_PRESS && key == OE_KEY_DOWN) { m_player->heading = HEADING_DOWN; m_player->hit(); return; }
	if (action == OE_PRESS && key == OE_KEY_LEFT) { m_player->heading = HEADING_LEFT; m_player->hit(); return; }
	if (action == OE_PRESS && key == OE_KEY_RIGHT) { m_player->heading = HEADING_RIGHT; m_player->hit(); return; }
	if (action == OE_PRESS && key == OE_KEY_E) { m_player->setX(round(m_player->getX() + 0.5) - 0.5); m_player->setY(round(m_player->getY() + 0.5) - 0.5); return; }

	//Inventory
	if (action == OE_PRESS && key == OE_KEY_R) { m_inventory->visible = !m_inventory->visible; return; }
	if (action == OE_PRESS && key == OE_KEY_ESCAPE) { m_inventory->visible = false; return; }

	//Inventory slot selecting
	if (action == OE_PRESS && key == OE_KEY_1) { m_inventory->selectedSlot = 0; return; }
	else if (action == OE_PRESS && key == OE_KEY_2) { m_inventory->selectedSlot = 1; return; }
	else if (action == OE_PRESS && key == OE_KEY_3) { m_inventory->selectedSlot = 2; return; }
	else if (action == OE_PRESS && key == OE_KEY_4) { m_inventory->selectedSlot = 3; return; }
	else if (action == OE_PRESS && key == OE_KEY_5) { m_inventory->selectedSlot = 4; return; }

	//Debug commands
	//Activate debug and advanced debug modes
	if (action == OE_PRESS && key == OE_KEY_F1) {
		debugMode = !debugMode; 
		if (m_window->getKey(OE_KEY_LEFT_SHIFT)) {
			debugMode = true;
			advancedDebugMode = debugMode;
		}
		else advancedDebugMode = false;
		return; 
	}

	//Debug ceil creaures
	if (action == OE_PRESS && key == OE_KEY_F2) {
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

	//Get FPS
	if (action == OE_PRESS && key == OE_KEY_F3) { oe::Logger::out(oe::info, "Fps: ", m_loop->getFPS()); return; }
	
	//Clear inventoy
	if (action == OE_PRESS && key == OE_KEY_F4) { m_inventory->clear(); return; }
	
	//Add creature at player
	if (action == OE_PRESS && key == OE_KEY_F5) { m_map->addCreature(m_player->getX(), m_player->getY() + 5, 1, false); return; }
}

void Game::buttonPress(int button, int action) {
#if USE_MOUSE
	if (mainMenu) MainMenu::button(button, action);

	m_player->mouseHit(button, action);
#endif
}

void Game::scroll(double y) {
	//Inventory slot scrolling
	if (y < 0) m_inventory->selectedSlot++;
	if (y > 0) m_inventory->selectedSlot--;
}

void Game::resize(int width, int height) {
	float aspect = float(width) / float(height);
	m_postshader->enable();
	m_postshader->setUniform1i("unif_effect", 0);
	glm::mat4 orthographic = glm::ortho(-aspect * DEBUG_ZOOM, aspect * DEBUG_ZOOM, DEBUG_ZOOM, -DEBUG_ZOOM);
	m_shader->enable(); m_shader->SetUniformMat4("pr_matrix", orthographic);
	m_pointshader->enable(); m_pointshader->SetUniformMat4("pr_matrix", orthographic);
}

void Game::charmod(unsigned int codepoint, int mods) {
	m_gui->typing(codepoint, mods);
}

void Game::close() {
	saveWorld();
}

//Saves loaded world
void Game::loadWorld(const char* name, bool create) {
	if (!m_map->load(name)) {
		oe::Logger::out(oe::info, "Couldn't load world");

		if (create) {
			m_map->create(name);
		}
	}
	else {
		m_player->load();
	}
}

//Loads world
void Game::saveWorld() {
	//Create save location
	std::string strcommand("mkdir \"" + m_map->saveLocation(getSaveLocation()) + "\"" + " >nul 2>&1");
	system(strcommand.c_str());

#if !DEBUG_DISABLE_SAVING
	m_map->save();
	m_player->save();
#endif
	m_gui->addChatLine("World saved");
}





//long long startTime = oe::Clock::getMicroseconds();
//oe::Logger::info(oe::Clock::getMicroseconds() - startTime);
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
	if (!oe::isInRange(x - deltaX, 0, RENDER_DST - 1) || !oe::isInRange(y - deltaY, 0, RENDER_DST - 1)) {
		addRegionToUnloadArray(m_region[x][y]);
	}
#endif
	if (oe::isInRange(x + deltaX, 0, RENDER_DST - 1) && oe::isInRange(y + deltaY, 0, RENDER_DST - 1)) {
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


	if (!oe::isInRange(finalRegionX, 0, RENDER_DST - 1) || !oe::isInRange(finalRegionY, 0, RENDER_DST - 1)) return nullptr;

	int finalX = x - m_region[finalRegionX][finalRegionY].getX();
	int finalY = y - m_region[finalRegionX][finalRegionY].getY();

	if (!oe::isInRange(finalX, 0, REGION_SIZE - 1) || !oe::isInRange(finalY, 0, REGION_SIZE - 1)) return nullptr;
	if (!m_regionExist[finalRegionX][finalRegionY]) return nullptr;

	return &m_region[finalRegionX][finalRegionY];
}

Tile* Game::getTile(float x, float y, std::string debugText) {
	x = round(x); y = round(y);

	Region *regionAt = getRegion(x, y);
	if (!regionAt) return nullptr;

	int finalX = x - regionAt->getX();
	int finalY = y - regionAt->getY();

	if (!oe::isInRange(finalX, 0, REGION_SIZE - 1) || !oe::isInRange(finalY, 0, REGION_SIZE - 1)) {
#if !SHOW_DEBUG_MESSAGES
		return nullptr;
#endif

		if (debugMode) oe::Logger::warning("Finals out of range!");
		if (debugMode) oe::Logger::warning(debugText);
		if (debugMode) oe::Logger::warning(std::to_string(x) + std::string(", ") + std::to_string(y));
		if (debugMode) oe::Logger::warning(std::to_string(regionAt->getX()) + std::string(", ") + std::to_string(regionAt->getY()));
		if (debugMode) oe::Logger::warning(std::to_string(finalX) + std::string(", ") + std::to_string(finalY));

		return nullptr;
	}

	Tile *tile = regionAt->getTile(finalX, finalY);
	if (!tile) {
#if !SHOW_DEBUG_MESSAGES
		return nullptr;
#endif

		if (debugMode) oe::Logger::error(std::string("region returned nullptr instead of tile"));

		return nullptr;
	}
	if (tile->getObjectId() > COUNT_OBJECTS || tile->getObjectId() < 0) {
#if !SHOW_DEBUG_MESSAGES
		return nullptr;
#endif

		if (debugMode) oe::Logger::error(std::string("getTile error at ") + std::to_string(finalX) + std::string(", ") + std::to_string(finalY));

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

		oe::Logger::info("Ending results not as expected");
		oe::Logger::info(std::string("selectedRegion ") + std::to_string(selectedRegionX) + std::string(", ") + std::to_string(selectedRegionY));
		oe::Logger::info(std::string("playerRegion ") + std::to_string(playerRegionX) + std::string(", ") + std::to_string(playerRegionY));
		oe::Logger::info(std::string("offsetRegion ") + std::to_string(offsetRegionX) + std::string(", ") + std::to_string(offsetRegionY));
		oe::Logger::info(std::string("finalRegion ") + std::to_string(finalRegionX) + std::string(", ") + std::to_string(finalRegionY));
		oe::Logger::info(std::string("playerPos ") + std::to_string(round(m_player->getX())) + std::string(", ") + std::to_string(round(m_player->getY())));
		oe::Logger::info(std::string("cursorPos ") + std::to_string(x) + std::string(", ") + std::to_string(y));

		oe::Logger::info(std::string("FinalXY ") + std::to_string(finalX) + std::string(", ") + std::to_string(finalY));
		oe::Logger::info(std::string("tile getXY ") + std::to_string(tile->getX()) + std::string(", ") + std::to_string(tile->getY()));
		oe::Logger::info(std::string("debug text: ") + debugText);

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

#endif

Player *Game::getPlayer() {
	return m_player.get();
}

Gui *Game::getGui() {
	return m_gui.get();
}

oe::GameLoop *Game::getLoop() {
	return m_loop.get();
}

Map *Game::getMap() {
	return m_map.get();
}

oe::Shader *Game::getShader() {
	return m_shader.get();
}

oe::Window *Game::getWindow() {
	return m_window.get();
}

std::string Game::getSaveLocation() {
	return SAVE_PATH + "\\";
}