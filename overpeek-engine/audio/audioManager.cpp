#include "audioManager.h"
#include "../graphics/window.h"

#include <fstream>
#include <string>
#include <al.h>
#include <alc.h>



namespace oe {

	int AudioManager::init() {
		spdlog::info("Opening audio device");
		ALCdevice* device = alcOpenDevice(NULL);
		if (device == NULL)
		{
			spdlog::error("Cannot open audio device");
			return -1;
		}
		ALCcontext* context = alcCreateContext(device, NULL);
		if (context == NULL)
		{
			spdlog::error("Cannot create OpenAL context");
			return -2;
		}
		alcMakeContextCurrent(context);
		
		spdlog::info("Audio init done");
	}
}