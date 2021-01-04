#pragma once

#include "engine/graphics/interface/texture.hpp"



namespace oe::graphics
{
	class GLTexture : public ITexture
	{
	private:
		uint32_t m_id;
		int32_t m_target;

		uint32_t m_gl_format;
		uint32_t m_gl_internalformat;
		uint32_t m_gl_filter;
		uint32_t m_gl_wrap;
		
	private:
		void empty1D(int32_t width);
		void empty2D(int32_t width, int32_t height);
		void empty3D(int32_t width, int32_t height, int32_t depth);

		void load1D(const void* data, oe::TextureInfo::data_types data_type, int32_t width);
		void load2D(const void* data, oe::TextureInfo::data_types data_type, int32_t width, int32_t height);
		void load3D(const void* data, oe::TextureInfo::data_types data_type, int32_t width, int32_t height, int32_t depth);

		void data1D(const void* data, oe::TextureInfo::data_types data_type, int32_t width, int32_t x_offset);
		void data2D(const void* data, oe::TextureInfo::data_types data_type, int32_t width, int32_t x_offset, int32_t height, int32_t y_offset);
		void data3D(const void* data, oe::TextureInfo::data_types data_type, int32_t width, int32_t x_offset, int32_t height, int32_t y_offset, int32_t depth, int32_t z_offset);

	public:
		GLTexture(const TextureInfo& texture_info);
		~GLTexture();

		void setData(const TextureInfo& texture_info) override;
		oe::utils::image_data getImageData() const override;

		void bind() const override;
		void unbind() const override;

		// compute shader only
		void bindCompute() const override;
		void unbindCompute() const override;

	public:
		uint32_t getGLTexture() { return m_id; }
		int32_t getGLTarget() { return m_target; }

		static int32_t gl_max_texture_size;

	};
}