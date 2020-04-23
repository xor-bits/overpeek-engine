#pragma once

#include <string>

#include "engine/graphics/interface/instance.hpp"
#include "engine/internal_libs.hpp"
#include "engine/enum.hpp"
#include "engine/utility/fileio.hpp"


#define M_NUM_KEYS		2048
#define M_NUM_BUTTONS	1024



class GLFWwindow;
namespace oe::graphics {

	class Window {
	public:
		WindowInfo m_window_info;

	protected:
		GLFWwindow* m_window_handle = nullptr;

		bool m_keys[M_NUM_KEYS];
		bool m_buttons[M_NUM_BUTTONS];

		glm::vec2 m_cursor_transformed = { 0.0f, 0.0f };
		glm::vec2 m_cursor_window = { 0, 0 };
		float m_aspect_ratio = 0;

		void postglfw();

	public:
		Window(const Instance* instance, const WindowInfo& window_config);
		~Window();

		virtual void update() = 0;
		virtual void clear(const glm::vec4& color = oe::colors::clear_color) = 0;
		virtual void viewport() = 0;

		/*Also known as VSync*/
		virtual void swapInterval(uint8_t frames) = 0;

		bool shouldClose();
		void setIcon(const oe::utils::image_data& image);
		void showCursor(bool show);

		virtual Renderer* createRenderer(const RendererInfo& renderer_info) const = 0;
		virtual Shader* createShader(const ShaderInfo& shader_info) const = 0;
		virtual Texture* createTexture(const TextureInfo& texture_info) const = 0;
		virtual FrameBuffer* createFrameBuffer(const FrameBufferInfo& framebuffer_info) const = 0;
		
		virtual void destroyRenderer(Renderer* renderer) const = 0;
		virtual void destroyShader(Shader* shader) const = 0;
		virtual void destroyTexture(Texture* texture) const = 0;
		virtual void destroyFrameBuffer(FrameBuffer* framebuffer) const = 0;

	public:
		inline const WindowInfo& getWindowInfo() { return m_window_info; }

		float aspect();
		float button(oe::mouse_buttons button);
		float key(oe::keys key);

		const glm::vec2& getPosition();
		void setPosition(const glm::vec2& pos);

		const glm::vec2& getSize();
		void setSize(const glm::vec2& size);

		const std::string& getTitle();
		void setTitle(const std::string& title);

		bool getBorderless();
		void setBorderless(bool borderless);

		bool getResizeable();
		void setResizeable(bool resizeable);

		bool getFullscreen();
		void setFullscreen(bool fullscreen);

		const glm::vec2& getCursorWindow();
		void setCursorWindow(const glm::vec2& cursor_at_window);

		const glm::vec2& getCursorTransformed();
		void setCursorTransformed(const glm::vec2& cursor_at_world_space);

		const std::string getClipboard();
		void setClipboard(const std::string& str);



		virtual std::string getAPI() const = 0;
		virtual std::string getAPIVersion() const = 0;
		virtual std::string getGPU() const = 0;
		virtual std::string getGPUVendor() const = 0;
	};

}