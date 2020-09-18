#pragma once

#include <string>
#include <mutex>

#include "engine/internal_libs.hpp"
#include "engine/enum.hpp"
#include "engine/utility/fileio.hpp"
#include "engine/utility/gameloop.hpp"


#define M_NUM_KEYS		2048
#define M_NUM_BUTTONS	1024



class GLFWwindow;
namespace oe::graphics {

	class IWindow {
	public:
		WindowInfo m_window_info;
		std::mutex dispatcher_mutex;

	protected:
		GLFWwindow* m_window_handle = nullptr;

		bool m_keys[M_NUM_KEYS];
		bool m_buttons[M_NUM_BUTTONS];

		glm::vec2 m_cursor_transformed = { 0.0f, 0.0f };
		glm::ivec2 m_cursor_window = { 0, 0 };
		float m_aspect_ratio = 0;

		oe::utils::GameLoop m_window_gameloop;
		
		bool m_processing_events = false;

		void postglfw();

	public:
		IWindow(const std::unique_ptr<Instance>& instance, const WindowInfo& window_config);
		virtual ~IWindow();

		virtual void update() = 0;
		virtual void clear(const glm::vec4& color = oe::colors::clear_color) = 0;
		virtual void viewport() const = 0;
		virtual void bind() const = 0;

		virtual void pollEvents() = 0;
		virtual void waitEvents(float timeout = 0.0f) = 0; // timeout in seconds, 0 for no timeout
		virtual void updateEvents() = 0; // only after for waitEvents, process events in main thread
		virtual void bump() = 0; // empty event to get out of waitEvents
		inline bool processingEvents() const { return m_processing_events; }

		/*Also known as VSync*/
		virtual void swapInterval(uint8_t frames) = 0;

		bool shouldClose();
		void setIcon(const oe::utils::image_data& image);
		void showCursor(bool show);

		// for multiwindow setups
		// active window while rendering needs to be specified first
		virtual void active_context() const = 0;
		virtual void inactive_context() const = 0;

	public:
		inline const WindowInfo& getWindowInfo() { return m_window_info; }
		inline oe::utils::GameLoop& getGameloop() {return m_window_gameloop; }

		float getAspect() const;
		float getButton(oe::mouse_buttons button) const;
		float getKey(oe::keys key) const;

		const glm::ivec2& getPosition() const;
		void setPosition(const glm::ivec2& pos);

		const glm::ivec2& getSize() const;
		void setSize(const glm::ivec2& size);

		const std::string& getTitle() const;
		void setTitle(const std::string& title);

		bool getBorderless() const;
		void setBorderless(bool borderless);

		bool getResizeable() const;
		void setResizeable(bool resizeable);

		bool getFullscreen() const;
		void setFullscreen(bool fullscreen);

		const glm::ivec2& getCursorWindow() const;
		void setCursorWindow(const glm::ivec2& cursor_at_window);

		const glm::vec2& getCursorTransformed() const;
		void setCursorTransformed(const glm::vec2& cursor_at_world_space);

		const std::string getClipboard() const;
		void setClipboard(const std::string& str);

		// -- events --
		// connect events
		template<typename Event, auto Listener, typename Instance>
		void connect_listener(const Instance& instance)
		{
			dispatcher_mutex.lock();
			m_window_gameloop.connect_listener<Event, Listener, Instance>(instance);
			dispatcher_mutex.unlock();
		}
		template<typename Event, auto Listener>
		void connect_listener()
		{
			dispatcher_mutex.lock();
			m_window_gameloop.connect_listener<Event, Listener>();
			dispatcher_mutex.unlock();
		}
		// disconenct events
		template<typename Event, auto Listener, typename Instance>
		void disconnect_listener(const Instance& instance)
		{
			dispatcher_mutex.lock();
			m_window_gameloop.disconnect_listener<Event, Listener, Instance>(instance);
			dispatcher_mutex.unlock();
		}
		template<typename Event, auto Listener>
		void disconnect_listener()
		{
			dispatcher_mutex.lock();
			m_window_gameloop.connect_listener<Event, Listener>();
			dispatcher_mutex.unlock();
		}



		virtual std::string getAPI() const = 0;
		virtual std::string getAPIVersion() const = 0;
		virtual std::string getGPU() const = 0;
		virtual std::string getGPUVendor() const = 0;
	};

}