#pragma once

#include <string>

namespace oe {

	class AudioManager {
	private:
		static unsigned int sources[8];

	public:
		static void init();
		static void loadAudio(std::string filepath, int id);
		static void play(int id);

	};

}