#pragma once

#include "vertexBuffer.h"

#include <vector>



namespace oe::graphics {

	class VertexArray {
	private:
		unsigned int p_id = 0;
		std::vector<VertexBuffer*> p_buffers;
	
	public:
		VertexArray();
		~VertexArray();
		
		void bind();
		void unbind();

		void addBuffer(VertexBuffer* buffer, unsigned int location);
	};

}