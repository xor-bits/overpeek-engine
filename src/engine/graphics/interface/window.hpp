#pragma once

#include <string>
#include <mutex>

#include "engine/internal_libs.hpp"
#include "engine/enum.hpp"
#include "engine/utility/fileio.hpp"
#include "engine/utility/gameloop.hpp"
#include "engine/utility/extra.hpp"



// glfw forwards
// window
struct GLFWwindow;
extern "C" {
void 	glfwSetWindowPos (GLFWwindow *window, int xpos, int ypos);
void 	glfwSetWindowSize (GLFWwindow *window, int width, int height);
void 	glfwSetWindowTitle (GLFWwindow *window, const char *title);
void 	glfwSetWindowAttrib (GLFWwindow *window, int attrib, int value);
// io
void 	glfwSetCursorPos (GLFWwindow *window, double xpos, double ypos);
void 	glfwSetClipboardString (GLFWwindow *window, const char *string);
const char * 	glfwGetClipboardString (GLFWwindow *window);
}
// macros
extern int oe_glfw_decorated;
extern int oe_glfw_resizeable;

namespace oe::graphics
{
	class IWindow
	{
	public:
		WindowInfo m_window_info{};
		std::mutex dispatcher_mutex{};

		constexpr static size_t max_number_of_keys = 2048;
		constexpr static size_t max_number_of_buttons = 1024;

	protected:
		GLFWwindow* m_window_handle = nullptr;

		std::array<bool, max_number_of_keys> m_keys{};
		std::array<bool, max_number_of_buttons> m_buttons{};

		glm::vec2 m_cursor_transformed = { 0.0f, 0.0f };
		glm::ivec2 m_cursor_window = { 0, 0 };
		float m_aspect_ratio = 0;

		oe::utils::GameLoop m_window_gameloop;
		
		bool m_processing_events = false;

		void postglfw();
		void makeFullscreen();
		void makeWindowed();

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
		[[nodiscard]] constexpr inline const WindowInfo& getWindowInfo() const noexcept
		{
			return m_window_info;
		}

		[[nodiscard]] constexpr inline oe::utils::GameLoop& getGameloop() noexcept
		{
			return m_window_gameloop;
		}
		
		// ----------------------------
		// immediate io getters/setters
		// ----------------------------
		
		[[nodiscard]] inline bool getButton(oe::mouse_buttons button) const
		{
			int32_t num = static_cast<int32_t>(button);
			if (!oe::utils::isInRange(num, 0, max_number_of_buttons))
			{
				spdlog::warn("Invalid button '{}' queried", button);
				return false;
			}
			else
				return m_buttons[num];
		}

		[[nodiscard]] inline bool getKey(oe::keys key) const
		{
			int32_t num = static_cast<int32_t>(key);
			if (!oe::utils::isInRange(num, 0, max_number_of_keys))
			{
				spdlog::warn("Invalid key '{}' queried", key);
				return false;
			}
			else
				return m_keys[num];
		}

		[[nodiscard]] inline const std::string_view getClipboard() const
		{
			constexpr const std::string_view empty_response = "";
			const char* cb = glfwGetClipboardString(m_window_handle);
			return cb == nullptr ? empty_response : cb;
		}
		
		inline void setClipboard(const std::string& str) const
		{
			glfwSetClipboardString(m_window_handle, str.c_str());
		}
		
		// ---------------------------------
		// window appearance getters/setters
		// ---------------------------------

		[[nodiscard]] constexpr inline float getAspect() const noexcept
		{
			return m_aspect_ratio;
		}
		
		[[nodiscard]] constexpr inline const glm::ivec2& getPosition() const noexcept
		{
			return m_window_info.position;
		}
		
		inline void setPosition(const glm::uvec2& pos)
		{
			m_window_info.position = pos;
			glfwSetWindowPos(m_window_handle, m_window_info.position.x, m_window_info.position.y);
		}

		[[nodiscard]] constexpr inline const glm::uvec2& getSize() const noexcept
		{
			return m_window_info.size;
		}

		inline void setSize(const glm::ivec2& size)
		{
			m_window_info.size = size;
			glfwSetWindowSize(m_window_handle, size.x, size.y);
		}

		[[nodiscard]] constexpr inline const std::string& getTitle() const noexcept
		{
			return m_window_info.title;
		}

		inline void setTitle(const std::string& title)
		{
			m_window_info.title = title;
			glfwSetWindowTitle(m_window_handle, title.c_str());
		}

#ifndef __EMSCRIPTEN__ /* WebGL does not support borderless or non-resizeable glfw windows */
		[[nodiscard]] constexpr inline bool getBorderless() const noexcept
		{
			return m_window_info.borderless;
		}

		inline void setBorderless(bool borderless)
		{
			m_window_info.borderless = borderless;
			glfwSetWindowAttrib(m_window_handle, oe_glfw_decorated, !m_window_info.resizeable);
		}

		[[nodiscard]] constexpr inline bool getResizeable() const noexcept
		{
			return m_window_info.resizeable;
		}

		inline void setResizeable(bool resizeable)
		{
			m_window_info.resizeable = resizeable;
			glfwSetWindowAttrib(m_window_handle, oe_glfw_resizeable, m_window_info.resizeable);
		}
#else /* __EMSCRIPTEN__ */
		[[nodiscard]] constexpr inline bool getBorderless() const noexcept { return false; }
		inline void setBorderless(bool borderless) { }
		[[nodiscard]] constexpr inline bool getResizeable() const noexcept { return false; }
		inline void setResizeable(bool resizeable) { }
#endif /* __EMSCRIPTEN__ */

		[[nodiscard]] constexpr inline bool getFullscreen() const noexcept
		{
			return m_window_info.fullscreen;
		}
		
		inline void setFullscreen(bool fullscreen)
		{
			m_window_info.fullscreen = fullscreen;
			if(m_window_info.fullscreen)
				makeFullscreen();
			else
				makeWindowed();
		}

		// -------------------------------
		// cursor position getters/setters
		// -------------------------------

		[[nodiscard]] constexpr inline const glm::ivec2& getCursorWindow() const noexcept
		{
			return m_cursor_window;
		}
		
		inline void setCursorWindow(const glm::ivec2& cursor_at_window)
		{
			m_cursor_window = cursor_at_window;
			glfwSetCursorPos(m_window_handle, m_cursor_window.x, m_cursor_window.y);
		}

		[[nodiscard]] constexpr inline const glm::vec2& getCursorTransformed() const noexcept
		{
			return m_cursor_transformed;
		}

		inline void setCursorTransformed(const glm::vec2& cursor_at_world_space) 
		{
			m_cursor_transformed = cursor_at_world_space;
			m_cursor_transformed.x = oe::utils::map((float)m_cursor_transformed.x, (float)-m_aspect_ratio, (float)m_aspect_ratio, (float)0.0, (float)m_window_info.size.x);
			m_cursor_transformed.y = oe::utils::map((float)m_cursor_transformed.y, (float)-1.0, (float)1.0, (float)0.0, (float)m_window_info.size.y);
			glfwSetCursorPos(m_window_handle, m_cursor_transformed.x, m_cursor_transformed.y);
		}

		// ----------------------------------
		// window + gameloop event connectors
		// ----------------------------------

		// connect events
		template<typename Event, auto Listener, typename Instance>
		void connect_listener(Instance* instance)
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
		void disconnect_listener(Instance* instance)
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

		// --------------------
		// graphics api queries
		// --------------------

		virtual std::string getAPI() const = 0;
		virtual std::string getAPIVersion() const = 0;
		virtual std::string getGPU() const = 0;
		virtual std::string getGPUVendor() const = 0;
	};
}