#pragma once

namespace logic {

	class GameLoop {
	private:
		void(*mCallbackRender)();
		void(*mCallbackUpdate)();
		void(*mCallbackConstantly)();
		long long mMicroSPer;
		bool mShouldRun = true;

		int mFps, mUps;

		void loop();

		int m_frames, m_updates;
		long long m_start;
		long long m_previous;
		long long m_lag;

	public:
		GameLoop(void(*callbackRender)(), void(*callbackUpdate)(), void(*callbackConstantly)(), long long microSecondsPerUpdate);

		void start();
		void stop();

		inline int getFPS() { return mFps; }
		inline int getUPS() { return mUps; }
	};

}