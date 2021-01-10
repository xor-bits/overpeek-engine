#pragma once

#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <optional>



namespace oe::utils
{
	// ts_queue: extended std::queue with wait_x, wait_for_x and wait_until_x
	template<typename T>
	class ts_queue
	{
	public:
		using container_type = std::queue<T>;
		using value_type = typename container_type::value_type;
		using size_type = typename container_type::size_type;

		using base_reference = typename container_type::reference;
		using const_base_reference = typename container_type::const_reference;

		using reference = std::pair<base_reference, std::unique_lock<std::mutex>>;
		using const_reference = std::pair<const_base_reference, std::unique_lock<std::mutex>>;
		
		// ------------
		// constructors
		// ------------

		inline ts_queue() = default;
		inline ts_queue(const ts_queue& copy)
		{
			std::scoped_lock lock(m_mtx, copy.m_mtx);
			m_container = copy.m_container;
			m_empty = copy.m_empty;
			m_size = copy.m_size;
		}
		inline ts_queue(ts_queue&& move)
		{
			std::scoped_lock lock(m_mtx, move.m_mtx);
			m_container = std::move(move.m_container);
			m_empty = move.m_empty;
			m_size = move.m_size;
		}
		
		template<class Alloc>
		inline explicit ts_queue(const Alloc& alloc)
			: m_container(alloc)
		{}
		template<class Alloc>
		inline ts_queue(const ts_queue& copy, const Alloc& alloc)
		{
			std::scoped_lock lock(m_mtx, copy.m_mtx);
			m_container = { copy, alloc };
			m_empty = copy.m_empty;
			m_size = copy.m_size;
		}
		template<class Alloc>
		inline ts_queue(ts_queue&& move, const Alloc& alloc)
		{
			std::scoped_lock lock(m_mtx, move.m_mtx);
			m_container = { move, alloc };
			m_empty = move.m_empty;
			m_size = move.m_size;
		}

		inline ts_queue& operator=(const ts_queue& copy)
		{
			std::scoped_lock lock(m_mtx, copy.m_mtx);
			m_container = { copy };
			m_empty = copy.m_empty;
			m_size = copy.m_size;
		}
		inline ts_queue& operator=(ts_queue&& move)
		{
			std::scoped_lock lock(m_mtx, move.m_mtx);
			m_container = { move };
			m_empty = move.m_empty;
			m_size = move.m_size;
		}

		// --------------
		// element access
		// --------------

		// immediate
		
		[[nodiscard]] reference inline front()
		{
			std::unique_lock lock(m_mtx);
			return { std::forward<base_reference>(m_container.front()), std::move(lock) };
		}
		[[nodiscard]] const_reference inline front() const
		{
			std::unique_lock lock(m_mtx);
			return { std::forward<const_base_reference>(m_container.front()), std::move(lock) };
		}

		[[nodiscard]] reference inline back()
		{
			std::unique_lock lock(m_mtx);
			return { std::forward<base_reference>(m_container.back()), std::move(lock) };
		}
		[[nodiscard]] const_reference inline back() const
		{
			std::unique_lock lock(m_mtx);
			return { std::forward<const_base_reference>(m_container.back()), std::move(lock) };
		}

		// wait

		inline void wait()
		{
			if(!empty())
				return;

			std::unique_lock cv_lock(m_cv_mtx);
			m_cv.wait(cv_lock, [this](){ return !empty(); });
		}
		
		[[nodiscard]] reference inline wait_front()
		{
			wait();
			return front();
		}
		[[nodiscard]] const_reference inline wait_front() const
		{
			wait();
			return front();
		}

		[[nodiscard]] reference inline wait_back()
		{
			wait();
			return back();
		}
		[[nodiscard]] const_reference inline wait_back() const
		{
			wait();
			return back();
		}

		// wait_for

		template<typename Rep, typename Period>
		inline bool wait_for(const std::chrono::duration<Rep, Period>& duration)
		{
			if(!empty())
				return true;

			std::unique_lock cv_lock(m_cv_mtx);
			return m_cv.wait_for(cv_lock, duration, [this](){ return !empty(); });
		}
		
		template<typename Rep, typename Period>
		[[nodiscard]] std::optional<reference> inline wait_for_front(const std::chrono::duration<Rep, Period>& duration)
		{
			if(!wait_for(duration))
				return std::nullopt;

			return front();
		}
		template<typename Rep, typename Period>
		[[nodiscard]] std::optional<const_reference> inline wait_for_front(const std::chrono::duration<Rep, Period>& duration) const
		{
			if(!wait_for(duration))
				return std::nullopt;

			return front();
		}

		template<typename Rep, typename Period>
		[[nodiscard]] std::optional<reference> inline wait_for_back(const std::chrono::duration<Rep, Period>& duration)
		{
			if(!wait_for(duration))
				return std::nullopt;

			return back();
		}
		template<typename Rep, typename Period>
		[[nodiscard]] std::optional<const_reference> inline wait_for_back(const std::chrono::duration<Rep, Period>& duration) const
		{
			if(!wait_for(duration))
				return std::nullopt;

			return back();
		}

		// wait_until

		template<typename Clock, typename Duration>
		inline bool wait_until(const std::chrono::time_point<Clock, Duration>& atime)
		{
			if(!empty())
				return true;

			std::unique_lock cv_lock(m_cv_mtx);
			return m_cv.wait_until(cv_lock, atime, [this](){ return !empty(); });
		}
		
		template<typename Clock, typename Duration>
		[[nodiscard]] std::optional<reference> inline wait_until_front(const std::chrono::time_point<Clock, Duration>& atime)
		{
			if(!wait_until(atime))
				return std::nullopt;

			return front();
		}
		template<typename Clock, typename Duration>
		[[nodiscard]] std::optional<const_reference> inline wait_until_front(const std::chrono::time_point<Clock, Duration>& atime) const
		{
			if(!wait_until(atime))
				return std::nullopt;

			return front();
		}

		template<typename Clock, typename Duration>
		[[nodiscard]] std::optional<reference> inline wait_until_back(const std::chrono::time_point<Clock, Duration>& atime)
		{
			if(!wait_until(atime))
				return std::nullopt;

			return back();
		}
		template<typename Clock, typename Duration>
		[[nodiscard]] std::optional<const_reference> inline wait_until_back(const std::chrono::time_point<Clock, Duration>& atime) const
		{
			if(!wait_until(atime))
				return std::nullopt;

			return back();
		}

		// --------
		// capacity
		// --------
		
		[[nodiscard]] inline bool empty() const { return m_empty; }
		[[nodiscard]] inline size_type size() const { return m_size; }

		// ---------
		// modifiers
		// ---------

		inline void push(const value_type& value)
		{
			std::scoped_lock lock(m_mtx);
			m_container.push(value);
			m_size = m_container.size();
			m_empty = m_container.empty();
			m_cv.notify_one();
		}
		inline void push(value_type&& value)
		{
			std::scoped_lock lock(m_mtx);
			m_container.push(value);
			m_size = m_container.size();
			m_empty = m_container.empty();
			m_cv.notify_one();
		}

		template<class ... Args>
		inline decltype(auto) emplace(Args&& ... args)
		{
			std::scoped_lock lock(m_mtx);
			m_container.emplace(std::forward<Args>(args)...);
			m_size = m_container.size();
			m_empty = m_container.empty();
			m_cv.notify_one();
		}

		inline void pop()
		{
			std::scoped_lock lock(m_mtx);
			m_container.pop();
			m_size = m_container.size();
			m_empty = m_container.empty();
			m_cv.notify_one();
		}

		inline void swap(ts_queue& other) noexcept(std::is_nothrow_swappable<container_type>::value)
		{
			std::scoped_lock lock(m_mtx, other.m_mtx);
			m_container.swap(other.m_container);
			m_size = other.m_size.exchange(m_size);
			m_empty = other.m_empty.exchange(m_empty);
			m_cv.notify_one();
		}

	private:
		container_type m_container{};
		std::mutex m_mtx{};
		std::atomic<bool> m_empty{ true };
		std::atomic<size_type> m_size{ 0 };

		std::condition_variable m_cv{};
		std::mutex m_cv_mtx{};
	};
}

namespace std
{
	template<typename T>
	void swap(oe::utils::ts_queue<T>& lhs, oe::utils::ts_queue<T>& rhs) noexcept(std::is_nothrow_swappable<typename oe::utils::ts_queue<T>::container_type>::value)
	{
		lhs.swap(rhs);
	}
}