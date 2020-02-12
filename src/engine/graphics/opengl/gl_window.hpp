#pragma once

#include <string>

#include "engine/internal_libs.hpp"
#include "engine/graphics/interface/window.hpp"

#include "engine/graphics/opengl/gl_framebuffer.hpp"
#include "engine/graphics/opengl/gl_renderer.hpp"
#include "engine/graphics/opengl/gl_shader.hpp"
#include "engine/graphics/opengl/gl_texture.hpp"



namespace oe::graphics {
	
	class GLWindow : public Window {
	private:
		bool m_debugging;

	public:
		GLWindow(const Instance* instance, const WindowInfo& window_config);
		~GLWindow();

		void glad(const Instance* instance);
		void glfw(const Instance* instance);

		// Inherited via Window
		virtual void update() override;
		virtual void clear(const glm::vec4& color = glm::vec4(0.18f, 0.18f, 0.20f, 1.0f)) override;
		virtual void viewport() override;
		virtual void swapInterval(uint8_t frames) override;

		virtual graphics::Renderer* createRenderer(const RendererInfo& renderer_info) const override;
		virtual graphics::Shader* createShader(const ShaderInfo& shader_info) const override;
		virtual graphics::Texture* createTexture(const TextureInfo& texture_info) const override;
		virtual graphics::FrameBuffer* createFrameBuffer(const FrameBufferInfo& framebuffer_info) const override;

		virtual void destroyRenderer(graphics::Renderer* renderer) const override;
		virtual void destroyShader(graphics::Shader* shader) const override;
		virtual void destroyTexture(graphics::Texture* texture) const override;
		virtual void destroyFrameBuffer(graphics::FrameBuffer* framebuffer) const override;

		virtual std::string getAPI() const override;
		virtual std::string getAPIVersion() const override;
		virtual std::string getGPU() const override;
		virtual std::string getGPUVendor() const override;
	};

}