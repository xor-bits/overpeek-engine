#pragma once

#include <GL/glew.h>
#include <vector>
#include "buffer.h"

namespace graphics {

	class VertexArray {
	private:
		GLuint mID;
		std::vector<Buffer*> mBuffers;
	public:
		VertexArray();
		~VertexArray();

		void addBuffer(Buffer* buffer, GLuint index);
		void bind();
		void unbind();
	};

}