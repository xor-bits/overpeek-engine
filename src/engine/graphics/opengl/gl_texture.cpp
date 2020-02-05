#include "gl_texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assert.h>
#include <spdlog/spdlog.h>

#include "engine/engine.h"
#include "engine/internal_libs.h"



namespace oe::graphics {

	Texture::Texture() {
		p_depth = 0;
		p_width = 0;
		p_height = 0;
		p_target = 0;

		glGenTextures(1, &p_id);
	}

	Texture::~Texture() {
		glDeleteTextures(1, &p_id);
	}

	void Texture::bind() const {
		glBindTexture(p_target, p_id);
	}

	void Texture::unbind() const {
		glBindTexture(p_target, 0);
	}

	void Texture::generateMipMap() {
		bind();
		glGenerateMipmap(p_target);
	}

	void Texture::empty2D(int width, int height) {
		p_target = GL_TEXTURE_2D;
		p_width = width;
		p_height = height;

		bind();

		glTexParameteri(p_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(p_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(p_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(p_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexStorage2D(p_target, 1, GL_RGBA8, width, height);
	}

	void Texture::empty3D(int width, int height, int depth) {
		p_target = GL_TEXTURE_2D_ARRAY;
		p_width = width;
		p_height = height;
		p_depth = depth;

		bind();

		glTexParameteri(p_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(p_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(p_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(p_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(p_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexStorage3D(p_target, 1, GL_RGBA8, width, height, depth);
	}

	void Texture::load2D(const oe::utils::image_data& data) {
		load2D(data.data, data.width, data.height);
	}

	void Texture::load2D(const void* data, int width, int height) {
		p_target = GL_TEXTURE_2D;
		p_width = width;
		p_height = height;

		bind();

		glTexParameteri(p_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(p_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(p_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(p_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		glTexImage2D(p_target, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	void Texture::load3D(const void* data, int width, int height, int depth) {
		p_target = GL_TEXTURE_2D_ARRAY;
		p_width = width;
		p_height = height;
		p_depth = depth;

		bind();

		glTexParameteri(p_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(p_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(p_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(p_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(p_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage3D(p_target, 0, GL_RGBA8, width, height, depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	void Texture::computeShaderBuffer(int width, int height) {
		p_target = GL_TEXTURE_2D;
		p_width = width;
		p_height = height;

		bind();

		glTexParameteri(p_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(p_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexImage2D(p_target, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
	}

	void Texture::data2D(const void* data, int offx, int offy, int width, int height) {
		if (offx + width > p_width || offy + height > p_height) oe_error_terminate("Sub texture bigger than initial texture");
		glTextureSubImage2D(p_id, 0, offx, offy, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	void Texture::data3D(const void* data, int offx, int offy, int offz, int width, int height, int depth) {
		if (offx + width > p_width || offy + height > p_height || offz + depth > p_depth) oe_error_terminate("Sub texture bigger than initial texture");
		glTextureSubImage3D(p_id, 0, offx, offy, offz, width, height, depth, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

}
