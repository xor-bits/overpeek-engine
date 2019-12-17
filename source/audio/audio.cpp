#include "audio.h"

#include "utility/debug.h"

#define DR_MP3_IMPLEMENTATION
#include <extras/dr_mp3.h>
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>



namespace oe::audio {

	void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
	{
		ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
		if (pDecoder == NULL) {
			return;
		}

		ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount);

		(void)pInput;
	}

	Audio::Audio(std::string audio_file) {
		ma_result result;
		m_decoder = new ma_decoder();
		m_device = new ma_device();

		// open audiofile
		result = ma_decoder_init_file(audio_file.c_str(), NULL, (ma_decoder*)m_decoder);
		if (result != MA_SUCCESS) {
			spdlog::error("Decode failed");
			return;
		}
		m_first_frame = 0;
		m_last_frame = ma_decoder_get_length_in_pcm_frames((ma_decoder*)m_decoder);

		// init device
		ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
		deviceConfig.playback.format = ((ma_decoder*)m_decoder)->outputFormat;
		deviceConfig.playback.channels = ((ma_decoder*)m_decoder)->outputChannels;
		deviceConfig.sampleRate = ((ma_decoder*)m_decoder)->outputSampleRate;
		deviceConfig.dataCallback = data_callback;
		deviceConfig.pUserData = (ma_decoder*)m_decoder;
		deviceConfig.wasapi.noAutoConvertSRC = true;

		// init device
		if (ma_device_init(NULL, &deviceConfig, ((ma_device*)m_device)) != MA_SUCCESS) {
			spdlog::error("Failed to open playback device");
			ma_decoder_uninit((ma_decoder*)m_decoder);
			return;
		}
		ma_decoder_seek_to_pcm_frame((ma_decoder*)m_decoder, m_last_frame);

		// start
		if (ma_device_start((ma_device*)m_device) != MA_SUCCESS) {
			spdlog::error("Failed to start playback device");
			return;
		}
	}

	Audio::~Audio() {
		delete m_decoder;
		delete m_device;
		ma_decoder_uninit((ma_decoder*)m_decoder);
		ma_device_uninit((ma_device*)m_device);
	}

	void Audio::play(glm::vec2 position) const {
		ma_decoder_seek_to_pcm_frame((ma_decoder*)m_decoder, m_first_frame);
	}

}