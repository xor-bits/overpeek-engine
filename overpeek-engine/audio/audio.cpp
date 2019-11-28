#include "audio.h"

#include <al.h>
#include <alc.h>

namespace oe {

	bool Audio::initialized = false;

	int Audio::init() {
		if (initialized) return 0;

		// audio device
		const char* device_name = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
		checkALErrors(__LINE__);
		ALCdevice* device = alcOpenDevice(device_name);
		checkALErrors(__LINE__);
		if (device == NULL) {
			spdlog::error("Cannot open audio device");
			return -1;
		}
		checkALErrors(__LINE__);

		// audio context
		ALCcontext* context = alcCreateContext(device, NULL);
		if (context == NULL) {
			spdlog::error("Cannot create OpenAL context");
			return -2;
		}
		alcMakeContextCurrent(context);
		checkALErrors(__LINE__);

		// post init
		spdlog::info("Audio device opened");
		initialized = true;
		return 0;
	}

	int Audio::checkALErrors(int debug_info) {
		ALCenum error = alGetError();
		if (error == AL_NO_ERROR) return 0;

		std::string error_string;
		switch (error)
		{
		case AL_NO_ERROR:
			error_string = "There is no current error";
			break;
		case AL_INVALID_NAME:
			error_string = "Invalid name parameter";
			break;
		case AL_INVALID_ENUM:
			error_string = "Invalid parameter";
			break;
		case AL_INVALID_VALUE:
			error_string = "Invalid enum parameter value";
			break;
		case AL_INVALID_OPERATION:
			error_string = "Illegal call";
			break;
		case AL_OUT_OF_MEMORY:
			error_string = "Unable to allocate memory";
			break;
		}

		if (debug_info == 0)
			spdlog::error("OpenAL ({}):\n{}", error, error_string);
		else 
			spdlog::error("(Line {}) OpenAL ({}):\n{}", debug_info, error, error_string);
		return error;
	}

	Audio::Audio(audio_data audio) {
		checkALErrors(__LINE__);
		alGenBuffers(1, &buffer_id);
		checkALErrors(__LINE__);
		alBufferData(buffer_id, audio.format, audio.data, audio.size, audio.sample_rate);
		checkALErrors(__LINE__);
		checkALErrors(__LINE__);
		alGenSources(1, &source_id);
		checkALErrors(__LINE__);
		alSourcei(source_id, AL_BUFFER, buffer_id);
		checkALErrors(__LINE__);
	}

	void Audio::play(glm::vec2 position) const {
		//alSource3f(source_id, AL_POSITION, position.x, position.y, 0.0f);
		alSourcePlay(source_id);

		checkALErrors(__LINE__);
	}

}