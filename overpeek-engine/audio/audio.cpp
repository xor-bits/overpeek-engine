#include "audio.h"
#include "audioManager.h"

#include <stb_vorbis.c>
#include <al.h>
#include <alc.h>

namespace oe {

	Audio::Audio(const char* filepath) {
		AudioManager::init();
		int channels, sampleRate;
		short* data;
		int size = stb_vorbis_decode_filename(filepath, &channels, &sampleRate, &data);
		if (size <= 0) {
			spdlog::error("Failed to load audiofile \"{}\"", std::string(filepath));
			return;
		}

		// Format
		int format = -1;
		if (channels == 1) {
			format = AL_FORMAT_MONO16;
		}
		else if (channels == 2) {
			format = AL_FORMAT_STEREO16;
		}

		alGenBuffers(1, &buffer_id);
		alBufferData(buffer_id, format, data, size, sampleRate);
		alGenSources(1, &source_id);
		alSourcei(source_id, AL_BUFFER, buffer_id);

		delete data;
	}

	void Audio::play(glm::vec2 position) {
		alSource3f(source_id, AL_POSITION, position.x, position.y, 0.0f);
		alSourcePlay(source_id);
	}

}