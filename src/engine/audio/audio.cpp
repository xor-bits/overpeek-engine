#include "audio.hpp"

#include <al.h>
#include <alc.h>
#include "engine/engine.hpp"



namespace oe::audio {

	ALCcontext* context = nullptr;
	ALCdevice* device = nullptr;



	bool Audio::init() {
		// audio device
		const char* device_name = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
		device = alcOpenDevice(device_name);
		if (device == NULL) {
			spdlog::critical("Cannot open audio device");
			return false;
		}

		// audio context
		context = alcCreateContext(device, NULL);
		if (context == NULL) {
			spdlog::critical("Cannot create OpenAL context");
			return false;
		}
		alcMakeContextCurrent(context);
		return true;
	}

	void Audio::deinit() {
		alcDestroyContext(context);
		alcCloseDevice(device);
	}

	void Audio::checkALErrors() {
		ALCenum error = alGetError();
		if (error == AL_NO_ERROR) return;

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

		oe_error_terminate("OpenAL ({}):\n{}", error, error_string);
	}

	Audio::Audio(const oe::utils::audio_data& audio) {
		alGenBuffers(1, &buffer_id);
		alBufferData(buffer_id, audio.format, audio.data, audio.size, audio.sample_rate);
		checkALErrors();
		alGenSources(1, &source_id);
		alSourcei(source_id, AL_BUFFER, buffer_id);
	}

	void Audio::play(glm::vec2 position) const {
		alSource3f(source_id, AL_POSITION, position.x, position.y, 0.0f);
		alSourcePlay(source_id);
	}

}