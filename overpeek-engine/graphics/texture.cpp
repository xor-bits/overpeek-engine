#include "texture.h"

#include <GL/glew.h>

namespace oe {

	Texture::Texture() {
		glGenTextures(1, &p_id);
		bind();
	}

	Texture::~Texture() {
		glDeleteTextures(1, &p_id);
	}

	void Texture::bind() {
		glBindTexture(p_target, p_id);
	}

	void Texture::unbind() {
		glBindTexture(p_target, 0);
	}

	void Texture::load2D(void* data, int width, int height) {
		p_target = GL_TEXTURE_2D;

		glTexParameteri(p_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(p_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(p_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(p_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(p_target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	void Texture::load3D(void* data, int width, int height, int depth) {
		p_target = GL_TEXTURE_2D_ARRAY;

		glTexParameteri(p_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(p_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(p_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(p_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(p_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage3D(p_target, 0, GL_RGBA, width, height, depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	void Texture::computeShaderBuffer(int width, int height) {
		p_target = GL_TEXTURE_2D;

		glTexParameteri(p_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(p_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexImage2D(p_target, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
	}

}