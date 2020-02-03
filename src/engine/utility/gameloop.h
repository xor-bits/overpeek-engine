#pragma once



namespace oe::utils {

	class GameLoop {
	private:
		static void(*p_callback_render)(float update_fraction);
		static void(*p_callback_update)();

		static int p_fps, p_ups;
		static int p_microsec_frame, p_microsec_update;

		static void loop();

	public:
		static void init(void(*callbackRender)(float update_fraction), void(*callbackUpdate)(), unsigned int updates_per_second);
		static void stop();

		static inline int getFPS() { return p_fps; }
		static inline int getUPS() { return p_ups; }

		static inline int getFrameMicroseconds() { return p_microsec_frame; }
		static inline float getFrameMilliseconds() { return getFrameMicroseconds() / 1000.0f; }
		static inline int getUpdateMicroseconds() { return p_microsec_update; }
		static inline float getUpdateMilliseconds() { return getUpdateMicroseconds() / 1000.0f; }

		// If you dont want to use update loop to move objects
		static inline float getFrameUpdateScale() { return 0.000001f * oe::utils::GameLoop::getFrameMicroseconds(); }

		static int getCalculatedFPS() {
			int mspf = getFrameMicroseconds();
			return (mspf != 0) ? (0.000001f * mspf) : 60;
		}
		static int getCalculatedUPS() {
			int mspu = getUpdateMicroseconds();
			return (mspu != 0) ? (0.000001f * mspu) : 60;
		}
	};

}