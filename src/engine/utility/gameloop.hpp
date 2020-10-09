#pragma once

#include <cstdint>
#include <functional>
#include <array>
#include <entt/entt.hpp>
#include <optional>
#include <chrono>
#include <atomic>
#include "engine/enum.hpp"


namespace oe::graphics { class IWindow; }
namespace oe::utils
{
	static const size_t mc_average_size = 200;
	static_assert(mc_average_size, "mc_average_size cannot be zero");

	class GameLoop;

	struct PerfLogger
	{
		// performance logger
		std::array<std::chrono::nanoseconds, mc_average_size> m_average_time;
		std::chrono::nanoseconds m_cached_average_time = std::chrono::nanoseconds::zero();
		size_t m_total_count = 0;
		size_t m_periodical_count = 0;
		size_t m_per_second = 0;
		
		std::chrono::nanoseconds m_min_time = std::chrono::nanoseconds::zero();
		std::chrono::nanoseconds m_max_time = std::chrono::nanoseconds::zero();

		constexpr PerfLogger() : m_average_time({}) {}
		constexpr void log(std::chrono::nanoseconds duration) noexcept
		{
			m_total_count++;
			m_periodical_count++;

			m_average_time[m_total_count % mc_average_size] = duration;

			
			m_cached_average_time = std::chrono::nanoseconds::zero();
			m_max_time = std::chrono::nanoseconds::min();
			m_min_time = std::chrono::nanoseconds::max();
			for(int i = 0; i < mc_average_size; i++) 
			{
				m_cached_average_time += m_average_time[i];
				m_max_time = std::max(m_max_time, m_average_time[i]);
				m_min_time = std::min(m_min_time, m_average_time[i]);
			}
			m_cached_average_time /= mc_average_size;
		}
	};

	struct UpdateSystemBase
	{
		PerfLogger m_perf_logger;

		// update balancing
		std::chrono::high_resolution_clock::time_point m_update_start;
		std::chrono::high_resolution_clock::time_point m_update_previous;
		std::chrono::nanoseconds m_update_time_lag = std::chrono::nanoseconds::zero();
		std::chrono::nanoseconds m_update_time_target = std::chrono::nanoseconds::zero();

		virtual void update_attempt(GameLoop& loop) = 0;
	};

	template<size_t ups>
	struct UpdateSystem;

	class GameLoop
	{
	private:
		PerfLogger m_render_perf_logger;
		std::chrono::duration<float> m_frame_counter_start = std::chrono::duration<float>::zero();
		entt::dispatcher dispatcher;

		std::unordered_map<size_t, std::unique_ptr<UpdateSystemBase>> m_update_systems;

		oe::graphics::IWindow* m_host_window;
		std::atomic<bool> m_should_run = false;

	private:
		void loop();

	public:
		GameLoop(oe::graphics::IWindow* window) noexcept : m_host_window(window) {}
		GameLoop(const GameLoop&) = delete;

		void start();
		void stop() noexcept;

		template<size_t ups>
		void try_create() noexcept
		{
			auto iter = m_update_systems.find(ups);
			if (iter == m_update_systems.end())
			 	m_update_systems.insert({ ups, std::move(std::make_unique<UpdateSystem<ups>>()) });
		}

		template<size_t ups>
		void try_remove() noexcept
		{
			auto iter = m_update_systems.find(ups);
			if (iter != m_update_systems.end() && dispatcher.sink<oe::UPS<ups>>().empty())
				m_update_systems.erase(iter);
		}

		template <typename T, typename = void>
		struct has_system_ups : std::false_type { };
		template <typename T>
		struct has_system_ups <T, std::void_t<decltype(T::system_ups)>> : std::true_type { };

		// events
		template<typename Event, auto Listener, typename Instance>
		void connect_listener(Instance* instance) noexcept
		{
			if constexpr (has_system_ups<Event>::value)
				try_create<Event::system_ups>();
			dispatcher.sink<Event>().template connect<Listener>(*instance);
		}
		template<typename Event, auto Listener>
		void connect_listener() noexcept
		{
			if constexpr (has_system_ups<Event>::value)
				try_create<Event::system_ups>();
			dispatcher.sink<Event>().template connect<Listener>();
		}
		template<typename Event, auto Listener, typename Instance>
		void disconnect_listener(Instance* instance) noexcept
		{
			dispatcher.sink<Event>().template disconnect<Listener>(*instance);
			if constexpr (has_system_ups<Event>::value)
				try_remove<Event::system_ups>();
		}
		template<typename Event, auto Listener>
		void disconnect_listener() noexcept
		{
			dispatcher.sink<Event>().template disconnect<Listener>();
			if constexpr (has_system_ups<Event>::value)
				try_remove<Event::system_ups>();
		}

	public:
		[[nodiscard]] inline constexpr size_t getAverageFPS() const noexcept
		{
			return m_render_perf_logger.m_per_second;
		}
		template<size_t ups>
		[[nodiscard]] inline size_t getAverageUPS() const noexcept
		{
			auto iter = m_update_systems.find(ups);
			if(iter == m_update_systems.end())
				return 0;
			return m_update_systems.at(ups)->m_perf_logger.m_per_second;
		}

		[[nodiscard]] inline constexpr std::chrono::nanoseconds getFrametime() const noexcept
		{
			return m_render_perf_logger.m_cached_average_time;
		}

		template<size_t ups>
		[[nodiscard]] inline std::chrono::nanoseconds getUpdatetime() const noexcept
		{
			auto iter = m_update_systems.find(ups);
			if(iter == m_update_systems.end())
				return std::chrono::nanoseconds::zero();
			return m_update_systems.at(ups)->m_perf_logger.m_cached_average_time;
		}

		[[nodiscard]] inline constexpr const PerfLogger& getFramePerfLogger() const noexcept
		{
			return m_render_perf_logger;
		}
		template<size_t ups>
		[[nodiscard]] inline const PerfLogger& getUpdatePerfLogger() const
		{
			auto iter = m_update_systems.find(ups);
			if(iter == m_update_systems.end())
				throw std::runtime_error(fmt::format("No PerfLogger for {}ups was found", ups));
			return m_update_systems.at(ups)->m_perf_logger;
		}

		template<size_t ups> // for smooth animations when ups is low < 0.0f - 1.0f >
		[[nodiscard]] inline float getUpdateLag() const
		{
			auto iter = m_update_systems.find(ups);
			if(iter == m_update_systems.end())
				0.0f;
			
			auto cast_to_float_dur = [](auto dur){ return std::chrono::duration_cast<std::chrono::duration<float, std::nano>>(dur); };
			return cast_to_float_dur(iter->second->m_update_time_target).count() / cast_to_float_dur(iter->second->m_update_time_target).count();
		};

		inline entt::dispatcher& getDispatcher() { return dispatcher; }

	template<size_t ups>
	friend struct UpdateSystem;
	};

	template<size_t ups>
	struct UpdateSystem : UpdateSystemBase
	{
		static constexpr size_t system_ups = ups;

		UpdateSystem() noexcept
		{
			m_update_start = std::chrono::high_resolution_clock::now();
			m_update_previous = m_update_start;
			m_update_time_target = std::chrono::seconds(1);
			m_update_time_target /= ups;
		}

		virtual void update_attempt(GameLoop& loop) override
		{
			auto current = std::chrono::high_resolution_clock::now();
			auto elapsed = current - m_update_previous;
			m_update_previous = current;
			m_update_time_lag += elapsed;

			// Updates
			while (m_update_time_lag >= m_update_time_target) {
				// start profiling the update
				auto timepoint = std::chrono::high_resolution_clock::now();
				
				// update callback
				loop.dispatcher.trigger(UPS<ups>{});

				// time used on the update
				auto update_time = (std::chrono::high_resolution_clock::now() - timepoint);
				m_perf_logger.log(update_time);

				m_update_time_lag -= m_update_time_target;
			}
		}
	};
}