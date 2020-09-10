#pragma once

#include <cstdint>
#include <functional>
#include <array>
#include <entt/entt.hpp>
#include <optional>
#include "engine/utility/clock.hpp"


namespace oe::graphics { class IWindow; }
namespace oe::utils
{
	static const size_t mc_average_size = 200;
	
	template<size_t ups> struct UPS { static constexpr size_t system_ups = ups; };
	template<size_t ups> using UpdateEvent = UPS<ups>;
	struct InitEvent {};
	struct CleanupEvent {};
	struct RenderEvent {};

	class GameLoop;

	struct PerfLogger
	{
		// performance logger
		std::array<size_t, mc_average_size> m_average_time;
		float m_cached_average_time = 0.0f;
		size_t m_total_count = 0;
		size_t m_periodical_count = 0;
		size_t m_per_second = 0;
		size_t m_min_time = std::numeric_limits<size_t>::max();
		size_t m_max_time = 0;

		PerfLogger();
		void log(size_t time);
	};

	struct UpdateSystemBase
	{
		PerfLogger m_perf_logger;

		// update balancing
		size_t m_update_start = 0;
		size_t m_update_previous = 0;
		size_t m_update_lag = 0;
		size_t m_ups_cap = 0;

		virtual void update_attempt(GameLoop& loop) = 0;
	};

	template<size_t ups>
	struct UpdateSystem;

	class GameLoop
	{
	private:
		PerfLogger m_render_perf_logger;
		float m_frame_counter_start = 0.0f;
		entt::dispatcher dispatcher;

		std::unordered_map<size_t, std::unique_ptr<UpdateSystemBase>> m_update_systems;

		oe::graphics::IWindow* m_host_window;
		bool m_should_run = false;

	private:
		void loop();

	public:
		GameLoop(oe::graphics::IWindow* window);
		GameLoop(const GameLoop&) = delete;
		~GameLoop();

		void start();
		void stop();

		template<size_t ups>
		void try_create()
		{
			spdlog::debug("new upssystem: {}", ups);
			std::unordered_map<size_t, std::unique_ptr<UpdateSystemBase>>::iterator iter = m_update_systems.find(ups);
			if (iter == m_update_systems.end())
			 	iter = m_update_systems.insert({ ups, std::move(std::make_unique<UpdateSystem<ups>>()) }).first;
		}

		template<size_t ups>
		void try_remove()
		{
			if ((const auto iter = m_update_systems.find(ups)) != m_update_systems.end() && dispatcher.sink<UPS<ups>>().empty())
				m_update_systems.erase(iter);
		}

		template <typename T>
		class has_system_ups
		{
			typedef char one;
			struct two { char x[2]; };

			template <typename C> static one test(char[sizeof(&C::system_ups)]) ;
			template <typename C> static two test(...);    

		public:
			enum { value = sizeof(test<T>(0)) == sizeof(char) };
		};

		// events
		template<typename Event, auto Listener, typename Instance>
		void connect_listener(const Instance& instance)
		{
			if constexpr (has_system_ups<Event>::value)
				try_create<Event::system_ups>();
			dispatcher.sink<Event>().template connect<Listener>(instance);
		}
		template<typename Event, auto Listener>
		void connect_listener()
		{
			if constexpr (has_system_ups<Event>::value)
				try_create<Event::system_ups>();
			dispatcher.sink<Event>().template connect<Listener>();
		}
		template<typename Event, auto Listener, typename Instance>
		void disconnect_listener(const Instance& instance)
		{
			dispatcher.sink<Event>().template disconnect<Listener>(instance);
			if constexpr (has_system_ups<Event>::value)
				try_remove<Event::system_ups>();
		}
		template<typename Event, auto Listener>
		void disconnect_listener()
		{
			dispatcher.sink<Event>().template disconnect<Listener>();
			if constexpr (has_system_ups<Event>::value)
				try_remove<Event::system_ups>();
		}

	public:
		inline uint32_t getAverageFPS() const
		{
			return m_render_perf_logger.m_per_second;
			// return (m_cached_average_frametime != 0) ? static_cast<uint32_t>(1000000.0f / m_cached_average_frametime) : 0;
		}
		template<size_t ups>
		inline uint32_t getAverageUPS() const
		{ 
			return m_update_systems.at(ups)->m_perf_logger.m_per_second;
			// return (m_cached_average_updatetime != 0) ? static_cast<uint32_t>(1000000.0f / m_cached_average_updatetime) : 0;
		}

		// frametime in microseconds
		inline float getFrametime() const { return m_render_perf_logger.m_cached_average_time; }

		// updatetime in microseconds
		template<size_t ups>
		inline float getUpdatetime() const { return m_update_systems.at(ups)->m_perf_logger.m_cached_average_time; }
		
		// frametime in milliseconds
		inline float getFrametimeMS() const { return getFrametime() / 1000.0f; }
		
		// updatetime in milliseconds
		template<size_t ups>
		inline float getUpdatetimeMS() const { return getUpdatetime<ups>() / 1000.0f; }

		// For logic inside the render loop
		inline float getFrameUpdateScale() const { return 0.000001f * getFrametime(); }

		template<size_t ups>
		inline auto& getPerfLoggerUPS() const { return m_update_systems.at(ups)->m_perf_logger; }
		inline auto& getPerfLoggerFPS() const { return m_render_perf_logger; }

		template<size_t ups> // for smooth animations when ups is low < 0.0f - 1.0f >
		inline float getUpdateLag() const
		{
			const auto& updatesystem = m_update_systems.at(ups);
			return static_cast<float>(updatesystem->m_update_lag) / static_cast<float>(updatesystem->m_ups_cap);
		};

		inline entt::dispatcher& getDispatcher() { return dispatcher; }

	template<size_t ups>
	friend struct UpdateSystem;
	};

	template<size_t ups>
	struct UpdateSystem : UpdateSystemBase
	{
		static constexpr size_t system_ups = ups;

		UpdateSystem()
		{
			m_update_start = oe::utils::Clock::getSingleton().getMicroseconds();
			m_update_previous = m_update_start;
			m_ups_cap = 1000000 / ups;
		}

		virtual void update_attempt(GameLoop& loop) override
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
				loop.dispatcher.trigger(UPS<ups>{});

				// time used on the update
				size_t update_time = clock.getMicroseconds() - timeupdate;
				m_perf_logger.log(update_time);

				m_update_lag -= m_ups_cap;
			}
		}
	};
}