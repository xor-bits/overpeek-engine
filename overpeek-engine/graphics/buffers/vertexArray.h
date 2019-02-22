#pragma once

#include <vector>

namespace oe {

	class Buffer;
	class VertexArray {
	private:
		unsigned int mID;
		std::vector<Buffer*> mBuffers;
	public:
		VertexArray();
		~VertexArray();

		void addBuffer(Buffer* buffer, unsigned int index);

		//Need to manually call glEnableVertexAttribArray and glVertexAttribPointer
		void addBuffer(Buffer* buffer);
		
		void bind();
		void unbind();
	};

}