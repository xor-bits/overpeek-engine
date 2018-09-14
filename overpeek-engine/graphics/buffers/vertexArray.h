#pragma once

#include <GL/glew.h>
#include <vector>
#include "vertexBuffer.h"

namespace graphics {

	class VertexArray {
	private:
		GLuint mID;
		std::vector<VertexBuffer*> mBuffers;
	public:
		VertexArray();
		~VertexArray();

		void addBuffer(VertexBuffer* buffer, GLuint index);
		void bind();
		void unbind();
	};

}