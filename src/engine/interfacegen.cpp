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
			m_obj = std::make_shared<GLTexture>(obj_info);
#ifdef BUILD_VULKAN
		else
			m_obj = std::make_shared<VKTexture>(obj_info);
#endif
	}

	Shader::Shader(const ShaderInfo& obj_info)
	{
		auto& engine = oe::Engine::getSingleton();
		if (engine.engine_info.api == graphics_api::OpenGL)
			m_obj = std::make_shared<GLShader>(obj_info);
#ifdef BUILD_VULKAN
		else
			m_obj = std::make_shared<VKShader>(obj_info);
#endif
	}

	Window::Window(const WindowInfo& obj_info)
	{
		auto& engine = oe::Engine::getSingleton();
		if (engine.engine_info.api == graphics_api::OpenGL)
			m_obj = std::make_shared<GLWindow>(oe::Engine::getSingleton().instance, obj_info);
#ifdef BUILD_VULKAN
		else
			m_obj = std::make_shared<VKWindow>(oe::Engine::getSingleton().instance, obj_info);
#endif
	}

	FrameBuffer::FrameBuffer(const FrameBufferInfo& obj_info, const Window& window)
	{
		auto& engine = oe::Engine::getSingleton();
		if (engine.engine_info.api == graphics_api::OpenGL)
			m_obj = std::make_shared<GLFrameBuffer>(obj_info, window);
#ifdef BUILD_VULKAN
		else
			m_obj = std::make_shared<VKFrameBuffer>(obj_info, window);
#endif
	}
}