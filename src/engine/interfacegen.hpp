#pragma once

#include "enum.hpp"

#include <memory>



namespace oe::graphics
{
	/* -- forward declarations -- */
	class ITexture;
	class GLTexture;
	class VKTexture;
	
	class IShader;
	class GLShader;
	class VKShader;
	
	class IWindow;
	class GLWindow;
	class VKWindow;
	
	class IFrameBuffer;
	class GLFrameBuffer;
	class VKFrameBuffer;

	template<primitive_types type, typename buffer_gen, typename vertex_type>
	class BasicPrimitiveRenderer;
	template<oe::primitive_types type>
	struct BasicBufferGen;
	struct VertexData;

	using TrianglePrimitiveRenderer = BasicPrimitiveRenderer<primitive_types::triangles, BasicBufferGen<primitive_types::triangles>, VertexData>;
	using QuadPrimitiveRenderer = BasicPrimitiveRenderer<primitive_types::quads, BasicBufferGen<primitive_types::quads>, VertexData>;
	using PrimitiveRenderer = QuadPrimitiveRenderer;
	/* -- forward declarations -- */



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
		inline GLTexture* getGL() const { return reinterpret_cast<GLTexture*>(m_obj.get()); }
		inline VKTexture* getVK() const { return reinterpret_cast<VKTexture*>(m_obj.get()); }

		inline ITexture* operator->() const { return get(); }
		inline Texture& operator=(const Texture& other) { m_obj = other.m_obj; return *this; }
		inline bool operator==(const Texture& other) const { return get() == other.get(); }
		inline bool operator!=(const Texture& other) const { return get() != other.get(); }
		inline operator bool() const { return get(); }

		inline void reset() { m_obj.reset(); }
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
		inline GLShader* getGL() const { return reinterpret_cast<GLShader*>(m_obj.get()); }
		inline VKShader* getVK() const { return reinterpret_cast<VKShader*>(m_obj.get()); }

		inline IShader* operator->() const { return get(); }
		inline Shader& operator=(const Shader& other) { m_obj = other.m_obj; return *this; }
		inline bool operator==(const Shader& other) const { return get() == other.get(); }
		inline bool operator!=(const Shader& other) const { return get() != other.get(); }
		inline operator bool() const { return get(); }

		inline void reset() { m_obj.reset(); }
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
		inline GLWindow* getGL() const { return reinterpret_cast<GLWindow*>(m_obj.get()); }
		inline VKWindow* getVK() const { return reinterpret_cast<VKWindow*>(m_obj.get()); }

		inline IWindow* operator->() const { return get(); }
		inline Window& operator=(const Window& other) { m_obj = other.m_obj; return *this; }
		inline bool operator==(const Window& other) const { return get() == other.get(); }
		inline bool operator!=(const Window& other) const { return get() != other.get(); }
		inline operator bool() const { return get(); }

		inline void reset() { m_obj.reset(); }
	};

	class FrameBuffer
	{
	private:
		std::shared_ptr<IFrameBuffer> m_obj;
	public:
		FrameBuffer() {}
		FrameBuffer(const FrameBufferInfo& obj_info);
		FrameBuffer(const FrameBuffer& copy) { m_obj = copy.m_obj; }
		FrameBuffer(const FrameBuffer&& move) { m_obj = std::move(move.m_obj); }

		inline IFrameBuffer* get() const { return m_obj.get(); }
		inline GLFrameBuffer* getGL() const { return reinterpret_cast<GLFrameBuffer*>(m_obj.get()); }
		inline VKFrameBuffer* getVK() const { return reinterpret_cast<VKFrameBuffer*>(m_obj.get()); }

		inline IFrameBuffer* operator->() const { return get(); }
		inline FrameBuffer& operator=(const FrameBuffer& other) { m_obj = other.m_obj; return *this; }
		inline bool operator==(const FrameBuffer& other) const { return get() == other.get(); }
		inline bool operator!=(const FrameBuffer& other) const { return get() != other.get(); }
		inline operator bool() const { return get(); }

		inline void reset() { m_obj.reset(); }
	};
}