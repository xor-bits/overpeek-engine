#include "gameloop.h"

#include "../tools/clock.h"

namespace logic {

	GameLoop::GameLoop(void(*callbackRender)(), void(*callbackUpdate)(), long long microSecondsPerUpdate) {
		mMicroSPer = microSecondsPerUpdate;
		mCallbackRender = callbackRender;
		mCallbackUpdate = callbackUpdate;
	}

	void GameLoop::start() {
		int frames = 0, updates = 0;
		long long start = tools::getMicroseconds();
		long long previous = start;
		long long lag = 0;
		while (mShouldRun)
		{
			long long current = tools::getMicroseconds();
			long long elapsed = current - previous;
			previous = current;
			lag += elapsed;
		
			while (lag >= mMicroSPer)
			{
				updates++;
				mCallbackUpdate();
				lag -= mMicroSPer;
			}
		
			frames++;
			if (mCallbackRender) mCallbackRender();
		
		
			//Frame/update counter
			if (current - start > 1000000) {
				mFps = frames; mUps = updates;
				frames = 0; updates = 0;
				start += 1000000;
			}
		}
	}

	void GameLoop::stop() {
		mShouldRun = false;
	}

}