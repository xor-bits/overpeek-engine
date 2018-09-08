#pragma once

namespace logic {

	class GameLoop {
	private:
		void(*mCallbackRender)();
		void(*mCallbackUpdate)();
		long long mMicroSPer;
		bool mShouldRun = true;

		int mFps, mUps;

	public:
		GameLoop(void(*callbackRender)(), void(*callbackUpdate)(), long long microSecondsPerUpdate);

		void start();
		void stop();

	};

}