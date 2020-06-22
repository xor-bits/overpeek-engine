#include "interfacegen.hpp"

#include "engine/graphics/opengl/gl_texture.hpp"
#include "engine/graphics/opengl/gl_shader.hpp"
#include "engine/graphics/opengl/gl_window.hpp"
#include "engine/graphics/opengl/gl_framebuffer.hpp"
#include "engine/graphics/opengl/gl_primitive_renderer.hpp"

#if defined(BUILD_VULKAN)
#include "engine/graphics/vulkan/vk_instance.hpp"
#include "engine/graphics/vulkan/vk_instance.hpp"
#include "engine/graphics/vulkan/vk_texture.hpp"
#include "engine/graphics/vulkan/vk_shader.hpp"
#include "engine/graphics/vulkan/vk_window.hpp"
#include "engine/graphics/vulkan/vk_framebuffer.hpp"
#include "engine/graphics/vulkan/vk_primitive_renderer.hpp"
#endif



namespace oe::graphics
{



	Texture::Texture(const TextureInfo& obj_info)
	{
		auto& engine = oe::Engine::getSingleton();
		if (engine.engine_info.api == graphics_api::OpenGL)
			m_obj = std::make_unique<GLTexture>(obj_info);
#ifdef BUILD_VULKAN
		else
			m_obj = std::make_unique<VKTexture>(obj_info);
#endif
	}

	Shader::Shader(const ShaderInfo& obj_info)
	{
		auto& engine = oe::Engine::getSingleton();
		if (engine.engine_info.api == graphics_api::OpenGL)
			m_obj = std::make_unique<GLShader>(obj_info);
#ifdef BUILD_VULKAN
		else
			m_obj = std::make_unique<VKShader>(obj_info);
#endif
	}

	Window::Window(const WindowInfo& obj_info)
	{
		auto& engine = oe::Engine::getSingleton();
		if (engine.engine_info.api == graphics_api::OpenGL)
			m_obj = std::make_unique<GLWindow>(oe::Engine::getSingleton().instance, obj_info);
#ifdef BUILD_VULKAN
		else
			m_obj = std::make_unique<VKWindow>(oe::Engine::getSingleton().instance, obj_info);
#endif
	}

	FrameBuffer::FrameBuffer(const FrameBufferInfo& obj_info, Window& window)
	{
		auto& engine = oe::Engine::getSingleton();
		if (engine.engine_info.api == graphics_api::OpenGL)
			m_obj = std::make_unique<GLFrameBuffer>(obj_info, window);
#ifdef BUILD_VULKAN
		else
			m_obj = std::make_unique<VKFrameBuffer>(obj_info, window);
#endif
	}

	PrimitiveRenderer::PrimitiveRenderer(const RendererInfo& obj_info)
	{
		auto& engine = oe::Engine::getSingleton();
		if (engine.engine_info.api == graphics_api::OpenGL)
			m_obj = std::make_unique<GLPrimitiveRenderer>(obj_info);
#ifdef BUILD_VULKAN
		else
			m_obj = std::make_unique<VKPrimitiveRenderer>(obj_info);
#endif
	}
}