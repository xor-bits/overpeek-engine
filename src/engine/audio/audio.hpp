#pragma once

#include "engine/internal_libs.hpp"



namespace oe::utils { struct audio_data; }

namespace oe::audio
{
	class Audio
	{
	private:
		unsigned int source_id = 0;
		unsigned int buffer_id = 0;

	public:
		static inline bool initialized = false;
		static void init();
		static void deinit();
		static void checkALErrors();

		Audio(const oe::utils::audio_data& audio);

		//Play audio at id
		void play(const glm::vec2& position = { 0.0f, 0.0f }) const;
	};

}