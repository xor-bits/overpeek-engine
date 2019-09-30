#include "audioManager.h"

#include "../utility/logger.h"
#include "../graphics/window.h"

#include <fstream>
#include <string>
#include <al.h>
#include <alc.h>



namespace oe {

	int AudioManager::init() {
		oe::Logger::info("Opening audio device");
		ALCdevice* device = alcOpenDevice(NULL);
		if (device == NULL)
		{
			oe::Logger::error("Cannot open audio device");
			return -1;
		}
		ALCcontext* context = alcCreateContext(device, NULL);
		if (context == NULL)
		{
			oe::Logger::error("Cannot create OpenAL context");
			return -2;
		}
		alcMakeContextCurrent(context);
		
		oe::Logger::info("Audio init done");
	}
}