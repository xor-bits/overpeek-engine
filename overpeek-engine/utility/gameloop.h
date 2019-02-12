#pragma once

namespace oe {

	class GameLoop {
	private:
		void(*mCallbackRender)();
		void(*mCallbackUpdate)();
		void(*mCallbackRapid)();
		long long m_upsCap;
		long long m_fpsCap;
		bool mShouldRun = true;

		int m_fps, m_ups;
		int m_frames, m_updates;

		long long m_update_start;
		long long m_update_previous;
		long long m_update_lag;

		long long m_frame_lastTime;
		long long m_frame_startTime;

		long long m_counter;

		short int m_microsec_frame;
		short int m_microsec_update;

		void loop();

	public:
		GameLoop(void(*callbackRender)(), void(*callbackUpdate)(), void(*callbackRapid)(), unsigned int upsCap, unsigned int fpsCap);

		void start();
		void stop();

		inline int getFPS() { return m_fps; }
		inline int getUPS() { return m_ups; }

		inline short int getFMS() { return m_microsec_frame; }
		inline short int getUMS() { return m_microsec_update; }
	};

}