#pragma once

#include "engine.hpp"
#include "engine/graphics/interface/primitive_renderer.hpp"
#include "engine/graphics/opengl/gl_primitive_renderer.hpp"
#ifdef BUILD_VULKAN
#include "engine/graphics/vulkan/vk_primitive_renderer.hpp"
#endif



namespace oe::graphics
{
	template<primitive_types type, typename buffer_gen, typename vertex_type>
	class BasicPrimitiveRenderer
	{
	private:
		using basic = IBasicPrimitiveRenderer<type, buffer_gen, vertex_type>;
		std::shared_ptr<basic> m_obj;
	public:
		BasicPrimitiveRenderer() {}
		BasicPrimitiveRenderer(const RendererInfo& obj_info)
		{
			auto& engine = oe::Engine::getSingleton();
			if (engine.engine_info.api == graphics_api::OpenGL)
				m_obj = std::make_shared<GLBasicPrimitiveRenderer<type, buffer_gen, vertex_type>>(obj_info);
#ifdef BUILD_VULKAN
			else
				m_obj = std::make_shared<VKPrimitiveRenderer>(obj_info);
#endif
		}
		BasicPrimitiveRenderer(const BasicPrimitiveRenderer& copy) { m_obj = copy.m_obj; }
		BasicPrimitiveRenderer(const BasicPrimitiveRenderer&& move) { m_obj = std::move(move.m_obj); }

		inline basic* get() const { return m_obj.get(); }
		// inline GLPrimitiveRenderer* getGL() const { return reinterpret_cast<GLPrimitiveRenderer*>(m_obj.get()); }
		// inline VKPrimitiveRenderer* getVK() const { return reinterpret_cast<VKPrimitiveRenderer*>(m_obj.get()); }

		inline basic* operator->() const { return get(); }
		inline BasicPrimitiveRenderer& operator=(const BasicPrimitiveRenderer& other) { m_obj = other.m_obj; return *this; }
		inline bool operator==(const BasicPrimitiveRenderer& other) const { return get() == other.get(); }
		inline bool operator!=(const BasicPrimitiveRenderer& other) const { return get() != other.get(); }
		inline operator bool() const { return get(); }

		inline void reset() { m_obj.reset(); }
	};

	using TrianglePrimitiveRenderer = BasicPrimitiveRenderer<primitive_types::triangles, BasicBufferGen<primitive_types::triangles>, VertexData>;
	using QuadPrimitiveRenderer = BasicPrimitiveRenderer<primitive_types::quads, BasicBufferGen<primitive_types::quads>, VertexData>;
	using PrimitiveRenderer = QuadPrimitiveRenderer;
}