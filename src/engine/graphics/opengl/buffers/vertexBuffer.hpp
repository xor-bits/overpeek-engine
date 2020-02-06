#pragma once

#include "buffer.hpp"
#include "engine/enum.hpp"



namespace oe::graphics {

	class VertexBuffer : public Buffer
	{
	private:
		size_t p_components_per_vertex;

	public:
		VertexBuffer(const void* data, size_t size, unsigned int components_per_vertex, oe::types buffer_type);

		void attrib(int index, int components, size_t startBytes);

		inline size_t getComponentsPerVertex() { return p_components_per_vertex; }
	};

}