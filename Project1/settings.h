#pragma once


#define VERSION_MAJOR			0
#define VERSION_MINOR			3
#define VERSION_BUG_FIX			0
#define VERSION_CSTR			"0.3.0"



#define UPDATES_PER_SECOND		100


#define DEBUG_ZOOM				1.0f
#define DEBUG_MODE				false
#define ENABLE_AUDIO			true
#define SHOW_DEBUG_MESSAGES		false
#define DEBUG_DISABLE_SAVING	false
#define STORE_MAP_IN_RAM		true

#define M_WINDOW_HEIGHT			720
#define M_WINDOW_WIDTH			(M_WINDOW_HEIGHT * 16 / 9) //1280
#define M_ASPECT				(float)M_WINDOW_WIDTH / (float)M_WINDOW_HEIGHT
#define M_ASPECT_FIXED			false
#define M_DEFAULT_MULTISAMPLE	2
#define M_GAME_NAME				std::string("Unnamed Game")
#define M_WINDOW_DEFAULT_TITLE	M_GAME_NAME + std::string(" Pre-Alpha v") + VERSION_CSTR

#if !STORE_MAP_IN_RAM
#define RENDER_DST				3 * 2
#define RENDER_DST_HALF			RENDER_DST / 2

#define REGION_SIZE				8
#define REGOIN_SIZE_HALF		REGION_SIZE / 2.0
#else
#define RENDER_VERTICAL			16
#define MAP_WORK_DST			32
#define RENDER_HORIZONTAL		RENDER_VERTICAL * Game::getWindow()->getAspect()
#endif

#define TILE_SIZE				0.1f

#define PLAYER_WIDTH			0.8f
#define PLAYER_HEIGHT			0.8f



#define MAP_SIZE 500
#define MAP_MAX_CREATURES 2048

#define APP_DATA std::string(getenv("APPDATA"))
#define SAVE_PATH std::string(APP_DATA + "\\OverpeekEngine")
#define WORLD_NAME std::string("test")