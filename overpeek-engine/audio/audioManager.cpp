#include "audioManager.h"

#include "../utility/logger.h"
#include "../graphics/window.h"

#include <fstream>
#include <string>
#include <al.h>
#include <alc.h>


namespace oe {

	unsigned int AudioManager::sources[8];

	bool isBigEndian()
	{
		int a = 1;
		return !((char*)&a)[0];
	}

	int convertToInt(char* buffer, int len)
	{
		int a = 0;
		if (!isBigEndian())
			for (int i = 0; i < len; i++)
				((char*)&a)[i] = buffer[i];
		else
			for (int i = 0; i < len; i++)
				((char*)&a)[3 - i] = buffer[i];
		return a;
	}

	char* loadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size)
	{
		char buffer[4];
		std::ifstream in(fn, std::ios::binary);
		in.read(buffer, 4);
		if (strncmp(buffer, "RIFF", 4) != 0)
		{
			oe::Logger::out(oe::error, "oe not valid!");
			return nullptr;
		}
		in.read(buffer, 4);
		in.read(buffer, 4);      //WAVE
		in.read(buffer, 4);      //fmt
		in.read(buffer, 4);      //16
		in.read(buffer, 2);      //1
		in.read(buffer, 2);
		chan = convertToInt(buffer, 2);
		in.read(buffer, 4);
		samplerate = convertToInt(buffer, 4);
		in.read(buffer, 4);
		in.read(buffer, 2);
		in.read(buffer, 2);
		bps = convertToInt(buffer, 2);
		in.read(buffer, 4);      //data
		in.read(buffer, 4);
		size = convertToInt(buffer, 4);
		char* data = new char[size];
		in.read(data, size);
		return data;
	}

	void AudioManager::init() {
		oe::Logger::out(oe::info, "Creating oe device");
		ALCdevice* device = alcOpenDevice(NULL);
		if (device == NULL)
		{
			oe::Logger::out(oe::error, "Cannot open sound card!");
			Window::terminate();
		}
		ALCcontext* context = alcCreateContext(device, NULL);
		if (context == NULL)
		{
			oe::Logger::out(oe::error, "Cannot open oe context!");
			Window::terminate();
		}
		alcMakeContextCurrent(context);
		oe::Logger::out(oe::info, "oe device done!");
	}

	void AudioManager::loadAudio(std::string filepath, int id) {
		int channel, sampleRate, bps, size;
		char* data = loadWAV(filepath.c_str(), channel, sampleRate, bps, size);
		unsigned int bufferid, format;
		alGenBuffers(1, &bufferid);
		if (channel == 1)
		{
			if (bps == 8) format = AL_FORMAT_MONO8;
			else format = AL_FORMAT_MONO16;
		}
		else {
			if (bps == 8) format = AL_FORMAT_STEREO8;
			else format = AL_FORMAT_STEREO16;
		}
		alBufferData(bufferid, format, data, size, sampleRate);
		alGenSources(1, &sources[id]);
		alSourcei(sources[id], AL_BUFFER, bufferid);
	}

	void AudioManager::play(int id) {
		//oe::Logger::info("Sound");
		alSourcePlay(sources[id]);
	}
}