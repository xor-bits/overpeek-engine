#include "audio.hpp"

#ifdef __EMSCRIPTEN__
#include <AL/al.h>
#include <AL/alc.h>
#elif defined(VCPKG_TOOLCHAIN)
#include <AL/al.h>
#include <AL/alc.h>
#else
#include <al.h>
#include <alc.h>
#endif
#include "engine/engine.hpp"



namespace oe::audio
{
	ALCcontext* context = nullptr;
	ALCdevice* device = nullptr;



	void Audio::init()
	{
		if(initialized)
			return;
			
		// audio device
		const char* device_name = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
		device = alcOpenDevice(device_name);
		if (device == NULL)
			throw std::runtime_error("Cannot open audio device");

		// audio context
		context = alcCreateContext(device, NULL);
		if (context == NULL)
			throw std::runtime_error("Cannot create OpenAL context");
		alcMakeContextCurrent(context);
		initialized = true;
	}

	void Audio::deinit()
	{
		if(!initialized)
			return;

		alcDestroyContext(context);
		alcCloseDevice(device);
	}

	void Audio::checkALErrors()
	{
		if(!initialized)
			return;

		ALCenum error = alGetError();
		if (error == AL_NO_ERROR) return;

		std::string error_string;
		switch (error)
		{
		case AL_NO_ERROR:
			error_string = "There was no error";
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
		
		const std::string formatted_error = fmt::format("OpenAL ({}):\n{}", error, error_string);
		if(Engine::getSingleton().engine_info.ignore_errors)
			spdlog::warn("{}", formatted_error);
		else
			throw std::runtime_error(formatted_error);
	}

	Audio::Audio(const oe::utils::audio_data& audio)
	{
		if(!initialized)
		{
			spdlog::warn("Engine audio was not enabled");
			return;
		}

		alGenBuffers(1, &buffer_id);
		alBufferData(buffer_id, audio.format, audio.data, audio.size, audio.sample_rate);
		checkALErrors();
		alGenSources(1, &source_id);
		alSourcei(source_id, AL_BUFFER, buffer_id);
	}

	void Audio::play(glm::vec2 position) const
	{
		if(!initialized)
			return;

		alSource3f(source_id, AL_POSITION, position.x, position.y, 0.0f);
		alSourcePlay(source_id);
	}

}