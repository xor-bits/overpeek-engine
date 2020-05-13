#pragma once

#include <cstdint>
#include <functional>
#include <array>


namespace oe::graphics { class Window; }
namespace oe::utils {

	class GameLoop {
	private:
		GameLoop(std::function<void(float)> callback_render, std::function<void()> callback_update, oe::graphics::Window* window)
		{
			m_host_window = window;
			m_average_frametime.fill(0);
			m_average_updatetime.fill(0);
			m_total_frame_count = 0;
			m_total_update_count = 0;
			m_callback_render = callback_render;
			m_callback_update = callback_update;
		}
	friend class oe::graphics::Window;

	private:
		std::function<void(float)> m_callback_render;
		std::function<void()> m_callback_update;

		// performance
		static const size_t mc_average_size = 200;
		std::array<size_t, mc_average_size> m_average_frametime;
		std::array<size_t, mc_average_size> m_average_updatetime;
		float m_cached_average_frametime;
		float m_cached_average_updatetime;
		size_t m_total_frame_count;
		size_t m_total_update_count;
		size_t m_periodical_frame_count = 0;
		size_t m_periodical_update_count = 0;
		size_t m_fps = 0;
		size_t m_ups = 0;

		// frame and update balancing
		size_t m_update_start = 0;
		size_t m_update_previous = 0;
		size_t m_update_lag = 0;
		size_t m_ups_cap = 0;

		//
		oe::graphics::Window* m_host_window;
		bool m_should_run = false;

	private:
		void loop();

	public:
		void start(uint32_t target_ups);
		void stop();

	public:
		inline uint32_t getAverageFPS()
		{
			return m_fps;
			// return (m_cached_average_frametime != 0) ? static_cast<uint32_t>(1000000.0f / m_cached_average_frametime) : 0;
		}
		inline uint32_t getAverageUPS()
		{ 
			return m_ups;
			// return (m_cached_average_updatetime != 0) ? static_cast<uint32_t>(1000000.0f / m_cached_average_updatetime) : 0;
		}

		// frametime in microseconds
		inline float getFrametime() { return m_cached_average_frametime; }

		// updatetime in microseconds
		inline float getUpdatetime() { return m_cached_average_updatetime; }
		
		// frametime in milliseconds
		inline float getFrametimeMS() { return getFrametime() / 1000.0f; }
		
		// updatetime in milliseconds
		inline float getUpdatetimeMS() { return getUpdatetime() / 1000.0f; }

		// For logic inside the render loop
		inline float getFrameUpdateScale() { return 0.000001f * getFrametime(); }

	};

}