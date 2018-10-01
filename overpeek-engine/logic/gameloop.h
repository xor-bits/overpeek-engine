#pragma once

namespace logic {

	class GameLoop {
	private:
		void(*mCallbackRender)();
		void(*mCallbackUpdate)();
		void(*mCallbackConstantly)();
		long long mMicroSPer;
		long long m_fpsCap;
		bool mShouldRun = true;

		int mFps, mUps;

		void loop();

		int m_frames, m_updates;
		long long m_start;
		long long m_previous;
		long long m_lag;
		long long m_fpslastTime;
		long long m_fpsstartTime;


	public:
		GameLoop(void(*callbackRender)(), void(*callbackUpdate)(), void(*callbackConstantly)(), long long microSecondsPerUpdate, unsigned int fpsCap);

		void start();
		void stop();

		inline int getFPS() { return mFps; }
		inline int getUPS() { return mUps; }
	};

}