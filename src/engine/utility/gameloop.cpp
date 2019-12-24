#include "gameloop.h"

#include "clock.h"



namespace oe::utils {
	void(*GameLoop::p_callback_render)(float update_fraction) = nullptr;
	void(*GameLoop::p_callback_update)() = nullptr;

	int GameLoop::p_fps = 0, GameLoop::p_ups = 0;
	int GameLoop::p_microsec_frame = 0, GameLoop::p_microsec_update = 0;

	long long update_start = 0;
	long long update_previous = 0;
	long long update_lag = 0;

	long long frame_lastTime = 0;
	long long frame_startTime = 0;

	long long counter = 0;

	int frames = 0, updates = 0;

	long long ups_cap = 0;
	bool should_run = false;

	void GameLoop::init(void(*callbackRender)(float), void(*callbackUpdate)(), unsigned int updates_per_second) {
		ups_cap = 1000000 / updates_per_second;
		p_callback_render = callbackRender;
		p_callback_update = callbackUpdate;

		frame_startTime = oe::utils::Clock::getMicroseconds();
		update_start = oe::utils::Clock::getMicroseconds();
		update_previous = update_start;

		should_run = true;
		while (should_run) {
			loop();
		}
	}

	void GameLoop::loop() {
		long long time = oe::utils::Clock::getMicroseconds() - frame_startTime;

		long long current = oe::utils::Clock::getMicroseconds();
		long long elapsed = current - update_previous;
		update_previous = current;
		update_lag += elapsed;

		//Updates
		while (update_lag >= ups_cap) {
			long long timeupdate = oe::utils::Clock::getMicroseconds();
			p_callback_update();
			p_microsec_update = oe::utils::Clock::getMicroseconds() - timeupdate;
			
			updates++;
			update_lag -= ups_cap;
		}

		//Frames
		frames++;
		long long timeframe = oe::utils::Clock::getMicroseconds();

		//m_window->clear();
		p_callback_render((float)update_lag / (float)ups_cap);
		//m_window->update();
		//m_window->input();

		p_microsec_frame = oe::utils::Clock::getMicroseconds() - timeframe;

		//FPS and UPS counters
		if (time - counter > 1000000) {
			counter += 1000000;
			p_fps = frames;
			p_ups = updates;
			frames = 0;
			updates = 0;
		}
	}

	void GameLoop::stop() {
		should_run = false;
	}

}