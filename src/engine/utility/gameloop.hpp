#pragma once

#include <cstdint>
#include <functional>
#include <array>
#include <entt/entt.hpp>


namespace oe::graphics { class Window; }
namespace oe::utils {

	static const size_t mc_average_size = 200;

	struct PerfLogger
	{
		// performance logger
		std::array<size_t, mc_average_size> m_average_time;
		float m_cached_average_time = 0.0f;
		size_t m_total_count = 0;
		size_t m_periodical_count = 0;
		size_t m_per_second = 0;

		PerfLogger();
		void log(size_t time);
	};

	struct UpdateSystem
	{
		PerfLogger m_perf_logger;
		entt::sigh<void()> update_signal;
		entt::sink<void()> update_sink{update_signal};

		// update balancing
		size_t m_update_start = 0;
		size_t m_update_previous = 0;
		size_t m_update_lag = 0;
		size_t m_ups_cap = 0;
		size_t m_updates_per_second = 0;

		UpdateSystem(size_t updates_per_second = 60);
		void update_attempt();
	};

	class GameLoop {
	private:
		PerfLogger m_render_perf_logger;
		float m_frame_counter_start = 0.0f;
		entt::sigh<void(float)> render_signal;
		entt::sink<void(float)> render_sink{render_signal};

		std::unordered_map<size_t, UpdateSystem*> m_update_systems;
		size_t m_main_updatesystem_ups;

		oe::graphics::Window* m_host_window;
		bool m_should_run = false;

	private:
		void loop();

	public:
		GameLoop(oe::graphics::Window* window, size_t main_updatesystem_ups);
		GameLoop(const GameLoop&) = delete;
		~GameLoop();

		void start();
		void stop();

		auto find_or_create(size_t ups)
		{
			auto iter = m_update_systems.find(ups);
			if (iter == m_update_systems.end())
			{
				// new update listener
				UpdateSystem* ptr = new UpdateSystem(ups);
				iter = m_update_systems.insert(std::make_pair(ups, ptr)).first;
			}
			return iter;
		}

		// connect update
		template<size_t ups, auto F, class ... Args>
		void connect_update_listener(Args&&... args)
		{
			find_or_create(ups)->second->update_sink.connect<F>(std::forward(args)...);
		}
		template<size_t ups, auto F>
		void connect_update_listener()
		{
			find_or_create(ups)->second->update_sink.connect<F>();
		}
		// disconnect update
		template<size_t ups, auto F, class ... Args>
		void disconnect_update_listener(Args&&... args)
		{
			m_update_systems[ups].disconnect<F>(std::forward(args)...);
		}
		template<size_t ups, auto F>
		void disconnect_update_listener()
		{
			m_update_systems[ups].disconnect<F>();
		}
		// connect render
		template<auto F, class ... Args>
		void connect_render_listener(Args&&... args)
		{
			render_sink.connect<F>(std::forward(args)...);
		}
		template<auto F>
		void connect_render_listener()
		{
			render_sink.connect<F>();
		}
		// disconnect render
		template<auto F, class ... Args>
		void disconnect_render_listener(Args&&... args)
		{
			render_sink.disconnect<F>(std::forward(args)...);
		}
		template<auto F>
		void disconnect_render_listener()
		{
			render_sink.disconnect<F>();
		}

	public:
		inline uint32_t getAverageFPS()
		{
			return m_render_perf_logger.m_per_second;
			// return (m_cached_average_frametime != 0) ? static_cast<uint32_t>(1000000.0f / m_cached_average_frametime) : 0;
		}
		template<size_t ups>
		inline uint32_t getAverageUPS()
		{ 
			return m_update_systems[ups].m_perf_logger.m_per_second;
			// return (m_cached_average_updatetime != 0) ? static_cast<uint32_t>(1000000.0f / m_cached_average_updatetime) : 0;
		}

		// frametime in microseconds
		inline float getFrametime() { return m_render_perf_logger.m_cached_average_time; }

		// updatetime in microseconds
		template<size_t ups>
		inline float getUpdatetime() { return m_update_systems[ups].m_perf_logger.m_cached_average_time; }
		
		// frametime in milliseconds
		inline float getFrametimeMS() { return getFrametime() / 1000.0f; }
		
		// updatetime in milliseconds
		template<size_t ups>
		inline float getUpdatetimeMS() { return getUpdatetime<ups>() / 1000.0f; }

		// For logic inside the render loop
		inline float getFrameUpdateScale() { return 0.000001f * getFrametime(); }

	};

}