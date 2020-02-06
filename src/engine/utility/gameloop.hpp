#pragma once

#include <cstdint>



namespace oe::utils {

	class GameLoop {
	private:
		static void(*p_callback_render)(float update_fraction);
		static void(*p_callback_update)();

		static uint32_t p_fps, p_ups;
		static uint32_t p_microsec_frame, p_microsec_update;

		static void loop();

	public:
		static void init(void(*callbackRender)(float update_fraction), void(*callbackUpdate)(), unsigned int updates_per_second);
		static void stop();

		static inline uint32_t getFPS() { return p_fps; }
		static inline uint32_t getUPS() { return p_ups; }

		static inline uint32_t getFrameMicroseconds() { return p_microsec_frame; }
		static inline float getFrameMilliseconds() { return getFrameMicroseconds() / 1000.0f; }
		static inline uint32_t getUpdateMicroseconds() { return p_microsec_update; }
		static inline float getUpdateMilliseconds() { return getUpdateMicroseconds() / 1000.0f; }

		// If you dont want to use update loop to move objects
		static inline float getFrameUpdateScale() { return 0.000001f * oe::utils::GameLoop::getFrameMicroseconds(); }

		static uint32_t getCalculatedFPS() {
			int mspf = getFrameMicroseconds();
			return (mspf != 0) ? static_cast<int>(0.000001f / mspf) : 60;
		}
		static uint32_t getCalculatedUPS() {
			int mspu = getUpdateMicroseconds();
			return (mspu != 0) ? static_cast<int>(0.000001f / mspu) : 60;
		}
	};

}