#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "renderables/quad.h"
#include "buffers/indexBuffer.h"
#include "buffers/vertexBuffer.h"
#include "buffers/vertexArray.h"

#define BATCH_MAX_SPRITE		10000
#define BATCH_VERTEX_SIZE		sizeof(glm::vec3)
#define BATCH_SPRITE_SIZE		BATCH_VERTEX_SIZE * 4
#define BATCH_MAX_SPRITE_SIZE	BATCH_SPRITE_SIZE * BATCH_MAX_SPRITE
#define BATCH_MAX_INDICES		BATCH_MAX_SPRITE * 6

namespace graphics {

	class BatchRenderer {
	private:
		GLuint mVAO;
		GLuint mVBO;
		IndexBuffer *mIBO;
		GLsizei	mIndexCount;

		glm::vec3 *mBuffer;

		void init();

	public:
		BatchRenderer();
		~BatchRenderer();
		void beginSubmits();
		void submit(Quad *sprite);
		void endSubmits();
		void render();

	};

}