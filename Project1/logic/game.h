#pragma once

#include "database.h"
#include <string>

#define DEBUG_ZOOM				1.0
#define ENABLE_AUDIO			true
#define SHOW_DEBUG_MESSAGES		false
#define DEBUG_DISABLE_SAVING	true
#define STORE_MAP_IN_RAM		true

#define M_WINDOW_WIDTH			1280
#define M_WINDOW_HEIGHT			720
#define M_ASPECT				(float)M_WINDOW_WIDTH / (float)M_WINDOW_HEIGHT
#define M_DEFAULT_MULTISAMPLE	4
#define M_WINDOW_DEFAULT_TITLE	"Unnamed Pre-Alpha 0.0.0.0"

#if !STORE_MAP_IN_RAM
#define RENDER_DST				3 * 2
#define RENDER_DST_HALF			RENDER_DST / 2

#define REGION_SIZE				8
#define REGOIN_SIZE_HALF		REGION_SIZE / 2.0
#else
#define RENDER_VERTICAL			8
#define RENDER_HORIZONTAL		RENDER_VERTICAL * Game::getWindow()->getAspect()
#endif

#define TILE_SIZE				0.1

#define USER_PROFILE std::string(getenv("USERPROFILE"))
#define SAVE_PATH std::string(USER_PROFILE + "\\AppData\\Roaming\\_overpeek-game\\")
#define WORLD_NAME std::string("test")

class Map;
class Tile;
class Region;
class Player;
class Creature;
class Gui;
class FastNoise;
namespace logic {class GameLoop;}
namespace graphics {class Window;class Shader;class Renderer;}

class Game {
private:
	static std::unique_ptr<graphics::Window> m_window;
	static std::unique_ptr<graphics::Shader> m_shader;
	static std::unique_ptr<graphics::Shader> m_postshader;
	static std::unique_ptr<graphics::Shader> m_guishader;
	static std::unique_ptr<graphics::Renderer> m_worldrenderer;
	static std::unique_ptr<graphics::Renderer> m_guirenderer;
	static std::unique_ptr<logic::GameLoop> m_loop;
	static std::unique_ptr<Gui> m_gui;
	static std::unique_ptr<Map> m_map;
	static std::unique_ptr<Player> m_player;

	static FastNoise m_biomenoise;
	static FastNoise m_mapnoise;
	static FastNoise m_plantnoise1;
	static FastNoise m_plantnoise2;


#if !STORE_MAP_IN_RAM
	static Region m_region[RENDER_DST][RENDER_DST];
	static bool m_regionExist[RENDER_DST][RENDER_DST];
	static std::vector<Region> m_regionUnloadArray;

	static float lastRegionX;
	static float lastRegionY;
	static int seed;

	static int asyncLoadX;
	static int asyncLoadY;
	static int asyncSaveIndex;


	static void shiftRegions(int deltaX, int deltaY, int playerRegionX, int playerRegionY);
	static void shiftRegionLoop(int x, int y, int deltaX, int deltaY, int playerRegionX, int playerRegionY);
	static void addRegionToUnloadArray(Region region);
	static void asyncLoadRegions();
	static void asyncLoadRegionSection(unsigned int x, unsigned int y, int playerRegionX, int playerRegionY);
	static void asyncUnload();
#endif

	static int hitCooldown;

	static bool justPaused;
	
	static void renderInfoScreen();

	static int getInfoFromNoiseIfLoop(Database::Biome biome, float x, float y, int index);

public:
	static bool tilesChanged;
	static bool debugMode;
	static bool advancedDebugMode;
	static bool paused;

	static void init();
	static void render();
	static void update();
	static void close();
	static void saveGame();
	static void loadGame();
	static void rapidUpdate();

#if !STORE_MAP_IN_RAM
	static void processNewArea();
	static float posToRegionPos(float xOrY);

	static Region *getRegion(float x, float y);
	static Tile *getTile(float x, float y, std::string debugText);

	static bool trySetTileObject(float x, float y, int id);
	static bool trySetTileObject(Tile *tile, int id);
	static void findAllCreatures(float x, float y, Creature** array, unsigned int& amount, float radius);
#endif

	static void keyPress(int key, int action);
	static void buttonPress(int button, int action);
	static void scroll(double y);
	
	static graphics::Shader *getShader();
	static graphics::Window *getWindow();
	static Player *getPlayer();
	static Gui *getGui();
	static logic::GameLoop *getLoop();
	static Map *getMap();

	static void getInfoFromNoise(int &tileId, int &objId, float x, float y);
	static Database::Biome getTileBiome(float x, float y);

	static void addCreature(float x, float y, int id, bool item);
	

	static std::string getSaveLocation();
};