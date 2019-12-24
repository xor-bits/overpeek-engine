#pragma once

#include "engine.h"
#include <filesystem>



namespace oe::graphics {

	class TextureManager {
	private:
		static unsigned char* m_textureBuffer;
		static size_t m_textureBufferCurrent;
		static size_t m_textureBufferSize;

		static int m_r;
		static Texture m_texture;
		static size_t m_current_layer;

		static Texture getTexture() {
			return m_texture;
		}

	public:
		/*
		 * 'wh' is width and height of all textures NOTE: All textures must be same size
		 */
		static void init(int wh);

		static size_t coordsToIndex(size_t x, size_t y, size_t c, size_t width, size_t channels);
		static void resize(size_t added_bytes);

		// pair first = gl texture id
		// pair second = texture count
		typedef std::pair<size_t, size_t> multitexture;
		
		// wont free the image data
		static multitexture load(oe::utils::image_data img);
		
		static multitexture load(fs::path path);
		
		static void finish();

		static void bind();

	};

}