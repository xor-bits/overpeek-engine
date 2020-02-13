#pragma once

#include "engine/graphics/interface/texture.hpp"



namespace oe::graphics {

	class GLTexture : public Texture {
	private:
		uint32_t m_id;
		int32_t m_target;
		

	private:
		void empty1D(size_t width);
		void empty2D(size_t width, size_t height);
		void empty3D(size_t width, size_t height, size_t depth);

		void load1D(const uint8_t* data, size_t width);
		void load2D(const uint8_t* data, size_t width, size_t height);
		void load3D(const uint8_t* data, size_t width, size_t height, size_t depth);

		void data1D(const uint8_t* data, size_t width, size_t x_offset);
		void data2D(const uint8_t* data, size_t width, size_t x_offset, size_t height, size_t y_offset);
		void data3D(const uint8_t* data, size_t width, size_t x_offset, size_t height, size_t y_offset, size_t depth, size_t z_offset);

	public:
		GLTexture(const TextureInfo& texture_info);
		~GLTexture();

		virtual void setData(const TextureInfo& texture_info) override;

		virtual void bind() override;
		virtual void unbind() override;

	public:
		uint32_t getGLTexture() { return m_id; }
		int32_t getGLTarget() { return m_target; }

	};

}