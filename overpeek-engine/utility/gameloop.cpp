#include "gameloop.h"

#include "../utility/clock.h"

namespace oe {

	GameLoop::GameLoop() {
		m_fps = 0; m_ups = 0;
		m_frames = 0; m_updates = 0;

		m_update_start = 0;
		m_update_previous = 0;
		m_update_lag = 0;
		
		m_frame_lastTime = 0.0;
		m_frame_startTime = 0;

		m_counter = 0.0;
		
		m_microsec_frame = 0;
		m_microsec_update = 0;
		
		m_upsCap = 0;
		
		mCallbackRender = nullptr;
		mCallbackUpdate = nullptr;
		mShouldRun = true;
	}

	void GameLoop::start(void(*callbackRender)(float), void(*callbackUpdate)(), unsigned int updates_per_second) {
		m_upsCap = 1000000 / updates_per_second;
		mCallbackRender = callbackRender;
		mCallbackUpdate = callbackUpdate;

		m_frame_startTime = oe::Clock::getMicroseconds();
		m_update_start = oe::Clock::getMicroseconds();
		m_update_previous = m_update_start;

		while (mShouldRun) {
			loop();
		}
	}

	void GameLoop::loop() {
		long long time = oe::Clock::getMicroseconds() - m_frame_startTime;

		long long current = oe::Clock::getMicroseconds();
		long long elapsed = current - m_update_previous;
		m_update_previous = current;
		m_update_lag += elapsed;

		//Updates
		while (m_update_lag >= m_upsCap) {
			long long timeupdate = oe::Clock::getMicroseconds();
			mCallbackUpdate();
			m_microsec_update = oe::Clock::getMicroseconds() - timeupdate;
			
			m_updates++;
			m_update_lag -= m_upsCap;
		}

		//Frames
		m_frames++;
		long long timeframe = oe::Clock::getMicroseconds();

		//m_window->clear();
		mCallbackRender((float)m_update_lag / (float)m_upsCap);
		//m_window->update();
		//m_window->input();

		m_microsec_frame = oe::Clock::getMicroseconds() - timeframe;

		//FPS and UPS counters
		if (time - m_counter > 1000000) {
			m_counter += 1000000;
			m_fps = m_frames;
			m_ups = m_updates;
			m_frames = 0;
			m_updates = 0;
		}
	}

	void GameLoop::stop() {
		mShouldRun = false;
	}

}