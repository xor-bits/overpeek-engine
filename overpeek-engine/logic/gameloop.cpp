#include "gameloop.h"

#include "../tools/clock.h"
#include <iostream>
#include <omp.h>

namespace logic {

	GameLoop::GameLoop(void(*callbackRender)(), void(*callbackUpdate)(), void(*callbackConstantly)(), long long microSecondsPerUpdate) {
		mMicroSPer = microSecondsPerUpdate;
		mCallbackRender = callbackRender;
		mCallbackUpdate = callbackUpdate;
		mCallbackConstantly = callbackConstantly;

		m_frames = 0, m_updates = 0;
		m_start = tools::Clock::getMicroseconds();
		m_previous = m_start;
		m_lag = 0;
	}

	void GameLoop::start() {
		#pragma omp parallel 
		if (omp_get_thread_num() == 0) {
			while (mShouldRun) {
				loop();
			}
		}
		else if (omp_get_thread_num() == 1) {
			while (mShouldRun) {
				mCallbackConstantly();
			}
		}
	}

	void GameLoop::loop() {
		long long current = tools::Clock::getMicroseconds();
		long long elapsed = current - m_previous;
		m_previous = current;
		m_lag += elapsed;

		while (m_lag >= mMicroSPer)
		{
			m_updates++;
			mCallbackUpdate();
			m_lag -= mMicroSPer;
		}

		m_frames++;
		if (mCallbackRender) mCallbackRender();

		//Frame/update counter
		if (current - m_start > 1000000) {
			mFps = m_frames; mUps = m_updates;
			m_frames = 0; m_updates = 0;
			m_start += 1000000;
		}
	}

	void GameLoop::stop() {
		mShouldRun = false;
	}

}