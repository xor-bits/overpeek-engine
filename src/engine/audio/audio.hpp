#pragma once

#include "engine/utility/filereader.hpp"
#include "engine/internal_libs.hpp"

namespace oe::audio {

	class Audio {
	private:
		unsigned int source_id;
		unsigned int buffer_id;

	public:
		static bool init();
		static void deinit();
		static void checkALErrors();

		Audio(const oe::utils::audio_data& audio);

		//Play audio at id
		void play(glm::vec2 position = glm::vec2(0.0f, 0.0f)) const;

	};

}