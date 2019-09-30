#pragma once

namespace oe {

	class GameLoop {
	private:
		void(*mCallbackRender)(float);
		void(*mCallbackUpdate)();
		long long m_upsCap;
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
		//Render loop:
		//render loop has argument "corrector" that is fixes moving object stuttering by applying ("corrector" times that object's velocity).
		//For example if ball is moving 5 units to left every second (second being update) you propably want to smooth those frames between
		//so render ball at (ball_location + ball_velocity * corrector / ups).
		//
		//Update loop:
		//Example of calculating position "loaction = velocity / ups"
		GameLoop();

		void start(void(*callbackRender)(float), void(*callbackUpdate)(), unsigned int updates_per_second);
		void stop();

		inline int getFPS() { return m_fps; }
		inline int getUPS() { return m_ups; }

		inline short int getFMS() { return m_microsec_frame; }
		inline short int getUMS() { return m_microsec_update; }

		int fastFPS() { if (getFMS() != 0) { return 1000000 / getFMS(); } else return 1; }
		int fastUPS() { if (getUMS() != 0) { return 1000000 / getUMS(); } else return 1; }
	};

}