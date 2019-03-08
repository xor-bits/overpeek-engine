#pragma once

#include <string>

namespace oe {

	class AudioManager {
	private:
		static unsigned int sources[8];

	public:
		//Initialize audio player to be ready to load and play audio
		static void init();

		//Load wav file into id
		static void loadAudio(std::string filepath, int id);
		
		//Play audio at id
		static void play(int id);

	};

}