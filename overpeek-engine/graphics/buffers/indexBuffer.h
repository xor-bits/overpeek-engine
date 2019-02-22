#pragma once

namespace oe {

	class IndexBuffer {
	private:
		unsigned int mID;
		unsigned int mCount;

	public:
		IndexBuffer(unsigned short int *data, int count, unsigned int usage);
		~IndexBuffer();

		void bind();
		void unbind();

		void setBufferData(const void *data, int count);

		inline unsigned int getCount() { return mCount; }
	};

}