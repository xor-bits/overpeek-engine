#include "audio.h"

#include <al.h>
#include <alc.h>

namespace oe {

	bool Audio::initialized = false;

	int Audio::init() {
		if (initialized) return 0;

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
		return 0;
	}

	Audio::Audio(audio_data audio) {
		alGenBuffers(1, &buffer_id);
		alBufferData(buffer_id, audio.format, audio.data, audio.size, audio.sample_rate);
		alGenSources(1, &source_id);
		alSourcei(source_id, AL_BUFFER, buffer_id);
	}

	void Audio::play(glm::vec2 position) const {
		alSource3f(source_id, AL_POSITION, position.x, position.y, 0.0f);
		alSourcePlay(source_id);
	}

}