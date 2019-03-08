#pragma once

#include "database.h"
#include "../settings.h"
#include <string>

class Map;
class Tile;
class Region;
class Player;
class Creature;
class Gui;
namespace oe {class GameLoop;}
namespace oe {class Window;class Shader;class Renderer;}

class Game {
private:
	static std::unique_ptr<oe::Window> m_window;
	static std::unique_ptr<oe::Shader> m_shader;
	static std::unique_ptr<oe::Shader> m_pointshader;
	static std::unique_ptr<oe::Shader> m_postshader;
	static std::unique_ptr<oe::Shader> m_guishader;
	static std::unique_ptr<oe::Renderer> m_worldrenderer;
	static std::unique_ptr<oe::Renderer> m_guirenderer;
	static std::unique_ptr<oe::GameLoop> m_loop;
	static std::unique_ptr<Gui> m_gui;
	static std::unique_ptr<Map> m_map;
	static std::unique_ptr<Player> m_player;


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

	static bool justPaused;

public:
	static bool tilesChanged;
	static bool debugMode;
	static bool advancedDebugMode;
	static bool paused;
	static bool mainMenu;

	static void init();
	static void render(float corrector);
	static void update();
	static void close();
	static void rapidUpdate();

#if !STORE_MAP_IN_RAM
	static void processNewArea();
	static float posToRegionPos(float xOrY);

	static Region *getRegion(float x, float y);
	static Tile *getTile(float x, float y, std::string debugText);

	static bool trySetTileObject(float x, float y, int id);
	static bool trySetTileObject(Tile *tile, int id);
	static void findAllCreatures(float x, float y, Creature** array, unsigned int& amount, float radius);
	static void addCreature(float x, float y, int id, bool item);
#endif

	static void resize(int width, int height);
	static void keyPress(int key, int action);
	static void buttonPress(int button, int action);
	static void scroll(double y);
	static void charmod(unsigned int codepoint, int mods);
	
	static oe::Shader *getShader();
	static oe::Window *getWindow();
	static oe::GameLoop *getLoop();

	//Loads world from path to active world
	//create is false when you dont want to generate new world when load fails
	static void loadWorld(const char* name, bool create);
	
	//Saves active world
	static void saveWorld();

	static Player *getPlayer();
	static Gui *getGui();
	static Map *getMap();

	static std::string getSaveLocation();

	inline static float renderScale() { return 720.0f / m_window->getHeight(); }
};