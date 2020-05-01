#include "gameloop.hpp"
#include "clock.hpp"
#include "engine/engine.hpp"
#include "engine/graphics/interface/window.hpp"



namespace oe::utils {

	void GameLoop::start(uint32_t target_ups) {
		oe_debug_call("gameloop");

		m_ups_cap = 1000000 / target_ups;

		auto& clock = oe::utils::Clock::getSingleton();
		m_update_start = clock.getMicroseconds();
		m_update_previous = m_update_start;

		m_should_run = true;
		while (m_should_run) {
			loop();
		}
	}

	void GameLoop::loop() {
		auto& clock = oe::utils::Clock::getSingleton();

		size_t current = clock.getMicroseconds();
		size_t elapsed = current - m_update_previous;
		m_update_previous = current;
		m_update_lag += elapsed;

		//Updates
		while (m_update_lag >= m_ups_cap) {
			// start profiling the update
			oe_debug_call("callback_update");
			size_t timeupdate = clock.getMicroseconds();
			
			// update callback
			m_callback_update();

			// time used on the update
			size_t update_time = clock.getMicroseconds() - timeupdate;
			m_total_update_count++;
			m_average_updatetime[m_total_update_count % mc_average_size] = update_time;
			
			m_cached_average_updatetime = 0;
			for(int i = 0; i < mc_average_size; i++) m_cached_average_updatetime += m_average_updatetime[i];
			m_cached_average_updatetime /= mc_average_size;

			m_update_lag -= m_ups_cap;
		}

		// start profiling the frame
		oe_debug_call("callback_render");
		size_t timeframe = clock.getMicroseconds();

		// render callback
		m_host_window->clear();
		m_callback_render((float)m_update_lag / (float)m_ups_cap);
		m_host_window->update();
		if (m_host_window->shouldClose()) stop();

		// time used on the frame
		size_t frame_time = clock.getMicroseconds() - timeframe;
		m_total_frame_count++;
		m_average_frametime[m_total_frame_count % mc_average_size] = frame_time;

		m_cached_average_frametime = 0;
		for(int i = 0; i < mc_average_size; i++) m_cached_average_frametime += m_average_frametime[i];
		m_cached_average_frametime /= mc_average_size;
	}

	void GameLoop::stop() {
		m_should_run = false;
	}

}