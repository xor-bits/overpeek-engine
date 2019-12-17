#pragma once

#include "utility/filereader.h"
#include "internal_libs.h"



namespace oe::audio {

	class Audio {
	private:
		void* m_decoder; // ma_decoder
		void* m_device;  // ma_device

		long m_first_frame;
		long m_last_frame;

	public:
		Audio(std::string audio_file);
		~Audio();

		//Play audio at id
		void play(glm::vec2 position = glm::vec2(0.0f, 0.0f)) const;
	};

}