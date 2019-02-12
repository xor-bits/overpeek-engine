#pragma once

#include <GL/glew.h>
#include <vector>
#include "buffer.h"

namespace oe {

	class VertexArray {
	private:
		GLuint mID;
		std::vector<Buffer*> mBuffers;
	public:
		VertexArray();
		~VertexArray();

		void addBuffer(Buffer* buffer, GLuint index);

		//Need to manually call glEnableVertexAttribArray and glVertexAttribPointer
		void addBuffer(Buffer* buffer);
		
		void bind();
		void unbind();
	};

}