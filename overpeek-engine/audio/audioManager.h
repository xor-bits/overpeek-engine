#pragma once

#include <string>
#include <al.h>
#include <alc.h>

#define MAX_SOURCES 8

namespace audio {

	class AudioManager {
	private:
		static ALuint sources[MAX_SOURCES];

	public:
		static void init();
		static void loadAudio(std::string filepath, int id);
		static void play(int id);

	};

}