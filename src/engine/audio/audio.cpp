#include "audio.hpp"

#include "engine/engine.hpp"
#include "engine/utility/formatted_error.hpp"
#include "engine/utility/fileio.hpp"



// ignore external warnings
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#elif _MSC_VER
#pragma warning( push )
#endif

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

// ignore external warnings
#ifdef __clang__
#pragma clang diagnostic pop
#elif __GNUC__
#pragma GCC diagnostic pop
#elif _MSC_VER
#pragma warning( pop )
#endif

namespace oe::audio
{
	AudioDevice::AudioDevice()
	{

	}

	AudioPlayer::AudioPlayer()
	{

	}
}