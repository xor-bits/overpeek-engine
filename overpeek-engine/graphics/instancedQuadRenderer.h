#pragma once

#include <glm/glm.hpp>

namespace oe {

	class Window;
	class VertexArray;
	class Buffer;
	class IndexBuffer;
	class InstancedQuadRenderer
	{
	public:
		struct InstanceData {
			glm::fvec2 pos;
			float texture;
		};

	private:
		VertexArray* m_VAO;
		Buffer* m_VBO;
		InstanceData* m_buffer;
		bool m_buffer_mapped;
		unsigned int m_buffer_current;

	public:
		InstancedQuadRenderer(unsigned int max_instance_count);
		~InstancedQuadRenderer();

		//Maps buffer
		void beginRendering();

		//Unmaps buffer
		void stopRendering();

		//Submit one instance of quad
		void submitInstance(InstanceData data);

		//Draws buffers and then clears them
		void draw(int texture, int textureType);
	};

}
