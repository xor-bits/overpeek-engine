#include "gameloop.hpp"
#include "clock.hpp"
#include "engine/engine.hpp"
#include "engine/graphics/interface/window.hpp"



namespace oe::utils {

	PerfLogger::PerfLogger()
	{
		m_average_time.fill(0);
	}

	void PerfLogger::log(size_t time)
	{
		m_total_count++;
		m_periodical_count++;

		m_average_time[m_total_count % mc_average_size] = time;
		
		m_cached_average_time = 0;
		for(int i = 0; i < mc_average_size; i++) m_cached_average_time += m_average_time[i];
		m_cached_average_time /= mc_average_size;
	}

	UpdateSystem::UpdateSystem(size_t updates_per_second)
	{
		m_updates_per_second = updates_per_second;
		m_ups_cap = 1000000 / updates_per_second;
		
		auto& clock = oe::utils::Clock::getSingleton();
		m_update_start = clock.getMicroseconds();
		m_update_previous = m_update_start;
	}

	void UpdateSystem::update_attempt()
	{
		auto& clock = oe::utils::Clock::getSingleton();

		size_t current = clock.getMicroseconds();
		size_t elapsed = current - m_update_previous;
		m_update_previous = current;
		m_update_lag += elapsed;

		// Updates
		while (m_update_lag >= m_ups_cap) {
			// start profiling the update
			size_t timeupdate = clock.getMicroseconds();
			
			// update callback
			update_signal.publish();

			// time used on the update
			size_t update_time = clock.getMicroseconds() - timeupdate;
			m_perf_logger.log(update_time);

			m_update_lag -= m_ups_cap;
		}
	}

	GameLoop::GameLoop(oe::graphics::Window* window, size_t main_updatesystem_ups)
	{
		m_host_window = window;
		m_main_updatesystem_ups = main_updatesystem_ups;

		UpdateSystem* ptr = new UpdateSystem(main_updatesystem_ups);
		m_update_systems.insert(std::make_pair(main_updatesystem_ups, ptr));
	}

	GameLoop::~GameLoop()
	{
		for(auto& system : m_update_systems)
		{
			delete system.second;
		}
	}

	void GameLoop::start() {
		oe_debug_call("gameloop");

		// initial resize
		oe::ResizeEvent event;
		event.framebuffer_size = m_host_window->m_window_info.size;
		event.framebuffer_size_old = event.framebuffer_size;
		event.aspect = m_host_window->aspect();
		m_host_window->dispatcher.trigger(event);

		m_should_run = true;
		while (m_should_run) {
			loop();
		}
	}

	void GameLoop::loop() {
		auto& clock = oe::utils::Clock::getSingleton();

		// updates
		for(auto& system : m_update_systems)
		{
			system.second->update_attempt();
		}

		// start profiling the frame
		size_t timeframe = clock.getMicroseconds();

		// render callback
		auto main_updatesystem_iter = m_update_systems[m_main_updatesystem_ups];
		float update_fraction = static_cast<float>(main_updatesystem_iter->m_update_lag) / static_cast<float>(main_updatesystem_iter->m_ups_cap);
		m_host_window->clear();
		render_signal.publish(update_fraction);

		// time used on the frame
		size_t frame_time = clock.getMicroseconds() - timeframe;

		// swap the framebuffers out of the timer
		m_host_window->update();
		if (m_host_window->shouldClose()) stop();

		m_render_perf_logger.log(frame_time);

		// counters
		float frame_counter_now = clock.getSessionMillisecond();
		if (m_frame_counter_start < frame_counter_now) {
			// frame counter
			m_render_perf_logger.m_per_second = m_render_perf_logger.m_periodical_count;
			m_render_perf_logger.m_periodical_count = 0;
			
			// update counters
			for(auto& system : m_update_systems)
			{
				system.second->m_perf_logger.m_per_second = system.second->m_perf_logger.m_periodical_count;
				system.second->m_perf_logger.m_periodical_count = 0;
			}

			// reset timer
			m_frame_counter_start = frame_counter_now + 1000.0f;
		}
	}

	void GameLoop::stop() {
		m_should_run = false;
	}

}