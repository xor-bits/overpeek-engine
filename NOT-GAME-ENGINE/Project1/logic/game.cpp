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
	oe::Logger::out("Creating window");
	m_window = std::unique_ptr<oe::Window>(new oe::Window(M_WINDOW_WIDTH, M_WINDOW_HEIGHT, (M_WINDOW_DEFAULT_TITLE).c_str(), WINDOW_MULTISAMPLE_X8 | WINDOW_RESIZEABLE));
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
	m_window->setBackFaceCulling(false);
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
	oe::Logger::out("Creating audio device");
	oe::AudioManager::init();
#endif

	//Create shader
	oe::Logger::out("Creating all the shaders");
	oe::Logger::out("Shader for postprocessing");
	m_postshader = std::unique_ptr<oe::Shader>(new oe::Shader("res/shader/postprocess.vert.glsl", "res/shader/postprocess.frag.glsl"));
	oe::Logger::out("Shader for textures");
	m_shader = std::unique_ptr<oe::Shader>(new oe::Shader("res/shader/texture.vert.glsl", "res/shader/texture.frag.glsl"));
	m_pointshader = std::unique_ptr<oe::Shader>(new oe::Shader("res/shader/geometrytexture.vert.glsl", "res/shader/geometrytexture.frag.glsl", "res/shader/geometrytexture.geom.glsl"));
	oe::Logger::out("All shaders created successfully!");

	//Shader stuff
	resize(m_window->getWidth(), m_window->getHeight());

	//Gameloop
	m_loop = std::unique_ptr<oe::GameLoop>(new oe::GameLoop(render, update, UPDATES_PER_SECOND, m_window.get()));
	m_worldrenderer = std::unique_ptr<oe::Renderer>(new oe::Renderer("res/font/arial.ttf", m_window.get()));
	m_guirenderer = std::unique_ptr<oe::Renderer>(new oe::Renderer("res/font/arial.ttf", m_window.get()));

	//Resources
	oe::Logger::out("Loading resources");
	oe::TextureManager::loadTextureAtlas("res/texture/atlas.png", 0, 16);
	oe::AudioManager::loadAudio("res/audio/hit.wav", 0);
	oe::AudioManager::loadAudio("res/audio/swing.wav", 1);
	oe::AudioManager::loadAudio("res/audio/collect.wav", 2);

	oe::Logger::out("Loading database");
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
	oe::Logger::out("Game ready! Running update and renderloops");
	m_loop->start();
}

void Game::rapidUpdate() {
}

void Game::render(float corrector) {
	//return;
	if (!m_window) return;
	m_window->clear();

	
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

		m_player->submitToRenderer(m_worldrenderer.get(), -m_player->getX() - m_player->getVelX() * corrector / UPDATES_PER_SECOND, -m_player->getY() - m_player->getVelY() * corrector / UPDATES_PER_SECOND, corrector, renderScale());
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


	m_window->update();
	m_window->input();
}

void Game::update() {
	//double mx, my;
	//m_window->getMousePos(mx, my);
	//oe::Logger::out("mouse: ", mx, my);

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
	if (action == OE_PRESS && key == OE_KEY_F3) { oe::Logger::out("Fps: ", m_loop->getFPS()); return; }
	
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
	m_pointshader->enable(); m_pointshader->setUniform1i("ortho", 1);
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
		oe::Logger::out("Couldn't load world");

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

float Game::renderScale() { 
	return 720.0f / m_window->getHeight(); 
}