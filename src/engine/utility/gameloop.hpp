#pragma once

#include <cstdint>
#include <functional>



namespace oe::utils {

	class GameLoop {
	private:
		static GameLoop* singleton;
		GameLoop() {}

	public:
		GameLoop(const GameLoop&) = delete;
		static GameLoop& getSingleton() {
			if (!singleton) singleton = new GameLoop();
			return *singleton;
		}

	private:
		std::function<void(float)> m_callback_render;
		std::function<void()> m_callback_update;

		uint32_t m_fps = 0, m_ups = 0;
		uint32_t m_microsec_frame = 0, m_microsec_update = 0;

		//
		size_t m_update_start = 0;
		size_t m_update_previous = 0;
		size_t m_update_lag = 0;

		size_t m_frame_lastTime = 0;
		size_t m_frame_startTime = 0;

		size_t m_counter = 0;

		uint32_t m_frames = 0, m_updates = 0;

		size_t m_ups_cap = 0;
		bool m_should_run = false;

	private:
		void loop();

	public:
		void start(std::function<void(float)> callback_render, std::function<void()> callback_update, uint32_t target_ups);
		void stop();

	public:
		inline uint32_t getFPS() { return m_fps; }
		inline uint32_t getUPS() { return m_ups; }

		inline uint32_t getFrameMicroseconds() { return m_microsec_frame; }
		inline uint32_t getUpdateMicroseconds() { return m_microsec_update; }
		inline float getFrameMilliseconds() { return getFrameMicroseconds() / 1000.0f; }
		inline float getUpdateMilliseconds() { return getUpdateMicroseconds() / 1000.0f; }

		// For logic inside the render loop
		inline float getFrameUpdateScale() { return 0.000001f * getFrameMicroseconds(); }

		uint32_t getCalculatedFPS() {
			uint32_t mspf = getFrameMicroseconds();
			return (mspf != 0) ? static_cast<int>(0.000001f / mspf) : 60;
		}
		uint32_t getCalculatedUPS() {
			uint32_t mspu = getUpdateMicroseconds();
			return (mspu != 0) ? static_cast<int>(0.000001f / mspu) : 60;
		}

	};

}