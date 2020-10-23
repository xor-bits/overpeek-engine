#include "gameloop.hpp"
#include "clock.hpp"
#include "engine/engine.hpp"
#include "engine/graphics/interface/window.hpp"



namespace oe::utils
{
	void GameLoop::start()
	{
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
			event.aspect = m_host_window->getAspect();
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

	void GameLoop::loop()
	{
		// updates
		for(auto& system : m_update_systems)
		{
			system.second->update_attempt(*this);
		}

		// start profiling the frame
		auto timepoint = std::chrono::high_resolution_clock::now();

		// render callback
		m_host_window->clear();
		dispatcher.trigger(RenderEvent{});

		// time used on the frame
		auto frame_time = (std::chrono::high_resolution_clock::now() - timepoint);

		// swap the framebuffers out of the timer
		m_host_window->update();
		m_host_window->updateEvents();
		if (m_host_window->shouldClose()) stop();

		m_render_perf_logger.log(frame_time);

		// counters
		auto frame_counter_now = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now().time_since_epoch());
		if (m_frame_counter_start <= frame_counter_now) {
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
			m_frame_counter_start = frame_counter_now + std::chrono::duration<float>(1);
		}
	}

	void GameLoop::stop() noexcept
	{
		m_should_run = false;
	}

}