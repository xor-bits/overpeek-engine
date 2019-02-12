#pragma once

namespace oe {

	class Buffer {
	private:
		unsigned int mID;
		unsigned int mComponentCount;

	public:
		/*
		- "count" means all of the objects in the array
		- "componentCount" is amount of components per vertex
		- "componentSize" is the size of one object in bytes
		- "usage" is either GL_STATIC_DRAW or GL_DYNAMIC_DRAW
		*/
		Buffer(const void *data, int count, unsigned int componentCount, int componentSize, unsigned int usage);
		~Buffer();

		void bind();
		void unbind();

		void setBufferData(const void *data, unsigned int count, unsigned int componentCount, int componentSize);
		void *mapBuffer();
		void unmapBuffer();

		inline unsigned int getComponentCount() { return mComponentCount; }
	};

}