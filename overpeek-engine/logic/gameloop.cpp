#include "gameloop.h"

#include "../engine.h"
#include "../tools/clock.h"
#include <iostream>
#include <omp.h>

namespace logic {

	GameLoop::GameLoop(void(*callbackRender)(), void(*callbackUpdate)(), void(*callbackRapid)(), unsigned int upsCap, unsigned int fpsCap) {
		m_upsCap = 1000000 / upsCap;
		m_fpsCap = 1000000 / fpsCap;
		m_frame_lastTime = 0.0;
		m_frame_startTime = tools::Clock::getMicroseconds();
		m_counter = 0.0;
		mCallbackRender = callbackRender;
		mCallbackUpdate = callbackUpdate;
		mCallbackRapid = callbackRapid;

		m_update_start = tools::Clock::getMicroseconds();
		m_update_previous = m_update_start;
		m_update_lag = 0;
	}

	void GameLoop::start() {
#pragma omp parallel 
		{
			if (omp_get_thread_num() == 0) {
				while (mShouldRun) {
					loop();
				}
			}
			//else if (omp_get_thread_num() == 1) {
			//	while (mShouldRun) {
			//		mCallbackRapid();
			//	}
			//}
		}
	}

	void GameLoop::loop() {
		long long time = tools::Clock::getMicroseconds() - m_frame_startTime;
		long long fpsdeltaTime = time - m_frame_lastTime;

		long long current = tools::Clock::getMicroseconds();
		long long elapsed = current - m_update_previous;
		m_update_previous = current;
		m_update_lag += elapsed;

		while (m_update_lag >= m_upsCap) {
			m_update_lag -= m_upsCap;

			m_updates++;
			mCallbackUpdate();
		}

		if (fpsdeltaTime >= m_fpsCap) {
			m_frame_lastTime = time;
		}

		m_frames++;
		mCallbackRender();

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