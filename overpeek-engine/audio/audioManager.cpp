#include "audioManager.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <GLFW/glfw3.h>

namespace audio {

	ALuint AudioManager::sources[MAX_SOURCES];

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
			std::cout << "this is not a valid WAVE file" << std::endl;
			return NULL;
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
		ALCdevice* device = alcOpenDevice(NULL);
		if (device == NULL)
		{
			std::cout << "ERROR Cannot open sound card!" << std::endl;
			glfwTerminate();
			system("pause");
			exit(EXIT_FAILURE);
		}
		ALCcontext* context = alcCreateContext(device, NULL);
		if (context == NULL)
		{
			std::cout << "ERROR Cannot open context!" << std::endl;
			glfwTerminate();
			system("pause");
			exit(EXIT_FAILURE);
		}
		alcMakeContextCurrent(context);
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
		alSourcePlay(sources[id]);
	}
}