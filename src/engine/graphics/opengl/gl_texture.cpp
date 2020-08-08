#include "gl_texture.hpp"

#include "gl_include.hpp"

#include <assert.h>
#include <spdlog/spdlog.h>

#include "engine/engine.hpp"
#include "engine/internal_libs.hpp"



namespace oe::graphics {

	uint32_t gl_format(oe::formats format) {
		switch (format)
		{
		case oe::formats::rgba:
			return GL_RGBA;
		case oe::formats::rgb:
			return GL_RGB;
		case oe::formats::mono:
			return GL_RED;
		}
	}

	uint32_t gl_internalformat(oe::formats format) {
		switch (format)
		{
		case oe::formats::rgba:
			return GL_RGBA8;
		case oe::formats::rgb:
			return GL_RGB8;
		case oe::formats::mono:
			return GL_R8;
		}
	}



	GLTexture::GLTexture(const TextureInfo& texture_info)
		: ITexture(texture_info)
		, m_gl_format(gl_format(texture_info.data_format))
		, m_gl_internalformat(gl_internalformat(texture_info.data_format))
	{
		oe_debug_call("gl_texture");

		// generate texture
		m_target = 0;
		glGenTextures(1, &m_id);

		size_t dimensions = texture_info.size.size();
		if (dimensions != texture_info.offset.size()) { oe_error_terminate("texture_info.size.size() and texture_info.offset.size() do not match"); }

		// load texture
		switch (dimensions)
		{
		case 1:
			if (m_texture_info.empty)
				empty1D(texture_info.size[0]);
			else
				load1D(m_texture_info.data, texture_info.size[0]);
			break;
		case 2:
			if (m_texture_info.empty)
				empty2D(texture_info.size[0], texture_info.size[1]);
			else
				load2D(m_texture_info.data, texture_info.size[0], texture_info.size[1]);
			break;
		case 3:
			if (m_texture_info.empty)
				empty3D(texture_info.size[0], texture_info.size[1], texture_info.size[2]);
			else
				load3D(m_texture_info.data, texture_info.size[0], texture_info.size[1], texture_info.size[2]);
			break;
		default:
			oe_error_terminate("Unsupported texture dimension ({})", dimensions);
			break;
		}

		// mipmaps
		if (m_texture_info.generate_mipmaps) {
			glGenerateMipmap(m_target);
		}
	}

	GLTexture::~GLTexture() {
		glDeleteTextures(1, &m_id);
	}

	void GLTexture::bind() const {
		glBindTexture(m_target, m_id);
	}

	void GLTexture::unbind() const {
		glBindTexture(m_target, 0);
	}

	void GLTexture::bindCompute() const
	{
		glBindImageTexture(0, m_id, 0, false, 0, GL_WRITE_ONLY, m_gl_internalformat);
	}

	void GLTexture::unbindCompute() const
	{
		glBindImageTexture(0, 0, 0, false, 0, GL_WRITE_ONLY, m_gl_internalformat);
	}

	void GLTexture::setData(const TextureInfo& texture_info) {
		m_gl_format = gl_format(texture_info.data_format);
		m_gl_internalformat = gl_internalformat(texture_info.data_format);

		size_t dimensions = texture_info.size.size();
		if (dimensions != texture_info.offset.size()) { oe_error_terminate("texture_info.size.size() and texture_info.offset.size() do not match"); }

		// load texture
		switch (dimensions)
		{
		case 1:
			data1D(m_texture_info.data, texture_info.size[0], texture_info.offset[0]);
			break;
		case 2:
			data2D(m_texture_info.data, texture_info.size[0], texture_info.offset[0], texture_info.size[1], texture_info.offset[1]);
			break;
		case 3:
			data3D(m_texture_info.data, texture_info.size[0], texture_info.offset[0], texture_info.size[1], texture_info.offset[1], texture_info.size[2], texture_info.offset[2]);
			break;
		default:
			oe_error_terminate("Unsupported texture dimension ({})", dimensions);
			break;
		}

		// mipmaps
		if (m_texture_info.generate_mipmaps) {
			glGenerateMipmap(m_target);
		}
	}

	oe::utils::image_data GLTexture::getImageData() const
	{
		if (m_texture_info.size.size() != 2) { oe_error_terminate("Texture dimensions must be 2x2 to getImageData"); }

		size_t size = 1;
		size *= m_texture_info.size[0];
		size *= m_texture_info.size[1];
		size *= oe::sizeofFormat(m_texture_info.data_format);

		uint8_t* data = new uint8_t[size];
		bind();

#ifndef __EMSCRIPTEN__
		// OpenGL method for pulling texture pixel data
		glGetTexImage(m_target, 0, m_gl_format, GL_UNSIGNED_BYTE, data); // read from texture
#else
		// OpenGL ES method for pulling texture data
		uint32_t fbo; // generate temporary framebuffer
		int32_t fbo_current;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo_current);
		glGenFramebuffers(1, &fbo); 
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_target, 0);

		glReadPixels(0, 0, m_texture_info.size[0], m_texture_info.size[1], m_gl_format, GL_UNSIGNED_BYTE, data); // read from framebuffer

		glBindFramebuffer(GL_FRAMEBUFFER, fbo_current); // bind the last active fbo back
		glDeleteFramebuffers(1, &fbo); // and delete the temporary one
#endif

		auto img = oe::utils::image_data(data, m_texture_info.data_format, m_texture_info.size[0], m_texture_info.size[1]);
		
		delete data;
		return img;
	}

#ifdef __EMSCRIPTEN__
#define GL_TEXTURE_1D GL_TEXTURE_2D
#define glTexStorage1D(target, levels, internalformat, width) glTexStorage2D(target, levels, internalformat, width, 1)
#define glTexImage1D(target, level, internalformat, width, border, format, type, data) glTexImage2D(target, level, internalformat, width, 1, border, format, type, data)
#define glTexSubImage1D(m_id, level, x_offset, width, m_gl_format, GL_UNSIGNED_BYTE, data) glTexSubImage2D(m_id, level, x_offset, 0, width, 1, m_gl_format, GL_UNSIGNED_BYTE, data)
#endif

	void GLTexture::empty1D(size_t width) {
		m_target = GL_TEXTURE_1D;
		bind();

		glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexStorage1D(m_target, 1, m_gl_internalformat, width);
	}

	void GLTexture::empty2D(size_t width, size_t height) {
		m_target = GL_TEXTURE_2D;
		bind();

		glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexStorage2D(m_target, 1, m_gl_internalformat, width, height);
	}

	void GLTexture::empty3D(size_t width, size_t height, size_t depth) {
		m_target = GL_TEXTURE_3D;
		bind();

		glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexStorage3D(m_target, 1, m_gl_internalformat, width, height, depth);
	}

	void GLTexture::load1D(const uint8_t* data, size_t width) {
		m_target = GL_TEXTURE_1D;
		bind();

		glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage1D(m_target, 0, m_gl_internalformat, width, 0, m_gl_format, GL_UNSIGNED_BYTE, data);
	}

	void GLTexture::load2D(const uint8_t* data, size_t width, size_t height) {
		m_target = GL_TEXTURE_2D;
		bind();

		glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		glTexImage2D(m_target, 0, m_gl_internalformat, width, height, 0, m_gl_format, GL_UNSIGNED_BYTE, data);
	}

	void GLTexture::load3D(const uint8_t* data, size_t width, size_t height, size_t depth) {
		m_target = GL_TEXTURE_3D;
		bind();

		glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage3D(m_target, 0, m_gl_internalformat, width, height, depth, 0, m_gl_format, GL_UNSIGNED_BYTE, data);
	}

	void GLTexture::data1D(const uint8_t* data, size_t width, size_t x_offset) {
		bind();
		if (m_texture_info.size.size() != 1) { oe_error_terminate("Dimension mismatch {} and 1", m_texture_info.size.size()); }
		if (x_offset + width > m_texture_info.size[0]) 
			oe_error_terminate("Sub texture bigger than initial texture");
		glTexSubImage1D(m_id, 0, x_offset, width, m_gl_format, GL_UNSIGNED_BYTE, data);
	}
	
	void GLTexture::data2D(const uint8_t* data, size_t width, size_t x_offset, size_t height, size_t y_offset) {
		bind();
		if (m_texture_info.size.size() != 1) { oe_error_terminate("Dimension mismatch {} and 2", m_texture_info.size.size()); }
		if (x_offset + width > m_texture_info.size[0] || y_offset + height > m_texture_info.size[1]) 
			oe_error_terminate("Sub texture bigger than initial texture");
		glTexSubImage2D(m_id, 0, x_offset, y_offset, width, height, m_gl_format, GL_UNSIGNED_BYTE, data);
	}
	
	void GLTexture::data3D(const uint8_t* data, size_t width, size_t x_offset, size_t height, size_t y_offset, size_t depth, size_t z_offset) {
		bind();
		if (m_texture_info.size.size() != 1) { oe_error_terminate("Dimension mismatch {} and 3", m_texture_info.size.size()); }
		if (x_offset + width > m_texture_info.size[0] || y_offset + height > m_texture_info.size[1] || z_offset + depth > m_texture_info.size[2]) 
			oe_error_terminate("Sub texture bigger than initial texture");
		glTexSubImage3D(m_id, 0, x_offset, y_offset, z_offset, width, height, depth, m_gl_format, GL_UNSIGNED_BYTE, data);
	}

}
