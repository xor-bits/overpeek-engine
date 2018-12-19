#pragma once

#include "../../engine.h"
#include "database.h"

#define DEBUG_ZOOM				1.0
#define ENABLE_AUDIO			true
#define SHOW_DEBUG_MESSAGES		false
#define DEBUG_DISABLE_SAVING	false

#define M_WINDOW_WIDTH			1280
#define M_WINDOW_HEIGHT			720
#define M_ASPECT				(float)M_WINDOW_WIDTH / (float)M_WINDOW_HEIGHT
#define M_DEFAULT_MULTISAMPLE	4
#define M_WINDOW_DEFAULT_TITLE	"Unnamed Pre-Alpha 0.0.0.0"


#define BIOME_SCRAMBLE1			-120.5
#define BIOME_SCRAMBLE2			500.3
#define RENDER_DST				4 * 2
#define RENDER_DST_HALF			RENDER_DST / 2

#define REGION_SIZE				8
#define REGOIN_SIZE_HALF		REGION_SIZE / 2.0
#define TILE_SIZE				0.08

#define USER_PROFILE std::string(getenv("USERPROFILE"))
#define SAVE_PATH std::string(USER_PROFILE + "\\AppData\\Roaming\\_overpeek-game\\")
#define WORLD_NAME std::string("test")

class Tile;
class Region;
class Player;
class Creature;
class Game {
private:
	static graphics::Window *m_window;
	static graphics::Shader *m_shader;
	static graphics::Shader *m_postshader;
	static graphics::Shader *m_guishader;
	static graphics::Renderer *m_worldrenderer;
	static graphics::Renderer *m_creaturerenderer;
	static graphics::Renderer *m_guirenderer;
	static logic::GameLoop *m_loop;
	static glm::ivec2 m_hover_tile;

	static Region m_region[RENDER_DST][RENDER_DST];
	static bool m_regionExist[RENDER_DST][RENDER_DST];
	static std::vector<Region> m_regionUnloadArray;
	static Player *m_player;

	static float lastRegionX;
	static float lastRegionY;
	static int seed;

	static int hitCooldown;
	static int asyncLoadX;
	static int asyncLoadY;
	static int asyncSaveIndex;
	
	static void renderInfoScreen();
	static void shiftRegions(int deltaX, int deltaY, int playerRegionX, int playerRegionY);
	static void shiftRegionLoop(int x, int y, int deltaX, int deltaY, int playerRegionX, int playerRegionY);
	static void addRegionToUnloadArray(Region region);
	static void asyncLoadRegions();
	static void asyncLoadRegionSection(unsigned int x, unsigned int y);

public:
	static bool tilesChanged;
	static bool debugMode;
	static bool advancedDebugMode;

	static void init();
	static void render();
	static void update();
	static void close();
	static void saveGame();
	static void loadGame();
	static void rapidUpdate();
	static void processNewArea();

	static void keyPress(int key, int action);
	static void buttonPress(int button, int action);
	static void scroll(double y);
	
	inline static graphics::Shader *getShader() { return m_shader; }
	inline static graphics::Window *getWindow() { return m_window; }

	static void getInfoFromNoise(int &tileId, int &objId, float x, float y);
	static float getTileBiome(float x, float y, float z);

	static float posToRegionPos(float xOrY);
	static void addCreature(float x, float y, int id, bool item);
	
	static Region *getRegion(float x, float y);
	static Tile *getTile(float x, float y, std::string debugText);
	static void findAllCreatures(float x, float y, Creature** array, unsigned int& amount, float radius);
	static Player *getPlayer();

	static bool trySetTileObject(float x, float y, int id);
	static bool trySetTileObject(Tile *tile, int id);

	static std::string getSaveLocation();
};