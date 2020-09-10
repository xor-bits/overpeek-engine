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
		m_max_time = 0;
		m_min_time = std::numeric_limits<size_t>::max();
		for(int i = 0; i < mc_average_size; i++) 
		{
			m_cached_average_time += m_average_time[i];
			if(m_max_time < m_average_time[i]) m_max_time = m_average_time[i];
			if(m_min_time > m_average_time[i]) m_min_time = m_average_time[i];
		}
		m_cached_average_time /= mc_average_size;
	}

	GameLoop::GameLoop(oe::graphics::IWindow* window)
	{
		m_host_window = window;
	}

	GameLoop::~GameLoop()
	{}

	void GameLoop::start() {
		oe_debug_call("gameloop");

		m_should_run = true;
		m_host_window->inactive_context(); // release context from main thread
		std::thread second_thread([&](){
			m_host_window->active_context(); // make context current in this second("gl") thread
			
			dispatcher.trigger(InitEvent{}); // init

			// initial resize after init signal
			oe::ResizeEvent event;
			event.framebuffer_size = m_host_window->m_window_info.size;
			event.framebuffer_size_old = event.framebuffer_size;
			event.aspect = m_host_window->aspect();
			dispatcher.trigger(event);

			while (m_should_run)
				loop(); // update and render
			
			dispatcher.trigger(CleanupEvent{}); // cleanup

			m_host_window->bump();
			m_host_window->inactive_context();
		});

		while(m_should_run)
		{
			m_host_window->waitEvents();
		}

		if(second_thread.joinable()) second_thread.join();
		m_host_window->active_context();
	}

	void GameLoop::loop() {
		auto& clock = oe::utils::Clock::getSingleton();

		// updates
		for(auto& system : m_update_systems)
		{
			system.second->update_attempt(*this);
		}

		// start profiling the frame
		size_t timeframe = clock.getMicroseconds();

		// render callback
		m_host_window->clear();
		dispatcher.trigger(RenderEvent{});

		// time used on the frame
		size_t frame_time = clock.getMicroseconds() - timeframe;

		// swap the framebuffers out of the timer
		m_host_window->update();
		m_host_window->updateEvents();
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