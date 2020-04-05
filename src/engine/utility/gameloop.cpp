#include "gameloop.hpp"

#include "clock.hpp"



namespace oe::utils {

	GameLoop* GameLoop::singleton = nullptr;

	
	
	void GameLoop::start(std::function<void(float)> callback_render, std::function<void()> callback_update, uint32_t target_ups) {
		m_ups_cap = 1000000 / target_ups;
		m_callback_render = callback_render;
		m_callback_update = callback_update;

		auto& clock = oe::utils::Clock::getSingleton();
		m_frame_startTime = clock.getMicroseconds();
		m_update_start = clock.getMicroseconds();
		m_update_previous = m_update_start;

		m_should_run = true;
		while (m_should_run) {
			loop();
		}
	}

	void GameLoop::loop() {
		auto& clock = oe::utils::Clock::getSingleton();

		long long time = clock.getMicroseconds() - m_frame_startTime;
		long long current = clock.getMicroseconds();
		long long elapsed = current - m_update_previous;
		m_update_previous = current;
		m_update_lag += elapsed;

		//Updates
		while (m_update_lag >= m_ups_cap) {
			long long timeupdate = clock.getMicroseconds();
			m_callback_update();
			m_microsec_update = clock.getMicroseconds() - timeupdate;
			
			m_updates++;
			m_update_lag -= m_ups_cap;
		}

		//Frames
		m_frames++;
		long long timeframe = clock.getMicroseconds();

		//m_window->clear();
		m_callback_render((float)m_update_lag / (float)m_ups_cap);
		//m_window->update();
		//m_window->input();

		m_microsec_frame = clock.getMicroseconds() - timeframe;

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
		m_should_run = false;
	}

}