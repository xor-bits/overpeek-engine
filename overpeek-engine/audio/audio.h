#pragma once

#include "../utility/filereader.h"
#include "../internal_libs.h"

namespace oe {

	class Audio {
	private:
		unsigned int source_id;
		unsigned int buffer_id;

		static bool initialized;

	public:
		static int init();
		static int checkALErrors(int debug_info = 0);

		Audio(audio_data audio);

		//Play audio at id
		void play(glm::vec2 position = glm::vec2(0.0f, 0.0f)) const;

	};

}