#pragma once

#include "buffer.hpp"
#include "engine/enum.hpp"
#include "engine/graphics/vertexData.hpp"



namespace oe::graphics
{
	class VertexBuffer : public Buffer
	{
	private:
		int32_t p_components_per_vertex;

	public:
		VertexBuffer(const void* data, int32_t size, unsigned int components_per_vertex, oe::types buffer_type);

		void attrib(uint32_t index, int32_t components, int32_t startBytes) const;

		inline int32_t getComponentsPerVertex() { return p_components_per_vertex; }
	};
}