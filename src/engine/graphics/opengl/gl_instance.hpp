#include "engine/graphics/interface/instance.hpp"
#include "engine/enum.hpp"



namespace oe::graphics {

	class Window; class Shader; class Texture; class FrameBuffer; class Renderer;
	class GLInstance : public Instance {
	public:
		GLInstance();
		~GLInstance();
		
		// Inherited via Instance
		virtual graphics::Window* createWindow(const WindowInfo& window_config) const override;
		virtual graphics::Shader* createShader(const ShaderInfo& shader_info) const override;
		virtual graphics::Texture* createTexture(const TextureInfo& texture_info) const override;
		virtual graphics::FrameBuffer* createFrameBuffer(const FrameBufferInfo& framebuffer_info) const override;

		virtual void destroyWindow(graphics::Window* window) const override;
		virtual void destroyShader(graphics::Shader* shader) const override;
		virtual void destroyTexture(graphics::Texture* texture) const override;
		virtual void destroyFrameBuffer(graphics::FrameBuffer* framebuffer) const override;
	
	public:
		virtual void blending(oe::modes mode = oe::modes::enable) const override;
		virtual void depth(oe::depth_functions func = oe::depth_functions::always) const override;
		virtual void swapInterval(unsigned int interval = 0) const override;
		virtual void culling(oe::culling_modes c = oe::culling_modes::neither) const override;
		virtual void lineWidth(float w = 1.0f) const override;
		virtual void pointRadius(float w = 1.0f) const override;
		virtual void polygonMode(oe::polygon_mode p = oe::polygon_mode::fill) const override;
	};

}