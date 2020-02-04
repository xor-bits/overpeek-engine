#pragma once

#include "engine/utility/filereader.h"
#include "engine/internal_libs.h"

namespace oe::audio {

	class Audio {
	private:
		unsigned int source_id;
		unsigned int buffer_id;

	public:
		static void init();
		static void deinit();
		static void checkALErrors();

		Audio(const oe::utils::audio_data& audio);

		//Play audio at id
		void play(glm::vec2 position = glm::vec2(0.0f, 0.0f)) const;

	};

}