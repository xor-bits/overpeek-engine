#pragma once

#include "enum.hpp"
#include "engine/graphics/interface/primitive_renderer.hpp"



namespace oe::graphics { class IWindow; class IShader; class ITexture; class IFrameBuffer; class Renderer; }
namespace oe::graphics
{
	class Texture
	{
	private:
		std::shared_ptr<ITexture> m_obj;
	public:
		Texture() {}
		Texture(const TextureInfo& obj_info);
		Texture(const Texture& copy) { m_obj = copy.m_obj; }
		Texture(const Texture&& move) { m_obj = std::move(move.m_obj); }

		inline ITexture* get() const { return m_obj.get(); }
		inline ITexture* operator->() const { return get(); }
		inline Texture& operator=(const Texture& other) { m_obj = other.m_obj; return *this; }
		inline bool operator==(const Texture& other) const { return get() == other.get(); }
		inline bool operator!=(const Texture& other) const { return get() != other.get(); }
		inline operator bool() const { return get(); }
	};

	class Shader
	{
	private:
		std::shared_ptr<IShader> m_obj;
	public:
		Shader() {}
		Shader(const ShaderInfo& obj_info);
		Shader(const Shader& copy) { m_obj = copy.m_obj; }
		Shader(const Shader&& move) { m_obj = std::move(move.m_obj); }

		inline IShader* get() const { return m_obj.get(); }
		inline IShader* operator->() const { return get(); }
		inline Shader& operator=(const Shader& other) { m_obj = other.m_obj; return *this; }
		inline bool operator==(const Shader& other) const { return get() == other.get(); }
		inline bool operator!=(const Shader& other) const { return get() != other.get(); }
		inline operator bool() const { return get(); }
	};

	class Window
	{
	private:
		std::shared_ptr<IWindow> m_obj;
	public:
		Window() {}
		Window(const WindowInfo& obj_info);
		Window(const Window& copy) { m_obj = copy.m_obj; }
		Window(const Window&& move) { m_obj = std::move(move.m_obj); }

		inline IWindow* get() const { return m_obj.get(); }
		inline IWindow* operator->() const { return get(); }
		inline Window& operator=(const Window& other) { m_obj = other.m_obj; return *this; }
		inline bool operator==(const Window& other) const { return get() == other.get(); }
		inline bool operator!=(const Window& other) const { return get() != other.get(); }
		inline operator bool() const { return get(); }
	};

	class FrameBuffer
	{
	private:
		std::shared_ptr<IFrameBuffer> m_obj;
	public:
		FrameBuffer() {}
		FrameBuffer(const FrameBufferInfo& obj_info, Window& window);
		FrameBuffer(const FrameBuffer& copy) { m_obj = copy.m_obj; }
		FrameBuffer(const FrameBuffer&& move) { m_obj = std::move(move.m_obj); }

		inline IFrameBuffer* get() const { return m_obj.get(); }
		inline IFrameBuffer* operator->() const { return get(); }
		inline FrameBuffer& operator=(const FrameBuffer& other) { m_obj = other.m_obj; return *this; }
		inline bool operator==(const FrameBuffer& other) const { return get() == other.get(); }
		inline bool operator!=(const FrameBuffer& other) const { return get() != other.get(); }
		inline operator bool() const { return get(); }
	};

	class PrimitiveRenderer
	{
	private:
		std::shared_ptr<IPrimitiveRenderer> m_obj;
	public:
		PrimitiveRenderer() {}
		PrimitiveRenderer(const RendererInfo& obj_info);
		PrimitiveRenderer(const PrimitiveRenderer& copy) { m_obj = copy.m_obj; }
		PrimitiveRenderer(const PrimitiveRenderer&& move) { m_obj = std::move(move.m_obj); }

		inline IPrimitiveRenderer* get() const { return m_obj.get(); }
		inline IPrimitiveRenderer* operator->() const { return get(); }
		inline PrimitiveRenderer& operator=(const PrimitiveRenderer& other) { m_obj = other.m_obj; return *this; }
		inline bool operator==(const PrimitiveRenderer& other) const { return get() == other.get(); }
		inline bool operator!=(const PrimitiveRenderer& other) const { return get() != other.get(); }
		inline operator bool() const { return get(); }
	};
	
}