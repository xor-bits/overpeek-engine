#include "audio.hpp"

#include "engine/engine.hpp"
#include "engine/utility/formatted_error.hpp"
#include "engine/utility/fileio.hpp"



// ignore external warnings
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#elif _MSC_VER
#pragma warning( push )
#endif

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

// ignore external warnings
#ifdef __clang__
#pragma clang diagnostic pop
#elif __GNUC__
#pragma GCC diagnostic pop
#elif _MSC_VER
#pragma warning( pop )
#endif

namespace oe::audio
{
	AudioDevice::AudioDevice()
	{

	}

	AudioPlayer::AudioPlayer()
	{

	}




	/*void Audio::init()
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

		std::string_view error_string = "There was no error";
		switch (error)
		{
		case AL_NO_ERROR: break;
		case AL_INVALID_NAME: error_string = "Invalid name parameter"; break;
		case AL_INVALID_ENUM: error_string = "Invalid parameter"; break;
		case AL_INVALID_VALUE: error_string = "Invalid enum parameter value"; break;
		case AL_INVALID_OPERATION: error_string = "Illegal call"; break;
		case AL_OUT_OF_MEMORY: error_string = "Unable to allocate memory"; break;
		}
		
		if(!Engine::getSingleton().engine_info.ignore_errors)
			throw oe::utils::formatted_error("OpenAL ({}):\n{}", error, error_string);
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

	void Audio::play(const glm::vec2& position) const
	{
		if(!initialized)
			return;

		alSource3f(source_id, AL_POSITION, position.x, position.y, 0.0f);
		alSourcePlay(source_id);
	}*/
}