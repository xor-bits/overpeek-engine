#pragma once

#include "../internal_libs.h"

namespace oe {

	class Audio {
	private:
		unsigned int source_id;
		unsigned int buffer_id;

	public:
		Audio(std::string filepath);

		//Play audio at id
		void play(glm::vec2 position = glm::vec2(0.0f, 0.0f));

	};

}