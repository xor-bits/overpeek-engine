#include "gl_texture.hpp"

#include "gl_include.hpp"

#include <assert.h>

#include "engine/engine.hpp"
#include "engine/internal_libs.hpp"
#include "engine/utility/formatted_error.hpp"



namespace oe::graphics
{
	constexpr uint32_t gl_format(oe::formats format)
	{
		switch (format)
		{
		case oe::formats::rgba:
			return GL_RGBA;
		case oe::formats::rgb:
			return GL_RGB;
		case oe::formats::mono:
			return GL_RED;
		}
		return 0;
	}

	constexpr uint32_t gl_internalformat(oe::formats format)
	{
		switch (format)
		{
		case oe::formats::rgba:
			return GL_RGBA8;
		case oe::formats::rgb:
			return GL_RGB8;
		case oe::formats::mono:
			return GL_R8;
		}
		return 0;
	}

	constexpr uint32_t gl_wrap(oe::texture_wrap wrap)
	{
		switch (wrap)
		{
		case oe::texture_wrap::repeat:
			return GL_REPEAT;
		case oe::texture_wrap::mirrored_repeat:
			return GL_MIRRORED_REPEAT;
		case oe::texture_wrap::clamp_to_edge:
			return GL_CLAMP_TO_EDGE;
		case oe::texture_wrap::clamp_to_border:
			return GL_CLAMP_TO_BORDER;
		}
		return 0;
	}

	constexpr uint32_t gl_filter(oe::texture_filter filter)
	{
		switch (filter)
		{
		case oe::texture_filter::linear:
			return GL_LINEAR;
		case oe::texture_filter::nearest:
			return GL_NEAREST;
		}
		return 0;
	}

	int32_t GLTexture::gl_max_texture_size = -1;

	GLTexture::GLTexture(const TextureInfo& texture_info)
		: ITexture(texture_info)
		, m_gl_format(gl_format(texture_info.data_format))
		, m_gl_internalformat(gl_internalformat(texture_info.data_format))
		, m_gl_filter(gl_filter(texture_info.filter))
		, m_gl_wrap(gl_wrap(texture_info.wrap))
	{
		oe_debug_call("gl_texture");
		if(gl_max_texture_size == -1)
			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &gl_max_texture_size);

		// generate texture
		m_target = 0;
		glGenTextures(1, &m_id);


		// no glTexStorage below gl 4.2, workaround with null texture
		if(m_texture_info.empty && oe::Engine::getSingleton().instance->versionNumber() < 42)
		{
			m_texture_info.empty = false;
			m_texture_info.data = nullptr;
		}

		// load texture
		size_t dimensions = texture_info.size_offset.size();
		switch (dimensions)
		{
		case 1:
			if (m_texture_info.empty)
				empty1D(texture_info.size_offset[0].first);
			else
				load1D(m_texture_info.data, texture_info.size_offset[0].first);
			break;
		case 2:
			if (m_texture_info.empty)
				empty2D(texture_info.size_offset[0].first, texture_info.size_offset[1].first);
			else
				load2D(m_texture_info.data, texture_info.size_offset[0].first, texture_info.size_offset[1].first);
			break;
		case 3:
			if (m_texture_info.empty)
				empty3D(texture_info.size_offset[0].first, texture_info.size_offset[1].first, texture_info.size_offset[2].first);
			else
				load3D(m_texture_info.data, texture_info.size_offset[0].first, texture_info.size_offset[1].first, texture_info.size_offset[2].first);
			break;
		default:
			throw oe::utils::formatted_error("Unsupported texture dimension ({})", dimensions);
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

	void GLTexture::setData(const TextureInfo& texture_info)
	{
		m_texture_info = texture_info;
		m_gl_format = gl_format(m_texture_info.data_format);
		m_gl_internalformat = gl_internalformat(m_texture_info.data_format);
		m_gl_filter = gl_filter(m_texture_info.filter);
		m_gl_wrap = gl_wrap(m_texture_info.wrap);

		size_t dimensions = m_texture_info.size_offset.size();

		// load texture
		switch (dimensions)
		{
		case 1:
			data1D(m_texture_info.data,
				m_texture_info.size_offset[0].first, m_texture_info.size_offset[0].second);
			break;
		case 2:
			data2D(m_texture_info.data,
				m_texture_info.size_offset[0].first, m_texture_info.size_offset[0].second,
				m_texture_info.size_offset[1].first, m_texture_info.size_offset[1].second);
			break;
		case 3:
			data3D(m_texture_info.data,
				m_texture_info.size_offset[0].first, m_texture_info.size_offset[0].second,
				m_texture_info.size_offset[1].first, m_texture_info.size_offset[1].second,
				m_texture_info.size_offset[2].first, m_texture_info.size_offset[2].second);
			break;
		default:
			throw oe::utils::formatted_error("Unsupported texture dimension ({})", dimensions);
			break;
		}

		// mipmaps
		if (m_texture_info.generate_mipmaps) {
			glGenerateMipmap(m_target);
		}
	}

	oe::utils::image_data GLTexture::getImageData() const
	{
		if (m_texture_info.size_offset.size() != 2)
			throw oe::utils::formatted_error("Texture dimensions must be 2x2 to getImageData");

		size_t size = 1;
		size *= m_texture_info.size_offset[0].first;
		size *= m_texture_info.size_offset[1].first;
		size *= oe::sizeofFormat(m_texture_info.data_format);

		auto img = oe::utils::image_data(m_texture_info.data_format, m_texture_info.size_offset[0].first, m_texture_info.size_offset[1].first);
		GLTexture::bind();

#ifndef __EMSCRIPTEN__
		// OpenGL method for pulling texture pixel data
		glGetTexImage(m_target, 0, m_gl_format, GL_UNSIGNED_BYTE, img.data); // read from texture
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
		
		return img;
	}

#ifdef __EMSCRIPTEN__
#define GL_TEXTURE_1D GL_TEXTURE_2D
#define glTexStorage1D(target, levels, internalformat, width) glTexStorage2D(target, levels, internalformat, width, 1)
#define glTexImage1D(target, level, internalformat, width, border, format, type, data) glTexImage2D(target, level, internalformat, width, 1, border, format, type, data)
#define glTexSubImage1D(m_id, level, x_offset, width, m_gl_format, GL_UNSIGNED_BYTE, data) glTexSubImage2D(m_id, level, x_offset, 0, width, 1, m_gl_format, GL_UNSIGNED_BYTE, data)
#endif

	void clamp_size(char name, int32_t& dim)
	{
		int32_t original = dim;
		dim = std::min(GLTexture::gl_max_texture_size, dim);
		
		if(original != dim)
			spdlog::warn("Texture dim<{}> {} was clamped to {}", name, original, dim);
	}

	void GLTexture::empty1D(int32_t width) {
		m_target = GL_TEXTURE_1D;
		GLTexture::bind();

		clamp_size('x', width);

		glTexParameteri(m_target, GL_TEXTURE_WRAP_S, m_gl_wrap);
		glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, m_gl_filter);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, m_gl_filter);

		glTexStorage1D(m_target, 1, m_gl_internalformat, width);
	}

	void GLTexture::empty2D(int32_t width, int32_t height) {
		m_target = GL_TEXTURE_2D;
		GLTexture::bind();

		clamp_size('x', width);
		clamp_size('y', height);

		glTexParameteri(m_target, GL_TEXTURE_WRAP_S, m_gl_wrap);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_T, m_gl_wrap);
		glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, m_gl_filter);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, m_gl_filter);

		glTexStorage2D(m_target, 1, m_gl_internalformat, width, height);
	}

	void GLTexture::empty3D(int32_t width, int32_t height, int32_t depth) {
		m_target = GL_TEXTURE_3D;
		GLTexture::bind();

		clamp_size('x', width);
		clamp_size('y', height);
		clamp_size('z', depth);

		glTexParameteri(m_target, GL_TEXTURE_WRAP_S, m_gl_wrap);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_T, m_gl_wrap);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_R, m_gl_wrap);
		glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, m_gl_filter);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, m_gl_filter);

		glTexStorage3D(m_target, 1, m_gl_internalformat, width, height, depth);
	}

	void GLTexture::load1D(const uint8_t* data, int32_t width) {
		m_target = GL_TEXTURE_1D;
		GLTexture::bind();

		clamp_size('x', width);

		glTexParameteri(m_target, GL_TEXTURE_WRAP_S, m_gl_wrap);
		glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, m_gl_filter);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, m_gl_filter);

		glTexImage1D(m_target, 0, m_gl_internalformat, width, 0, m_gl_format, GL_UNSIGNED_BYTE, data);
	}

	void GLTexture::load2D(const uint8_t* data, int32_t width, int32_t height) {
		m_target = GL_TEXTURE_2D;
		GLTexture::bind();

		clamp_size('x', width);
		clamp_size('y', height);

		glTexParameteri(m_target, GL_TEXTURE_WRAP_S, m_gl_wrap);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_T, m_gl_wrap);
		glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, m_gl_filter);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, m_gl_filter);
		
		glTexImage2D(m_target, 0, m_gl_internalformat, width, height, 0, m_gl_format, GL_UNSIGNED_BYTE, data);
	}

	void GLTexture::load3D(const uint8_t* data, int32_t width, int32_t height, int32_t depth) {
		m_target = GL_TEXTURE_3D;
		GLTexture::bind();

		clamp_size('x', width);
		clamp_size('y', height);
		clamp_size('z', depth);

		glTexParameteri(m_target, GL_TEXTURE_WRAP_S, m_gl_wrap);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_T, m_gl_wrap);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_R, m_gl_wrap);
		glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, m_gl_filter);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, m_gl_filter);

		glTexImage3D(m_target, 0, m_gl_internalformat, width, height, depth, 0, m_gl_format, GL_UNSIGNED_BYTE, data);
	}

	void test(size_t size_a, size_t size_b)
	{
		if (size_a != size_b)
			throw oe::utils::formatted_error("Dimension mismatch {} and {}", size_a, size_b);
	}

	void GLTexture::data1D(const uint8_t* data, int32_t width, int32_t x_offset)
	{
		x_offset = std::abs(x_offset);
		width = std::clamp(width, 0, m_texture_info.size_offset[0].first - x_offset);

		test(m_texture_info.size_offset.size(), 1);

		GLTexture::bind();
		glTexSubImage1D(m_id, 0, x_offset, width, m_gl_format, GL_UNSIGNED_BYTE, data);
	}
	
	void GLTexture::data2D(const uint8_t* data, int32_t width, int32_t x_offset, int32_t height, int32_t y_offset)
	{
		x_offset = std::abs(x_offset);
		y_offset = std::abs(y_offset);
		width = std::clamp(width, 0, m_texture_info.size_offset[0].first - x_offset);
		height = std::clamp(height, 0, m_texture_info.size_offset[1].first - y_offset);

		test(m_texture_info.size_offset.size(), 2);

		GLTexture::bind();
		glTexSubImage2D(m_id, 0, x_offset, y_offset, width, height, m_gl_format, GL_UNSIGNED_BYTE, data);
	}
	
	void GLTexture::data3D(const uint8_t* data, int32_t width, int32_t x_offset, int32_t height, int32_t y_offset, int32_t depth, int32_t z_offset)
	{
		x_offset = std::abs(x_offset);
		y_offset = std::abs(y_offset);
		z_offset = std::abs(z_offset);
		width = std::clamp(width, 0, m_texture_info.size_offset[0].first - x_offset);
		height = std::clamp(height, 0, m_texture_info.size_offset[1].first - y_offset);
		depth = std::clamp(depth, 0, m_texture_info.size_offset[2].first - z_offset);

		test(m_texture_info.size_offset.size(), 3);

		GLTexture::bind();
		glTexSubImage3D(m_id, 0, x_offset, y_offset, z_offset, width, height, depth, m_gl_format, GL_UNSIGNED_BYTE, data);
	}

}
